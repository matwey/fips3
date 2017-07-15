#ifndef _OPENGLWIDGET_H_
#define _OPENGLWIDGET_H_

#include <QOpenGLBuffer>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLWidget>

#include <fits.h>

class OpenGLWidget: public QOpenGLWidget, protected QOpenGLFunctions {
public:
	OpenGLWidget(QWidget *parent, const QString &fits_filename);
	~OpenGLWidget();

protected:
	void initializeGL() override;
	void resizeGL(int w, int h) override;
	void paintGL() override;

private:
	const QString fits_filename_;
	FITS *fits_;
	QImage *img_;
	QOpenGLTexture *texture_;
//	GLuint textureID;
	QOpenGLBuffer vbo_;
	QOpenGLShaderProgram *program_;
	static const int program_vertex_coord_attribute = 0;
	static const int program_vertex_uv_attribute    = 1;
	static const int program_texture_uniform = 0;

	static constexpr GLfloat vbo_data[] = {
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
	};
};


#endif // _OPENGLWIDGET_H_
