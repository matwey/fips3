#include <QFile>

#include <limits>

#include <openglwidget.h>

void OpenGLWidget::Exception::raise() const {
	throw *this;
}
QException* OpenGLWidget::Exception::clone() const {
	return new OpenGLWidget::Exception(*this);
}
void OpenGLWidget::ShaderLoadError::raise() const {
	throw *this;
}
QException* OpenGLWidget::ShaderLoadError::clone() const {
	return new OpenGLWidget::ShaderLoadError(*this);
}
void OpenGLWidget::ShaderBindError::raise() const {
	throw *this;
}
QException* OpenGLWidget::ShaderBindError::clone() const {
	return new OpenGLWidget::ShaderBindError(*this);
}
void OpenGLWidget::ShaderCompileError::raise() const {
	throw *this;
}
QException* OpenGLWidget::ShaderCompileError::clone() const {
	return new OpenGLWidget::ShaderCompileError(*this);
}

OpenGLWidget::OpenGLWidget(QWidget *parent, FITS* fits):
	QOpenGLWidget(parent),
	fits_(fits),
	texture_deleter_(this),
	texture_(new QOpenGLTexture(QOpenGLTexture::Target2D), texture_deleter_),
	pixel_transfer_options_deleter_(this),
	pixel_transfer_options_(new QOpenGLPixelTransferOptions, pixel_transfer_options_deleter_),
	program_deleter_(this),
	program_(new QOpenGLShaderProgram, program_deleter_) {}

OpenGLWidget::~OpenGLWidget() {
	makeCurrent();

	vbo_.destroy();

	doneCurrent();
}

void OpenGLWidget::initializeGL() {
	initializeOpenGLFunctions();

	struct texture_loader {
		OpenGLWidget* openGL_widget;
		GLfloat* normalizer;
		QOpenGLTexture::TextureFormat* texture_format;
		QOpenGLTexture::PixelFormat* pixel_format;
		QOpenGLTexture::PixelType *pixel_type;
		bool* swap_bytes_enabled;
		QString* fragment_shader_source_main_;

		void operator() (const FITS::DataUnit<quint8>&) const {
			*normalizer = static_cast<GLfloat>(std::numeric_limits<quint8>::max());
			*texture_format = QOpenGLTexture::AlphaFormat;
			*pixel_format = QOpenGLTexture::Alpha;
			*pixel_type = QOpenGLTexture::UInt8;
			*swap_bytes_enabled = false;
			*fragment_shader_source_main_ =
					"	float fits_value = texture2D(texture, UV).a;\n"
					"	float physical_value = bscale * fits_value + bzero;\n"
					"	gl_FragColor = vec4(vec3(physical_value), 1);\n";
		}
		void operator() (const FITS::DataUnit<qint16>&) const {
			*normalizer = static_cast<GLfloat>(std::numeric_limits<qint16>::max()) - static_cast<GLfloat>(std::numeric_limits<qint16>::min());
			*texture_format = QOpenGLTexture::LuminanceAlphaFormat;
			*pixel_format = QOpenGLTexture::LuminanceAlpha;
			*pixel_type = QOpenGLTexture::UInt8;
			*swap_bytes_enabled = false;
			*fragment_shader_source_main_ =
					"	vec4 raw_color = texture2D(texture, UV);\n"
					"	float raw_fits_value = (raw_color.a + raw_color.r * 256.0) / 257.0;\n"
					"	bool sign_mask = raw_fits_value > 0.5;\n"
					"   float fits_value = raw_fits_value - float(sign_mask);\n"
					"	float physical_value = bscale * fits_value + bzero;\n"
					"	gl_FragColor = vec4(vec3(physical_value), 1);\n";
		}
		void operator() (const FITS::DataUnit<qint32>&) const {
			*normalizer = static_cast<GLfloat>(std::numeric_limits<qint32>::max()) - static_cast<GLfloat>(std::numeric_limits<qint32>::min());
			*texture_format = QOpenGLTexture::RGBAFormat;
			*pixel_format = QOpenGLTexture::RGBA;
			*pixel_type = QOpenGLTexture::UInt8;
			*swap_bytes_enabled = false;
			*fragment_shader_source_main_ =
					"	vec4 raw_color = texture2D(texture, UV);\n"
					"	float raw_fits_value = (raw_color.a + raw_color.b * 256.0 + raw_color.g * 65536.0 + raw_color.r * 4294967296.0) / 4295033089.0;\n"
					"	bool sign_mask = raw_fits_value > 0.5;\n"
					"   float fits_value = raw_fits_value - float(sign_mask);\n"
					"	float physical_value = bscale * fits_value + bzero;\n"
					"	gl_FragColor = vec4(vec3(physical_value), 1);\n";
		}
		void operator() (const FITS::DataUnit<qint64>&) const {
			*normalizer = static_cast<GLfloat>(std::numeric_limits<qint64>::max()) - static_cast<GLfloat>(std::numeric_limits<qint64>::min());
			*texture_format = QOpenGLTexture::RGBA16_UNorm;
			*pixel_format = QOpenGLTexture::RGBA;
			*pixel_type = QOpenGLTexture::UInt16;
			*swap_bytes_enabled = true;
			*fragment_shader_source_main_ =
					"	vec4 raw_color = texture2D(texture, UV);\n"
					"	float raw_fits_value = (raw_color.a + raw_color.b * 65536.0 + raw_color.g * 4294967296.0 + raw_color.r * 18446744073709551616.0) / 18446744078004584449.0;\n"
					"	bool sign_mask = raw_fits_value > 0.5;\n"
					"   float fits_value = raw_fits_value - float(sign_mask);\n"
					"	float physical_value = bscale * fits_value + bzero;\n"
					"	gl_FragColor = vec4(vec3(physical_value), 1);\n";
		}
		void operator() (const FITS::DataUnit<float>&) const {
			// TODO: Check GL_ARB_color_buffer_float, GL_OES_texture_float.
			if (! openGL_widget->context()->hasExtension("GL_ARB_texture_float")) {
				// TODO: recode data from float into (u)int32
				qDebug() << "BITPIX==-32 is not implemented for this hardware";
			} else {
				// Constant from GL_ARB_texture_float extension documentation:
				// https://www.khronos.org/registry/OpenGL/extensions/ARB/ARB_texture_float.txt
				*normalizer = 1.0f;
				static const quint64 alpha32f_arb = 0x8816;
				*texture_format = static_cast<QOpenGLTexture::TextureFormat>(alpha32f_arb);
				*pixel_format = QOpenGLTexture::Alpha;
				*pixel_type = QOpenGLTexture::Float32;
				*swap_bytes_enabled = true;
				*fragment_shader_source_main_ =
						"	float fits_value = texture2D(texture, UV).a;\n"
						"	float physical_value = bscale * fits_value + bzero;\n"
						"	gl_FragColor = vec4(vec3(physical_value), 1);\n";
			}
		}
		void operator() (const FITS::DataUnit<double>&) const {
			qDebug() << "BITPIX==-64 is not implemented";
		}
	};

	GLfloat normalizer;
	QOpenGLTexture::TextureFormat texture_format;
	QOpenGLTexture::PixelFormat pixel_format;
	QOpenGLTexture::PixelType pixel_type;
	bool swap_bytes_enabled;
	QString fragment_shader_source_main;
	fits_->data_unit().apply(texture_loader{this,
											&normalizer,
											&texture_format,
											&pixel_format,
											&pixel_type,
											&swap_bytes_enabled,
											&fragment_shader_source_main});


	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glDisable(GL_DEPTH_TEST);

	texture_->setMinificationFilter(QOpenGLTexture::Nearest);
	texture_->setMagnificationFilter(QOpenGLTexture::Nearest);
	texture_->setFormat(texture_format);
	qDebug() << glGetError();
	texture_->setSize(fits_->data_unit().width(), fits_->data_unit().height());
	qDebug() << glGetError();
	// We use this overloading to provide a possibility to use texture internal format unsupported by QT
	texture_->allocateStorage(pixel_format, pixel_type);
	qDebug() << glGetError();
	pixel_transfer_options_->setSwapBytesEnabled(swap_bytes_enabled);
	texture_->setData(pixel_format, pixel_type, fits_->data_unit().data(), pixel_transfer_options_.get());
	qDebug() << glGetError();

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
			"void main(){\n"
			"	gl_Position = MVP * vec4(vertexCoord,1);\n"
			"	UV = VertexUV;\n"
			"}\n";
	if (! vshader->compileSourceCode(vsrc)) throw ShaderCompileError();

	QString fsrc =
			"#version 110\n"
			"varying vec2 UV;\n"
			"uniform sampler2D texture;\n"
			"uniform float bzero;\n"
			"uniform float bscale;\n"
			"void main(){\n"
			+ fragment_shader_source_main +
			"}\n";
	QOpenGLShader *fshader = new QOpenGLShader(QOpenGLShader::Fragment, this);
	if (! fshader->compileSourceCode(fsrc)) throw ShaderCompileError();

	if (! program_->addShader(vshader)) throw ShaderLoadError();
	if (! program_->addShader(fshader)) throw ShaderLoadError();
	program_->bindAttributeLocation("vertexCoord", program_vertex_coord_attribute);
	program_->bindAttributeLocation("vertexUV",    program_vertex_uv_attribute);
	if (! program_->link()) throw ShaderLoadError();
	if (! program_->bind()) throw ShaderBindError();
	program_->setUniformValue("bzero",  static_cast<GLfloat>(fits_->header_unit().bzero()) / normalizer);
	program_->setUniformValue("bscale", static_cast<GLfloat>(fits_->header_unit().bscale()));
}

void OpenGLWidget::resizeGL(int w, int h) {
}

void OpenGLWidget::paintGL() {
	glClear(GL_COLOR_BUFFER_BIT);

	program_->enableAttributeArray(program_vertex_coord_attribute);
	program_->enableAttributeArray(program_vertex_uv_attribute);
	program_->setAttributeBuffer(program_vertex_coord_attribute, GL_FLOAT, 0,                   3, 5 * sizeof(GLfloat));
	program_->setAttributeBuffer(program_vertex_uv_attribute,    GL_FLOAT, 3 * sizeof(GLfloat), 2, 5 * sizeof(GLfloat));

	QMatrix4x4 mvp(base_mvp_);
	mvp.ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
	program_->setUniformValue("MVP", mvp);

	texture_->bind();

	glDrawArrays(GL_TRIANGLES, 0, 6);
}

constexpr GLfloat OpenGLWidget::vbo_data[];
