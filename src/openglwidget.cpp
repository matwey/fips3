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

#include <openglwidget.h>
#include <utils/swapbytes.h>

namespace {
struct HDUValueGetter {
	const FITS::HeaderDataUnit* hdu;
	const QPoint* image_position;
	double* value;

	template<class T> void operator() (const FITS::DataUnit<T>& data) const {
		using Utils::swap_bytes;
		const auto raw_value = static_cast<double>(swap_bytes(*(data.data() + image_position->y() * data.width() + image_position->x())));
		*value = raw_value * hdu->header().bscale() + hdu->header().bzero();
	}
	void operator() (const FITS::EmptyDataUnit&) const {
		Q_ASSERT(0);
	}
};
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

CoordinateSystemTransform::CoordinateSystemTransform(OpenGLWidget *parent):
	QObject(parent),
	parent_(parent) {}
const QMatrix4x4& CoordinateSystemTransform::matrix() {
	return matrix_;
}

RotationTransform::RotationTransform(OpenGLWidget *parent):
	CoordinateSystemTransform(parent) {}
void RotationTransform::setRotationAngle(double angle) {
	if (angle_ != angle) {
		QMatrix4x4 viewrect_rotation_matrix;
		viewrect_rotation_matrix.rotate(angle_-angle, 0, 0, 1); // Rotation in viewrect coordinates is clockwise
		auto &viewrect = parent_->viewrect();
		const auto new_view_center = viewrect_rotation_matrix.map(viewrect.view().center());
		auto new_view_rect = viewrect.view();
		new_view_rect.moveCenter(new_view_center);

		angle_ = angle;
		QMatrix4x4 m;
		m.rotate(angle_, 0, 0, 1);
		matrix_ = m;

		viewrect.setBorder(parent_->vertexCoords()->borderRect(angle_));
		viewrect.set(new_view_rect);

		emit rotationAngleChanged(angle_);
	}
}

OpenGLTransform::OpenGLTransform(OpenGLWidget *parent):
	CoordinateSystemTransform(parent) {
	connect(&parent_->viewrect(), SIGNAL(scrollRectChanged(const QRect&)), this, SLOT(scrollRectChanged()));
}
const QMatrix4x4& OpenGLTransform::matrix() {
	if (projection_to_be_changed_) {
		QMatrix4x4 m;
		m.ortho(parent_->viewrect().openGLprojection());
		projection_ = m;
		projection_to_be_changed_ = false;
		matrix_ = projection_* parent_->rotationTransform().matrix();
	}
	return CoordinateSystemTransform::matrix();
}

PixelTransform::PixelTransform(OpenGLWidget *parent):
	CoordinateSystemTransform(parent) {
	connect(&parent_->viewrect(), SIGNAL(scrollRectChanged(const QRect&)), this, SLOT(scrollRectChanged()));
	connect(parent_, SIGNAL(resized()), this, SLOT(widgetResized()));
	connect(&parent_->rotationTransform(), SIGNAL(rotationAngleChanged(double)), this, SLOT(angleChanged()));
	connect(parent_, SIGNAL(textureInitialized(const OpenGLTexture*)), this, SLOT(imageReloaded()));
}
const QMatrix4x4& PixelTransform::matrix() {
	if (widget_to_world_to_be_changed_ || world_to_model_to_be_changed_ || model_to_image_to_be_changed_) {
		const bool widget_to_model_to_be_changed = widget_to_world_to_be_changed_ || world_to_model_to_be_changed_;
		if (widget_to_world_to_be_changed_) {
			const auto view = parent_->viewrect().view();
			const auto widget = parent_->size();
			QMatrix4x4 m;
			auto data = m.data();
			data[0] = view.width() / (widget.width() - 1.0);
			data[5] = -view.height() / (widget.height() - 1.0);
			data[12] = view.left();
			data[13] = -view.top();
			widget_to_world_ = m;
			widget_to_world_to_be_changed_ = false;
		}
		if (world_to_model_to_be_changed_) {
			world_to_model_ = parent_->rotationTransform().matrix().transposed();
			world_to_model_to_be_changed_ = false;
		}
		if (widget_to_model_to_be_changed) {
			widget_to_model_ = world_to_model_ * widget_to_world_;
		}
		if (model_to_image_to_be_changed_) {
			const auto image = parent_->image_size();
			QMatrix4x4 m;
			auto data = m.data();
			data[0] = 0.5 * image.width()  * parent_->vertexCoords()->factor() / (image.width()  - 1.0);
			data[5] = 0.5 * image.height() * parent_->vertexCoords()->factor() / (image.height() - 1.0);
			data[12] = 0.5 * (image.width() - 1.0);
			data[13] = 0.5 * (image.height() - 1.0);
			model_to_image_ = m;
			model_to_image_to_be_changed_ = false;
		}
		matrix_ = model_to_image_ * widget_to_model_;
	}
	return CoordinateSystemTransform::matrix();
}

OpenGLWidget::VertexCoordinates::VertexCoordinates(const QSize &image_size, GLfloat factor):
	image_size_(image_size),
	factor_(factor) {
	const auto semi_width  = (image_size_.width()  - 1.0f) / factor;
	const auto semi_height = (image_size_.height() - 1.0f) / factor;
	// Center of the rectangle should be at (0,0)
	data_ = {
			-semi_width, -semi_height,
			-semi_width,  semi_height,
			 semi_width,  semi_height,
			 semi_width, -semi_height,
	};
}
OpenGLWidget::VertexCoordinates::VertexCoordinates(const QSize &image_size):
	VertexCoordinates(image_size, image_size.width() - 1) {}
QRectF OpenGLWidget::VertexCoordinates::borderRect(GLfloat angle) const {
	QMatrix4x4 rotation_matrix;
	// Rotation in viewrect coordinates is clockwise, but it doesn't matter in
	// the case of rectangle with the center in (0,0)
	rotation_matrix.rotate(-angle, 0, 0, 1);
	// Arguments are top left and bottom right corners in viewrect coordinates:
	QRectF non_rotated_image(QPointF(data_[2], -data_[3]), QPointF(data_[6], -data_[7]));
	return rotation_matrix.mapRect(non_rotated_image);
}

OpenGLWidget::OpenGLWidget(QWidget *parent, const FITS::HeaderDataUnit& hdu):
	QOpenGLWidget(parent),
	hdu_(&hdu),
	rotation_transform_(this),
	open_gl_transform_(this),
	pixel_transform_(this),
	shader_uniforms_(new OpenGLShaderUniforms(1, 1, 0, 1)),
	colormaps_{{
		openGL_unique_ptr<OpenGLColorMap>(new GrayscaleColorMap() , colormap_deleter_type(this)),
		openGL_unique_ptr<OpenGLColorMap>(new PurpleBlueColorMap(), colormap_deleter_type(this)),
	}},
	colormap_index_(0) {

	connect(&viewrect_, SIGNAL(scrollRectChanged(const QRect&)), this, SLOT(update()));
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
	std::unique_ptr<VertexCoordinates> new_vertex_coords{new VertexCoordinates(image_size())};

	struct ShaderLoader {
		QString* fragment_shader_source_main_;

		void operator() (const FITS::DataUnit<quint8>&) const {
			*fragment_shader_source_main_ =
					"uniform float c;\n"
					"uniform float z;\n"
					"void main() {\n"
					"	float value = c * (texture2D(texture, UV).a - z);\n";
		}
		void operator() (const FITS::DataUnit<qint16>&) const {
			*fragment_shader_source_main_ =
					"uniform vec2 c;\n"
					"uniform vec2 z;\n"
					"void main() {\n"
					"	vec2 raw_value = texture2D(texture, UV).ga;\n"
					"   raw_value.x -= float(raw_value.x > 0.5) * 1.003921568627451;  // 256.0 / 255.0\n"
					"	float value = dot(c, raw_value - z);\n";
		}
		void operator() (const FITS::DataUnit<qint32>&) const {
			*fragment_shader_source_main_ =
					"uniform vec4 c;\n"
					"uniform vec4 z;\n"
					"void main() {\n"
					"	vec4 raw_value = texture2D(texture, UV);\n"
					"   raw_value.x -= float(raw_value.x > 0.5) * 1.003921568627451;  // 256.0 / 255.0\n"
					"	float value = dot(c, raw_value - z);\n";
		}
		void operator() (const FITS::DataUnit<qint64>&) const {
			*fragment_shader_source_main_ =
					"uniform vec4 c;\n"
					"uniform vec4 z;\n"
					"void main() {\n"
					"	vec4 raw_value = texture2D(texture, UV);\n"
					"   raw_value.x -= float(raw_value.x > 0.5) * 1.0000152590218967;  // 65536.0 / 65535.0\n"
					"	float value = dot(c, raw_value - z);\n";
		}
		void operator() (const FITS::DataUnit<float>&) const {
			// TODO: Check GL_ARB_color_buffer_float, GL_OES_texture_float.
			if (! QOpenGLContext::currentContext()->hasExtension("GL_ARB_texture_float")) {
				// TODO: recode data from float into (u)int32
				qDebug() << "BITPIX==-32 is not implemented for this hardware";
			} else {
				*fragment_shader_source_main_ =
						"uniform float c;\n"
						"uniform float z;\n"
						"void main() {\n"
						"	float value = c * (texture2D(texture, UV).a - z);\n";
			}
		}
		void operator() (const FITS::DataUnit<double>&) const {
			qDebug() << "BITPIX==-64 is not implemented";
		}
		void operator() (const FITS::EmptyDataUnit&) const {
			Q_ASSERT(0);
		}
	};

	QString fragment_shader_source_main;
	hdu_->data().apply(ShaderLoader{&fragment_shader_source_main});

	QOpenGLShader *vshader = new QOpenGLShader(QOpenGLShader::Vertex, this);
	const char *vsrc =
			"attribute vec2 vertexCoord;\n"
			"attribute vec2 VertexUV;\n"
			"varying vec2 UV;\n"
			"uniform mat4 MVP;\n"
			"void main() {\n"
			"	gl_Position = MVP * vec4(vertexCoord,0,1);\n"
			"	UV = VertexUV;\n"
			"}\n";
	if (! vshader->compileSourceCode(vsrc)) throw ShaderCompileError(glGetError());

	QString fsrc =
			"#ifdef GL_ES\n"
			"	#ifdef GL_FRAGMENT_PRECISION_HIGH\n"
			"		precision highp float;\n"
			"		precision highp sampler2D;\n"
			"	#else\n"
			"		precision mediump float;\n"
			"		precision mediump sampler2D;\n"
			"	#endif\n"
			"#endif\n"
			"varying vec2 UV;\n"
			"uniform sampler2D texture;\n"
			"uniform sampler1D colormap;\n"
			+ fragment_shader_source_main +
			"	gl_FragColor = texture1D(colormap, clamp(value, 0.0, 1.0));\n"
			"}\n";
	QOpenGLShader *fshader = new QOpenGLShader(QOpenGLShader::Fragment, this);
	if (! fshader->compileSourceCode(fsrc)) throw ShaderCompileError(glGetError());

	openGL_unique_ptr<QOpenGLShaderProgram> new_program(new QOpenGLShaderProgram(this), OpenGLDeleter<QOpenGLShaderProgram>(this));
	if (! new_program->addShader(vshader)) throw ShaderLoadError(glGetError());
	if (! new_program->addShader(fshader)) throw ShaderLoadError(glGetError());;
	new_program->bindAttributeLocation("vertexCoord", program_vertex_coord_attribute_);
	new_program->bindAttributeLocation("vertexUV",    program_vertex_uv_attribute_);
	if (! new_program->link()) throw ShaderLoadError(glGetError());
	if (! new_program->bind()) throw ShaderBindError(glGetError());
	new_program->enableAttributeArray(program_vertex_coord_attribute_);
	new_program->enableAttributeArray(program_vertex_uv_attribute_);
	new_program->setAttributeArray(program_vertex_coord_attribute_, new_vertex_coords->data(), 2);
	new_program->setAttributeArray(program_vertex_uv_attribute_, uv_data, 2);
	new_program->setUniformValue("texture",  program_texture_uniform_);
	new_program->setUniformValue("colormap", program_colormap_uniform_);

	openGL_unique_ptr<OpenGLTexture> new_texture(new OpenGLTexture(), OpenGLDeleter<OpenGLTexture>(this));
	new_texture->initialize(hdu_);

	// If no exceptions were thrown then we can put new objects to object's member pointers
	vertex_coords_ = std::move(new_vertex_coords);
	viewrect_.setBorder(vertex_coords_->borderRect(angle()));

	if (program_) program_->release();
	program_ = std::move(new_program);
	program_->bind();

	if (texture_ && texture_->isBound(program_texture_uniform_)) texture_->release(program_texture_uniform_);
	texture_ = std::move(new_texture);
	texture_->bind(program_texture_uniform_);

	emit textureInitialized(texture_.get());
	shader_uniforms_.reset(new OpenGLShaderUniforms(texture_->channels(), texture_->channel_size(), hdu_->header().bzero(), hdu_->header().bscale()));
	shader_uniforms_->setMinMax(texture_->hdu_minmax());
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
		viewrect_.set(new_view);
	}
}

void OpenGLWidget::paintGL() {
	glClear(GL_COLOR_BUFFER_BIT);

	program_->bind();

	program_->setAttributeArray(program_vertex_coord_attribute_, vertex_coords_->data(), 2);

	program_->setUniformValue("MVP", open_gl_transform_.matrix());

	program_->setUniformValueArray("c", shader_uniforms_->get_c().data(), 1, shader_uniforms_->channels);
	program_->setUniformValueArray("z", shader_uniforms_->get_z().data(), 1, shader_uniforms_->channels);

	texture_->bind(program_texture_uniform_);
	colormaps_[colormap_index_]->bind(program_colormap_uniform_);

	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

void OpenGLWidget::setHDU(const FITS::HeaderDataUnit &hdu) {
	const auto old_image_size = image_size();
	const auto old_hdu = hdu_;
	hdu_ = &hdu;
	try {
		initializeGLObjects();
		if (image_size() != old_image_size) {
			fitViewrect();
		} else {
			update();
		}
	} catch (const std::exception& e) {
		hdu_ = old_hdu;
		throw e;
	}
}

void OpenGLWidget::changeLevels(const std::pair<double, double>& minmax) {
	shader_uniforms_->setMinMax(minmax);
	update();
}

void OpenGLWidget::changeColorMap(int colormap_index) {
	Q_ASSERT(colormap_index >= 0 && colormap_index < colormaps_.size());
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
	const auto position = pixel_transform_.matrix().map(QPointF(widget_coord)).toPoint();

	const bool inside_image = QRect({0, 0}, image_size()).contains(position);
	if (inside_image) {
		double value;
		hdu_->data().apply(HDUValueGetter{hdu_, &position, &value});
		return Pixel(position, value);
	}
	return Pixel(position);
}

constexpr const GLfloat OpenGLWidget::uv_data[];
