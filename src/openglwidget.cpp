#include <QFile>

#include <openglwidget.h>

OpenGLWidget::OpenGLWidget(QWidget *parent, FITS* fits):
	QOpenGLWidget(parent),
	fits_(fits),
//	texture_deleter(this),
	texture_(new QOpenGLTexture(QOpenGLTexture::Target2D)/*, texture_deleter*/),
	program_(new QOpenGLShaderProgram)
	{
	resize(800, 448);
}

OpenGLWidget::~OpenGLWidget() {
	makeCurrent();

	texture_.reset(nullptr);
	program_.reset(nullptr);
	vbo_.destroy();

	doneCurrent();
}

void OpenGLWidget::initializeGL() {
	initializeOpenGLFunctions();

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glDisable(GL_DEPTH_TEST);

	texture_->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
	texture_->setMagnificationFilter(QOpenGLTexture::Nearest);
	texture_->setFormat(QOpenGLTexture::LuminanceFormat);
	texture_->setMipLevels(4);
	texture_->setSize(fits_->data_unit().width(), fits_->data_unit().height());
	texture_->allocateStorage(QOpenGLTexture::Luminance, QOpenGLTexture::UInt8);
	texture_->setData(QOpenGLTexture::Luminance, QOpenGLTexture::UInt8, fits_->data_unit().data());

	vbo_.create();
	vbo_.bind();
	vbo_.allocate(vbo_data, sizeof(vbo_data));

	QOpenGLShader *vshader = new QOpenGLShader(QOpenGLShader::Vertex, this);
	const char *vsrc =
			"attribute highp vec2 VertexUV;\n"
			"attribute highp vec3 vertexCoord;\n"
			"varying highp vec2 UV;\n"
			"void main(){\n"
			"	gl_Position =  vec4(vertexCoord,1);\n"
			"	UV = VertexUV;\n"
			"}\n";
	vshader->compileSourceCode(vsrc);

	QOpenGLShader *fshader = new QOpenGLShader(QOpenGLShader::Fragment, this);
	const char *fsrc =
			"varying highp vec2 UV;\n"
			"uniform highp sampler2D texture;\n"
			"void main(){\n"
			"	gl_FragColor = texture2D(texture, UV);\n"
			"}\n";
	fshader->compileSourceCode(fsrc);

	program_->addShader(vshader);
	program_->addShader(fshader);
	program_->bindAttributeLocation("vertexCoord", program_vertex_coord_attribute);
	program_->bindAttributeLocation("vertexUV",    program_vertex_uv_attribute);
	program_->link();
	program_->bind();
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
