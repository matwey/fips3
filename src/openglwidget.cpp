/*
 *  Copyright (C) 2017  Matwey V. Kornilov <matwey.kornilov@gmail.com>
 *                      Konstantin Malanchev <hombit@gmail.com>
 *
 *  This program is free software: you can redistribute it and/or modify it
 *  under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or (at
 *  your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful, but
 *  WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser
 *  General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <QFile>
#include <QPoint>

#include <openglplan.h>
#include <openglplanfactory.h>
#include <openglwidget.h>
#include <openglshaderprogram.h>
#include <utils/swapbytes.h>

namespace {
struct HDUValueGetter {
	const QPoint& image_position;

	template<class T> double operator() (const FITS::DataUnit<T>& data) const {
		const auto data_ptr = data.data() + image_position.y() * data.width() + image_position.x();

#if Q_BYTE_ORDER == Q_LITTLE_ENDIAN
		const auto raw_value = Utils::swap_bytes(*data_ptr);
#else
		const auto raw_value = *data_ptr;
#endif

		return raw_value;
	}

	double operator() (const FITS::EmptyDataUnit&) const {
		Q_ASSERT(0);
		Q_UNREACHABLE();
	}
};
}

OpenGLWidget::PlanCreationError::PlanCreationError():
	Utils::Exception("Cannot create appropriate OpenGL plan for current configuration") {
}
void OpenGLWidget::PlanCreationError::raise() const {
	throw *this;
}
QException* OpenGLWidget::PlanCreationError::clone() const {
	return new OpenGLWidget::PlanCreationError(*this);
}

OpenGLWidget::PlanInitializationError::PlanInitializationError(const AbstractOpenGLPlan& plan):
	Utils::Exception(QString("Cannot initialize OpenGL plan ") + plan.name()) {
}
void OpenGLWidget::PlanInitializationError::raise() const {
	throw *this;
}
QException* OpenGLWidget::PlanInitializationError::clone() const {
	return new OpenGLWidget::PlanInitializationError(*this);
}

OpenGLWidget::ShaderLoadError::ShaderLoadError(GLenum gl_error_code):
	OpenGLException("Cannot load the shader", gl_error_code) {
}
void OpenGLWidget::ShaderLoadError::raise() const {
	throw *this;
}
QException* OpenGLWidget::ShaderLoadError::clone() const {
	return new OpenGLWidget::ShaderLoadError(*this);
}
OpenGLWidget::ShaderBindError::ShaderBindError(GLenum gl_error_code):
	OpenGLException("Cannot bind the shader", gl_error_code) {
}
void OpenGLWidget::ShaderBindError::raise() const {
	throw *this;
}
QException* OpenGLWidget::ShaderBindError::clone() const {
	return new OpenGLWidget::ShaderBindError(*this);
}
OpenGLWidget::ShaderCompileError::ShaderCompileError(GLenum gl_error_code):
	OpenGLException("Cannot compile the shader", gl_error_code) {
}
void OpenGLWidget::ShaderCompileError::raise() const {
	throw *this;
}
QException* OpenGLWidget::ShaderCompileError::clone() const {
	return new OpenGLWidget::ShaderCompileError(*this);
}

OpenGLWidget::OpenGLWidget(QWidget *parent, const FITS::AbstractHeaderDataUnit& hdu):
	QOpenGLWidget(parent),
	hdu_(&hdu),
	opengl_transform_(this),
	widget_to_fits_(this),
	fits_to_wcs_(this),
	shader_uniforms_(new OpenGLShaderUniforms(1, 1, 0, 1)),
	colormaps_{{
		openGL_unique_ptr<OpenGLColorMap>(new GrayscaleColorMap() , colormap_deleter_type(this)),
		openGL_unique_ptr<OpenGLColorMap>(new PurpleBlueColorMap(), colormap_deleter_type(this)),
	}},
	colormap_index_(0) {

	connect(&viewrect_, SIGNAL(viewChanged(const QRectF&)), this, SLOT(viewChanged(const QRectF&)));
	connect(&viewrect_, SIGNAL(scrollChanged(const QRect&)), this, SLOT(update()));
	connect(this, SIGNAL(rotationChanged(double)), this, SLOT(update()));
	connect(this, SIGNAL(horizontalFlipChanged(bool)), this, SLOT(update()));
	connect(this, SIGNAL(verticalFlipChanged(bool)), this, SLOT(update()));
	setMouseTracking(true); // We need it to catch mouseEvent when mouse buttons aren't pressed
}

void OpenGLWidget::initializeGL() {
	initializeOpenGLFunctions();

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glDisable(GL_DEPTH_TEST);

	for (auto& x: colormaps_) {
		x->initialize();
	}

	// Should be after colormaps_ initialization
	initializeGLObjects();
}

void OpenGLWidget::initializeGLObjects() {
	OpenGLPlanFactory plan_factory{*context()};

	openGL_unique_ptr<AbstractOpenGLPlan> new_plan{hdu_->apply(plan_factory), OpenGLDeleter<AbstractOpenGLPlan>(this)};
	if (!new_plan) throw PlanCreationError();

	if (!new_plan->initialize()) throw PlanInitializationError(*new_plan);
	new_plan->program().bind();

	plan_ = std::move(new_plan);
	// If no exceptions were thrown then we can put new objects to object's member pointers
	viewrect_.setBorder(plan_->plane().borderRect(rotation()));
	widget_to_fits_.setScale(plan_->plane().scale());
	widget_to_fits_.setImageSize(image_size());
	fits_to_wcs_.setWcsMatrix(WcsData(hdu_->header()).matrix());

	emit planInitialized(*plan_);

	shader_uniforms_.reset(new OpenGLShaderUniforms(plan_->channels(), plan_->channel_size(), hdu_->header().bzero(), hdu_->header().bscale()));
	shader_uniforms_->setMinMax(plan_->hduMinMax());
	shader_uniforms_->setColorMapSize(colormaps_[colormap_index_]->width());
}

void OpenGLWidget::resizeEvent(QResizeEvent* event) {
	QOpenGLWidget::resizeEvent(event);

	const auto new_widget_size = event->size();
	auto old_widget_size = event->oldSize();
	// event->oldSize() for the first call of resizeEvent equals -1,-1
	if (old_widget_size.width() < 0 || old_widget_size.height() < 0) {
		fitViewrect();
	} else {
		const auto new_viewrect_width  = viewrect_.view().width()  * (new_widget_size.width()  - 1.0) / (old_widget_size.width()  - 1.0);
		const auto new_viewrect_height = viewrect_.view().height() * (new_widget_size.height() - 1.0) / (old_widget_size.height() - 1.0);
		QRectF new_view(viewrect_.view());
		new_view.setSize({new_viewrect_width, new_viewrect_height});
		viewrect_.setView(new_view);
	}

	widget_to_fits_.setWidgetSize(new_widget_size);
}

void OpenGLWidget::paintGL() {
	glClear(GL_COLOR_BUFFER_BIT);

	plan_->program().bind();
	plan_->program().setMVPUniform(opengl_transform_.transformMatrix());
	plan_->program().setCUniform(shader_uniforms_->get_c(), shader_uniforms_->channels);
	plan_->program().setZUniform(shader_uniforms_->get_z(), shader_uniforms_->channels);

	plan_->imageTexture().bind(OpenGLShaderProgram::image_texture_index);
	colormaps_[colormap_index_]->bind(OpenGLShaderProgram::colormap_texture_index);

	plan_->draw();
}

void OpenGLWidget::setHDU(const FITS::AbstractHeaderDataUnit &hdu) {
	const auto old_image_size = image_size();
	const auto old_hdu = hdu_;
	hdu_ = &hdu;
	try {
		makeCurrent();
		initializeGLObjects();
		if (image_size() != old_image_size) {
			fitViewrect();
		} else {
			update();
		}
		doneCurrent();
	} catch (const std::exception& e) {
		doneCurrent();
		hdu_ = old_hdu;
		throw e;
	}
}

void OpenGLWidget::changeLevels(const std::pair<double, double>& minmax) {
	shader_uniforms_->setMinMax(minmax);
	update();
}

void OpenGLWidget::changeColorMap(int colormap_index) {
	Q_ASSERT(colormap_index >= 0 && colormap_index < static_cast<int>(colormaps_.size()));
	if (colormap_index != colormap_index_) {
		colormap_index_ = colormap_index;
		shader_uniforms_->setColorMapSize(colormaps_[colormap_index]->width());
		update();
	}
}

QSize OpenGLWidget::sizeHint() const {
	return image_size();
}

Pixel OpenGLWidget::pixelFromWidgetCoordinate(const QPoint &widget_coord) {
	const auto p = widget_to_fits_.transform(widget_coord.x(), widget_coord.y());
	const QPoint position(p.x(), p.y());

	const bool inside_image = QRect({0, 0}, image_size()).contains(position);
	if (!inside_image) {
		return Pixel(position);
	}

	const auto wcs_vector = fits_to_wcs_.transform(p);
    qDebug() << wcs_vector.x() << " " << wcs_vector.y();
	const QPoint wcs_position(wcs_vector.x(), wcs_vector.y());
	const auto value = hdu_->header().bscale() * hdu_->data().apply(HDUValueGetter{position}) + hdu_->header().bzero();
	return Pixel(wcs_position, value);
}

void OpenGLWidget::viewChanged(const QRectF& view_rect) {
	opengl_transform_.setViewrect(view_rect);
	widget_to_fits_.setViewrect(view_rect);
}

void OpenGLWidget::setRotation(double angle) {
	if (rotation() == angle) return;

	QMatrix4x4 viewrect_rotation_matrix;
	viewrect_rotation_matrix.rotate(rotation()-angle, 0, 0, 1); // Rotation in viewrect coordinates is clockwise
	const auto new_view_center = viewrect_rotation_matrix.map(viewrect_.view().center());
	auto new_view = viewrect_.view();
	new_view.moveCenter(new_view_center);
	viewrect_.setView(new_view);
	viewrect_.setBorder(plan_->plane().borderRect(angle));

	opengl_transform_.setRotation(angle);
	widget_to_fits_.setRotation(angle);

	emit rotationChanged(rotation());
}

void OpenGLWidget::flipViewrect(Qt::Axis flip_axis) {
	QMatrix4x4 rotation_matrix;
	rotation_matrix.rotate(-rotation(), 0, 0, 1);
	auto unrotated_view_center = rotation_matrix.transposed().map(viewrect_.view().center());
	switch (flip_axis) {
		case Qt::XAxis:
			unrotated_view_center.setX(-unrotated_view_center.x());
			break;
		case Qt::YAxis:
			unrotated_view_center.setY(-unrotated_view_center.y());
			break;
		default:
			Q_ASSERT(false);
	}
	const auto view_center = rotation_matrix.map(unrotated_view_center);
	auto new_view = viewrect_.view();
	new_view.moveCenter(view_center);
	viewrect_.setView(new_view);
}

void OpenGLWidget::setHorizontalFlip(bool flip) {
	if (horizontalFlip() == flip) return;

	flipViewrect(Qt::XAxis);

	opengl_transform_.setHorizontalFlip(flip);
	widget_to_fits_.setHorizontalFlip(flip);

	emit horizontalFlipChanged(horizontalFlip());
}

void OpenGLWidget::setVerticalFlip(bool flip) {
	if (verticalFlip() == flip) return;

	flipViewrect(Qt::YAxis);

	opengl_transform_.setVerticalFlip(flip);
	widget_to_fits_.setVerticalFlip(flip);

	emit verticalFlipChanged(verticalFlip());
}

const QString& OpenGLWidget::planName() const {
	static const QString none("(none)");
	return (plan_ ? plan_->name() : none);
}
