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
	int layer;

	template<class T> double operator() (const FITS::DataUnit<T>& data) const {
		const auto w = data.width();
		const auto h = data.height();
		const auto f = w * h;
		const auto data_ptr = data.data() + layer * f + image_position.y() * w + image_position.x();

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
	fit_to_window_(false),
	opengl_transform_(this),
	widget_to_fits_(this),
	shader_uniforms_(new OpenGLShaderUniforms(1, 1, 0, 1)),
	colormaps_{{
		openGL_unique_ptr<OpenGLColorMap>(new GrayscaleColorMap() , colormap_deleter_type(this)),
		openGL_unique_ptr<OpenGLColorMap>(new PurpleBlueColorMap(), colormap_deleter_type(this)),
	}},
	colormap_index_(0),
	layer_(0) {

	connect(&viewrect_, SIGNAL(scaleChanged(float)), this, SLOT(scaleChanged(float)));
	connect(&viewrect_, SIGNAL(virtualPosChanged(const QPoint&)), this, SLOT(virtualPosChanged(const QPoint&)));
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
	opengl_transform_.setImageSize(image_size());
	widget_to_fits_.setImageSize(image_size());
	viewrect_.setBorder(opengl_transform_.border());

	emit planInitialized(*plan_);

	shader_uniforms_.reset(new OpenGLShaderUniforms(plan_->channels(), plan_->channel_size(), hdu_->header().bzero(), hdu_->header().bscale()));
	shader_uniforms_->setMinMax(plan_->hduMinMax());
	shader_uniforms_->setColorMapSize(colormaps_[colormap_index_]->width());
}

void OpenGLWidget::resizeEvent(QResizeEvent* event) {
	QOpenGLWidget::resizeEvent(event);

	const auto new_widget_size = event->size();
	const auto old_widget_size = event->oldSize();

	opengl_transform_.setWidgetSize(new_widget_size);
	widget_to_fits_.setWidgetSize(new_widget_size);

	// event->oldSize() for the first call of resizeEvent equals -1,-1
	if (fitToWindow() || old_widget_size.width() < 0 || old_widget_size.height() < 0) {
		fitViewrect();
	}
}

void OpenGLWidget::wheelEvent(QWheelEvent* event) {
	/* One wheel tick is 120 eighths of degree */
	const double log_factor_per_eighth = 0.05 / 120;
	const auto zoom_factor = std::exp(log_factor_per_eighth * event->delta());

	zoom(zoom_factor, event->pos());
}

void OpenGLWidget::paintGL() {
	glClear(GL_COLOR_BUFFER_BIT);

	plan_->program().bind();
	plan_->program().setMVPUniform(opengl_transform_.transformMatrix());
	plan_->program().setCUniform(shader_uniforms_->get_c(), shader_uniforms_->channels);
	plan_->program().setZUniform(shader_uniforms_->get_z(), shader_uniforms_->channels);
	plan_->program().setLayerUniform(layer());

	plan_->imageTexture().bind(OpenGLShaderProgram::image_texture_index);
	colormaps_[colormap_index_]->bind(OpenGLShaderProgram::colormap_texture_index);

	plan_->draw();
}

void OpenGLWidget::setHDU(const FITS::AbstractHeaderDataUnit &hdu) {
	const auto old_image_size = image_size();
	const auto old_hdu = hdu_;
	const auto old_layer = layer();

	hdu_ = &hdu;
	try {
		makeCurrent();
		initializeGLObjects();
		if (image_size() != old_image_size) {
			setLayer(0);
			fitViewrect();
		} else {
			if (old_layer >= depth()) {
				setLayer(0);
			}
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

Pixel OpenGLWidget::pixelFromWidgetCoordinate(const QPoint& widget_coord) {
	const auto p = widget_to_fits_.transform(widget_coord.x(), widget_coord.y());
	const QPoint position(p.x(), p.y());

	const bool inside_image = QRect({0, 0}, image_size()).contains(position);
	if (!inside_image) {
		return Pixel(position);
	}

	const auto value = hdu_->header().bscale() * hdu_->data().apply(HDUValueGetter{position, layer()}) + hdu_->header().bzero();
	return Pixel(position, value);
}

void OpenGLWidget::setFitToWindow(bool fit) {
	if (fitToWindow() == fit) return;

	fit_to_window_ = fit;

	if (fit_to_window_) fitViewrect();

	emit fitToWindowChanged(fit_to_window_);
}

void OpenGLWidget::zoom(double zoom_factor) {
	zoom(zoom_factor, rect().center());
}

void OpenGLWidget::zoom(double zoom_factor, const QPoint& fixed_point) {
	setFitToWindow(false);

	const auto old_vpos = viewrect().virtualPos();
	const auto old_scale = viewrect().scale();

	viewrect().zoom(zoom_factor);

	const auto scale = viewrect().scale();
	const QPointF diff = old_vpos - fixed_point - QPointF{0.5, 0.5};
	const QPointF new_vpos = QPointF{0.5, 0.5} + fixed_point + diff * scale / old_scale;

	viewrect().setVirtualPos(new_vpos.toPoint());
}

void OpenGLWidget::scaleChanged(float scale) {
	opengl_transform_.setScale(scale);
	widget_to_fits_.setScale(scale);

	update();
}

void OpenGLWidget::virtualPosChanged(const QPoint& vpos) {
	opengl_transform_.setVirtualPos(vpos);
	widget_to_fits_.setVirtualPos(vpos);

	update();
}

void OpenGLWidget::setRotation(double angle) {
	if (rotation() == angle) return;

	QMatrix4x4 rot;
	rot.rotate(rotation() - angle, 0, 0, 1);

	const auto fixed_point = rect().center();
	const auto scale = viewrect().scale();
	const auto old_vpos = viewrect().virtualPos();
	const auto old_border = viewrect().border();
	const auto old_bvec = QPointF{
		0.5 * scale * old_border.width(),
		0.5 * scale * old_border.height()};

	opengl_transform_.setRotation(angle);
	widget_to_fits_.setRotation(angle);
	viewrect().setBorder(opengl_transform_.border());
	if (fitToWindow()) fitViewrect();

	const auto border = viewrect().border();
	const auto bvec = QPointF{
		0.5 * scale * border.width(),
		0.5 * scale * border.height()};
	const QPointF diff = old_vpos - fixed_point - QPointF{0.5, 0.5} + old_bvec;
	const QPointF new_vpos = QPointF{0.5, 0.5} + fixed_point - bvec + rot.map(diff);

	viewrect().setVirtualPos(new_vpos.toPoint());

	emit rotationChanged(rotation());
}

void OpenGLWidget::flipViewrect(Qt::Axis flip_axis) {
	QMatrix4x4 rot;
	rot.rotate(-2 * rotation(), 0, 0, 1);

	const auto fixed_point = rect().center();
	const auto scale = viewrect().scale();
	const auto old_vpos = viewrect().virtualPos();
	const auto border = viewrect().border();
	const auto bvec = QPointF{
		0.5 * scale * border.width(),
		0.5 * scale * border.height()};

	QPointF diff = old_vpos - fixed_point - QPointF{0.5, 0.5} + bvec;
	switch (flip_axis) {
		case Qt::XAxis:
			diff.setX(-diff.x());
			break;
		case Qt::YAxis:
			diff.setY(-diff.y());
			break;
		default:
			Q_ASSERT(false);
	}
	const QPointF new_vpos = QPointF{0.5, 0.5} + fixed_point - bvec + rot.map(diff);

	viewrect().setVirtualPos(new_vpos.toPoint());
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

void OpenGLWidget::setLayer(int layer) {
	const auto new_layer = std::min(layer, static_cast<int>(image_depth()) - 1);

	if (new_layer == layer_)
		return;

	layer_ = new_layer;
	emit layerChanged(layer_);
	update();
}
