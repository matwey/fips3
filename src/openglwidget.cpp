#include <QFile>
#include <QPoint>

#include <openglwidget.h>

OpenGLWidget::Exception::Exception(const QString &what, GLenum gl_error_code):
	::Exception(what + ": " + glErrorString(gl_error_code)) {
}
void OpenGLWidget::Exception::raise() const {
	throw *this;
}
QException* OpenGLWidget::Exception::clone() const {
	return new OpenGLWidget::Exception(*this);
}
// From OpenGL ES 2.0 documentation:
// https://www.khronos.org/registry/OpenGL-Refpages/es2.0/xhtml/glGetError.xml
QString OpenGLWidget::Exception::glErrorString(GLenum gl_error_code) {
	switch (gl_error_code) {
//		case GL_NO_ERROR:
//			return "No error has been recorded. The value of this symbolic constant is guaranteed to be 0.";
		case GL_INVALID_ENUM:
			return "An unacceptable value is specified for an enumerated argument. "
					"The offending command is ignored and has no other side effect than to set the error flag.";
		case GL_INVALID_VALUE:
			return "A numeric argument is out of range. "
					"The offending command is ignored and has no other side effect than to set the error flag.";
		case GL_INVALID_OPERATION:
			return "The specified operation is not allowed in the current state. "
					"The offending command is ignored and has no other side effect than to set the error flag.";
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			return "The command is trying to render to or read from the framebuffer while the currently bound framebuffer is not framebuffer complete (i.e. the return value from glCheckFramebufferStatus is not GL_FRAMEBUFFER_COMPLETE). "
					"The offending command is ignored and has no other side effect than to set the error flag.";
		case GL_OUT_OF_MEMORY:
			return "There is not enough memory left to execute the command. "
					"The state of the GL is undefined, except for the state of the error flags, after this error is recorded.";
		default:
			return "Unknown error";
	}
}
OpenGLWidget::ShaderLoadError::ShaderLoadError(GLenum gl_error_code):
	OpenGLWidget::Exception("Cannot load the shader", gl_error_code) {
}
void OpenGLWidget::ShaderLoadError::raise() const {
	throw *this;
}
QException* OpenGLWidget::ShaderLoadError::clone() const {
	return new OpenGLWidget::ShaderLoadError(*this);
}
OpenGLWidget::ShaderBindError::ShaderBindError(GLenum gl_error_code):
	OpenGLWidget::Exception("Cannot bind the shader", gl_error_code) {
}
void OpenGLWidget::ShaderBindError::raise() const {
	throw *this;
}
QException* OpenGLWidget::ShaderBindError::clone() const {
	return new OpenGLWidget::ShaderBindError(*this);
}
OpenGLWidget::ShaderCompileError::ShaderCompileError(GLenum gl_error_code):
	OpenGLWidget::Exception("Cannot compile the shader", gl_error_code) {
}
void OpenGLWidget::ShaderCompileError::raise() const {
	throw *this;
}
QException* OpenGLWidget::ShaderCompileError::clone() const {
	return new OpenGLWidget::ShaderCompileError(*this);
}
OpenGLWidget::TextureCreateError::TextureCreateError(GLenum gl_error_code):
		OpenGLWidget::Exception("Cannot create texture", gl_error_code) {
}
void OpenGLWidget::TextureCreateError::raise() const {
	throw *this;
}
QException* OpenGLWidget::TextureCreateError::clone() const {
	return new OpenGLWidget::TextureCreateError(*this);
}

OpenGLWidget::OpenGLWidget(QWidget *parent, const FITS::HeaderDataUnit& hdu):
	QOpenGLWidget(parent),
	hdu_(&hdu),
	texture_deleter_(this),
	texture_(new OpenGLTexture(hdu_), texture_deleter_),
	pixel_transfer_options_deleter_(this),
	pixel_transfer_options_(new QOpenGLPixelTransferOptions, pixel_transfer_options_deleter_),
	program_deleter_(this),
	program_(new QOpenGLShaderProgram, program_deleter_),
	viewrect_(0, 0, 1, 1),
	pixel_viewrect_(QPoint(0, 0), image_size()),
	shader_uniforms_(new OpenGLShaderUniforms(1, 1, hdu_->header().bzero(), hdu_->header().bscale())),
	palette_(PaletteWidget::GrayPalette) {
}

OpenGLWidget::~OpenGLWidget() {
	makeCurrent();

	vbo_.destroy();

	doneCurrent();
}

void OpenGLWidget::initializeGL() {
	initializeOpenGLFunctions();

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

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glDisable(GL_DEPTH_TEST);

	texture_->initialize();
	emit textureInitialized(texture_.get());
	shader_uniforms_.reset(new OpenGLShaderUniforms(texture_->channels(), texture_->channel_size(), hdu_->header().bzero(), hdu_->header().bscale()));
	shader_uniforms_->setMinMax(texture_->hdu_minmax());

	vbo_.create();
	vbo_.bind();
	vbo_.allocate(vbo_data, sizeof(vbo_data));

	QOpenGLShader *vshader = new QOpenGLShader(QOpenGLShader::Vertex, this);
	const char *vsrc =
			"#version 110\n"
			"attribute vec2 VertexUV;\n"
			"attribute vec3 vertexCoord;\n"
			"varying vec2 UV;\n"
			"uniform mat4 MVP;\n"
			"void main() {\n"
			"	gl_Position = MVP * vec4(vertexCoord,1);\n"
			"	UV = VertexUV;\n"
			"}\n";
	if (! vshader->compileSourceCode(vsrc)) throw ShaderCompileError(glGetError());

	QString fsrc =
			"#version 110\n"
			"varying vec2 UV;\n"
			"uniform sampler2D texture;\n"
			"uniform int palette;\n"
			"vec3 col_gray(in float x){\n"
			"	return clamp(vec3(x), 0.0, 1.0);\n"
			"}\n"
			"vec3 col_half_hsl_circle(in float x){\n"
			"	vec3 color = clamp(vec3(2.0-3.0*x, 3.0*x, -2.0+3.0*x), 0.0, 1.0);\n"
			"	float brightness = 3.0 * x;\n"
			"	return clamp(brightness * color, 0.0, 1.0);\n"
			"}\n"
			"vec3 col_purple_blue(in float x){\n"
			"	return clamp(vec3(1.0-2.0*x, -1.0+2.0*x, 1.0), 0.0, 1.0);\n"
			"}\n"
			+ fragment_shader_source_main +
			"	if (palette == 0){"
			"		gl_FragColor = vec4(col_gray(value), 1);\n"
			"		return;\n"
			"	}\n"
			"	if (palette == 1){"
			"		gl_FragColor = vec4(col_half_hsl_circle(value), 1);\n"
			"		return;\n"
			"	}\n"
			"	if (palette == 2){"
			"		gl_FragColor = vec4(col_purple_blue(value), 1);\n"
			"		return;\n"
			"	}\n"
			"}\n";
	QOpenGLShader *fshader = new QOpenGLShader(QOpenGLShader::Fragment, this);
	if (! fshader->compileSourceCode(fsrc)) throw ShaderCompileError(glGetError());

	if (! program_->addShader(vshader)) throw ShaderLoadError(glGetError());
	if (! program_->addShader(fshader)) throw ShaderLoadError(glGetError());
	program_->bindAttributeLocation("vertexCoord", program_vertex_coord_attribute);
	program_->bindAttributeLocation("vertexUV",    program_vertex_uv_attribute);
	if (! program_->link()) throw ShaderLoadError(glGetError());
	if (! program_->bind()) throw ShaderBindError(glGetError());
	program_->enableAttributeArray(program_vertex_coord_attribute);
	program_->enableAttributeArray(program_vertex_uv_attribute);
	program_->setAttributeBuffer(program_vertex_coord_attribute, GL_FLOAT, 0, 3, 3 * sizeof(GLfloat));
	program_->setAttributeBuffer(program_vertex_uv_attribute,    GL_FLOAT, 0, 2, 3 * sizeof(GLfloat));
}

void OpenGLWidget::resizeEvent(QResizeEvent* event) {
	QOpenGLWidget::resizeEvent(event);

	const auto new_widget_size = event->size();
	auto old_widget_size = event->oldSize();
	// event->oldSize() for the first call of resizeEvent equals -1,-1
	if (old_widget_size.width() < 0 || old_widget_size.height() < 0) {
		const auto ratio = (image_size().height() - 1.0) * (new_widget_size.width() - 1.0) / (image_size().width() - 1.0) / (new_widget_size.height() - 1.0);
		QSizeF new_size(ratio, 1);
		QRectF new_viewrect(QPointF(0, 0), new_size);
		setViewrect(new_viewrect);
	} else {
		const auto new_viewrect_width = viewrect_.width() * (static_cast<double>(new_widget_size.width()) - 1.0) / (static_cast<double>(old_widget_size.width()) - 1.0);
		const auto new_viewrect_height = viewrect_.height() * (new_widget_size.height() - 1.0) / (old_widget_size.height() - 1.0);
		QRectF new_viewrect(viewrect_);
		new_viewrect.setSize({new_viewrect_width, new_viewrect_height});
		setViewrect(new_viewrect);
	}
}

void OpenGLWidget::changeLevels(const std::pair<double, double>& minmax) {
	shader_uniforms_->setMinMax(minmax);
	update();
}

void OpenGLWidget::changePalette(int palette) {
	palette_ = palette;
	update();
}

void OpenGLWidget::paintGL() {
	glClear(GL_COLOR_BUFFER_BIT);

	QMatrix4x4 mvp(base_mvp_);
	// QT and OpenGL have different coordinate systems, we should change y-axes direction
	mvp.ortho(viewrect_.left(), viewrect_.right(), 1 - viewrect_.bottom(), 1 - viewrect_.top(), -1.0f, 1.0f);
	program_->setUniformValue("MVP", mvp);

	program_->setUniformValue("palette", static_cast<GLint>(palette_));

	program_->setUniformValueArray("c", shader_uniforms_->get_c(), 1, shader_uniforms_->channels);
	program_->setUniformValueArray("z", shader_uniforms_->get_z(), 1, shader_uniforms_->channels);

	texture_->bind();

	glDrawArrays(GL_TRIANGLES, 0, 6);
}

QSize OpenGLWidget::sizeHint() const {
	return image_size();
}

void OpenGLWidget::setViewrect(const QRectF &viewrect) {
	viewrect_ = viewrect;
	correct_viewrect();
	const QRect old_pixel_viewrect(pixel_viewrect_);
	pixel_viewrect_ = viewrectToPixelViewrect(viewrect_);
	if (pixel_viewrect_ != old_pixel_viewrect) {
		update();
		emit pixelViewrectChanged(pixel_viewrect_);
	}
}

QRect OpenGLWidget::viewrectToPixelViewrect(const QRectF& viewrect) const {
	const int left =   qRound(viewrect.left()   * (image_size().width()  - 1));
	const int top  =   qRound(viewrect.top()    * (image_size().height() - 1));
	const int width =  qRound(viewrect.width()  * image_size().width());
	const int height = qRound(viewrect.height() * image_size().height());
	return {left, top, width, height};
}

void OpenGLWidget::setPixelViewrect(const QRect& pixel_viewrect) {
	const auto left   = static_cast<double>(pixel_viewrect.left()  ) / (image_size().width()  - 1);
	const auto top    = static_cast<double>(pixel_viewrect.top()   ) / (image_size().height() - 1);
	const auto width  = static_cast<double>(pixel_viewrect.width() ) / image_size().width();
	const auto height = static_cast<double>(pixel_viewrect.height()) / image_size().height();
	setViewrect({left, top, width, height});
}

bool OpenGLWidget::correct_viewrect() {
	auto viewrect = viewrect_;
	if (viewrect.size().width() > 1) {
		viewrect.moveCenter({0.5, viewrect.center().y()});
	} else {
		if (viewrect.left() < 0) {
			viewrect.moveLeft(0);
		}
		if (viewrect.right() > 1) {
			viewrect.moveRight(1);
		}
	}
	if (viewrect.size().height() > 1) {
		viewrect.moveCenter({viewrect.center().x(), 0.5});
	} else {
		if (viewrect.top() < 0 ) {
			viewrect.moveTop(0);
		}
		if (viewrect.bottom() > 1) {
			viewrect.moveBottom(1);
		}
	}
	if ( viewrect != viewrect_ ){
		viewrect_ = viewrect;
		return true;
	}
	return false;
}

constexpr GLfloat OpenGLWidget::vbo_data[];
