#include <QFile>

#include <openglwidget.h>

OpenGLWidget::OpenGLWidget(QWidget *parent, FITS* fits):
	QOpenGLWidget(parent),
	fits_(fits) {

	resize(800, 448);
	img_ = new QImage(fits_->data_unit().data(), fits_->data_unit().width(), fits_->data_unit().height(), /*sizeof(*(fits_->data_unit().data())) * fits_->data_unit().width(),*/ QImage::Format_Grayscale8);
}

OpenGLWidget::~OpenGLWidget() {
	makeCurrent();

	delete img_;
	delete texture_;
	delete program_;
	vbo_.destroy();

	doneCurrent();
}

void OpenGLWidget::initializeGL() {
	initializeOpenGLFunctions();

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glDisable(GL_DEPTH_TEST);

	texture_ = new QOpenGLTexture(*img_);
//	glGenTextures(1, &textureID);
//	glBindTexture(GL_TEXTURE_2D, textureID);
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, fits_->data_unit().width(), fits_->data_unit().height(), 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, fits_->data_unit().data());
//	glGenerateMipmap(GL_TEXTURE_2D);  //Generate mipmaps now!!!
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	vbo_.create();
	vbo_.bind();
	vbo_.allocate(vbo_data, sizeof(vbo_data));

	QOpenGLShader *vshader = new QOpenGLShader(QOpenGLShader::Vertex, this);
	const char *vsrc =
			"attribute mediump vec2 VertexUV;\n"
			"attribute highp vec3 vertexCoord;\n"
			"varying mediump vec2 UV;\n"
			"void main(){\n"
			"	gl_Position =  vec4(vertexCoord,1);\n"
			"	UV = VertexUV;\n"
			"}\n";
	vshader->compileSourceCode(vsrc);

	QOpenGLShader *fshader = new QOpenGLShader(QOpenGLShader::Fragment, this);
	const char *fsrc =
			"varying mediump vec2 UV;\n"
			"uniform sampler2D texture;\n"
			"void main(){\n"
			"	gl_FragColor = texture2D(texture, UV);\n"
			"}\n";
	fshader->compileSourceCode(fsrc);

	program_ = new QOpenGLShaderProgram;
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
//	glBindTexture(GL_TEXTURE_2D, textureID);

	glDrawArrays(GL_TRIANGLES, 0, 6);
}

constexpr GLfloat OpenGLWidget::vbo_data[];
