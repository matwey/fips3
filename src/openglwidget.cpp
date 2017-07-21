#include <QFile>

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
	program_(new QOpenGLShaderProgram, program_deleter_)
	{
	resize(800, 448);
}

OpenGLWidget::~OpenGLWidget() {
	makeCurrent();

	vbo_.destroy();

	doneCurrent();
}

void OpenGLWidget::initializeGL() {
	initializeOpenGLFunctions();

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glDisable(GL_DEPTH_TEST);

	texture_->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
	texture_->setMagnificationFilter(QOpenGLTexture::Nearest);
	texture_->setFormat(QOpenGLTexture::R16_UNorm);
	qDebug() << glGetError();
	texture_->setMipLevels(4);
	texture_->setSize(fits_->data_unit().width(), fits_->data_unit().height());
	texture_->allocateStorage(QOpenGLTexture::Red, QOpenGLTexture::UInt16);
	qDebug() << glGetError();
	pixel_transfer_options_->setSwapBytesEnabled(true);
	texture_->setData(QOpenGLTexture::Red, QOpenGLTexture::UInt16, fits_->data_unit().data(), pixel_transfer_options_.get());
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
			"void main(){\n"
			"	gl_Position =  vec4(vertexCoord,1);\n"
			"	UV = VertexUV;\n"
			"}\n";
	if (! vshader->compileSourceCode(vsrc)) throw ShaderCompileError();

	QOpenGLShader *fshader = new QOpenGLShader(QOpenGLShader::Fragment, this);
	const char *fsrc =
			"#version 110\n"
			"varying vec2 UV;\n"
			"uniform sampler2D texture;\n"
			"uniform float bzero;\n"
			"uniform float bscale;\n"
			"void main(){\n"
			"	float raw_fits_value = texture2D(texture, UV).r;\n"
			"	bool sign_mask = raw_fits_value > 0.5;\n"
			"   float fits_value = raw_fits_value - float(sign_mask);\n"
			"	float physical_value = bscale * fits_value + bzero;\n"
			"	gl_FragColor = vec4(vec3(physical_value), 1);\n"
			"}\n";
	if (! fshader->compileSourceCode(fsrc)) throw ShaderCompileError();

	if (! program_->addShader(vshader)) throw ShaderLoadError();
	if (! program_->addShader(fshader)) throw ShaderLoadError();
	program_->bindAttributeLocation("vertexCoord", program_vertex_coord_attribute);
	program_->bindAttributeLocation("vertexUV",    program_vertex_uv_attribute);
	if (! program_->link()) throw ShaderLoadError();
	if (! program_->bind()) throw ShaderBindError();
	program_->setUniformValue("tex", texture_->textureId());
	// TODO: get bzero & bscale values from FITS header
	program_->setUniformValue("bzero", 0.5f);
	program_->setUniformValue("bscale", 1.0f);
}

void OpenGLWidget::resizeGL(int w, int h){
	w *= devicePixelRatio();
	h *= devicePixelRatio();

	glViewport(0, 0, w, h);
}

void OpenGLWidget::paintGL() {
	glClear(GL_COLOR_BUFFER_BIT);

	program_->enableAttributeArray(program_vertex_coord_attribute);
	program_->enableAttributeArray(program_vertex_uv_attribute);
	program_->setAttributeBuffer(program_vertex_coord_attribute, GL_FLOAT, 0,                   3, 5 * sizeof(GLfloat));
	program_->setAttributeBuffer(program_vertex_uv_attribute,    GL_FLOAT, 3 * sizeof(GLfloat), 2, 5 * sizeof(GLfloat));

	texture_->bind();

	glDrawArrays(GL_TRIANGLES, 0, 6);
}

constexpr GLfloat OpenGLWidget::vbo_data[];
