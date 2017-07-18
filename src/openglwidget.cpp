#include <QFile>

#include <openglwidget.h>

void OpenGLWidget::Exception::raise() const {
	throw *this;
}
QException* OpenGLWidget::Exception::clone() const {
	return new OpenGLWidget::Exception(*this);
}
void OpenGLWidget::ShaderLoadingError::raise() const {
	throw *this;
}
QException* OpenGLWidget::ShaderLoadingError::clone() const {
	return new OpenGLWidget::ShaderLoadingError(*this);
}
void OpenGLWidget::ShaderBindingError::raise() const {
	throw *this;
}
QException* OpenGLWidget::ShaderBindingError::clone() const {
	return new OpenGLWidget::ShaderBindingError(*this);
}

OpenGLWidget::OpenGLWidget(QWidget *parent, FITS* fits):
	QOpenGLWidget(parent),
	fits_(fits),
	texture_deleter_(this),
	texture_(new QOpenGLTexture(QOpenGLTexture::Target2D), texture_deleter_),
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
	texture_->setFormat(QOpenGLTexture::R8_UNorm);
	texture_->setMipLevels(4);
	texture_->setSize(fits_->data_unit().width(), fits_->data_unit().height());
	texture_->allocateStorage(QOpenGLTexture::Red, QOpenGLTexture::UInt8);
	texture_->setData(QOpenGLTexture::Red, QOpenGLTexture::UInt8, fits_->data_unit().data());

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
	vshader->compileSourceCode(vsrc);

	QOpenGLShader *fshader = new QOpenGLShader(QOpenGLShader::Fragment, this);
	const char *fsrc =
			"#version 110\n"
			"varying vec2 UV;\n"
			"uniform sampler2D texture;\n"
			"void main(){\n"
			"	gl_FragColor = vec4(vec3(texture2D(texture, UV).r), 1);\n"
			"}\n";
	fshader->compileSourceCode(fsrc);

	if (! program_->addShader(vshader)) throw ShaderLoadingError();
	if (! program_->addShader(fshader)) throw ShaderLoadingError();
	program_->bindAttributeLocation("vertexCoord", program_vertex_coord_attribute);
	program_->bindAttributeLocation("vertexUV",    program_vertex_uv_attribute);
	if (! program_->link()) throw ShaderLoadingError();
	if (! program_->bind()) throw ShaderBindingError();
	program_->setUniformValue("texture", program_texture_uniform);
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
