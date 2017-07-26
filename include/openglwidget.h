#ifndef _OPENGLWIDGET_H_
#define _OPENGLWIDGET_H_

#include <QOpenGLBuffer>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOpenGLPixelTransferOptions>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLWidget>
#include <QMatrix4x4>

#include <exception.h>

#include <fits.h>

class OpenGLWidget: public QOpenGLWidget, protected QOpenGLFunctions {
public:
	class Exception: public ::Exception {
	public:
		virtual void raise() const override;
		virtual QException* clone() const override;
	};

	class ShaderLoadError: public Exception {
	public:
		virtual void raise() const override;
		virtual QException* clone() const override;
	};

	class ShaderBindError: public Exception {
	public:
		virtual void raise() const override;
		virtual QException* clone() const override;
	};

	class ShaderCompileError: public Exception {
	public:
		virtual void raise() const override;
		virtual QException* clone() const override;
	};

	template<class T> class OpenGLDeleter {
	private:
		QOpenGLWidget *openGL_widget_;
	public:
		OpenGLDeleter(QOpenGLWidget *openGL_widget):
				openGL_widget_(openGL_widget){}
		void operator() (T* ptr) {
			openGL_widget_->makeCurrent();
			delete ptr;
			openGL_widget_->doneCurrent();
		};
	};

	template<class T> using openGL_unique_ptr = std::unique_ptr<T, OpenGLDeleter<T>>;

public:
	OpenGLWidget(QWidget *parent, FITS* fits);
	~OpenGLWidget();

	void setViewport(const QRect& viewport);
	inline const QRect& viewport() const { return viewport_; }
	inline QSize fits_size() const { return fits_->data_unit().size(); }

protected:
	void initializeGL() override;
	void resizeGL(int w, int h) override;
	void paintGL() override;
	QSize sizeHint() const override;

private:
	std::unique_ptr<FITS> fits_;
	OpenGLDeleter<QOpenGLTexture> texture_deleter_;
	openGL_unique_ptr<QOpenGLTexture> texture_;
	OpenGLDeleter<QOpenGLPixelTransferOptions> pixel_transfer_options_deleter_;
	openGL_unique_ptr<QOpenGLPixelTransferOptions> pixel_transfer_options_;
	OpenGLDeleter<QOpenGLShaderProgram> program_deleter_;
	openGL_unique_ptr<QOpenGLShaderProgram> program_;
	QOpenGLBuffer vbo_;
	QMatrix4x4 base_mvp_;

	static const int program_vertex_coord_attribute = 0;
	static const int program_vertex_uv_attribute    = 1;
	static constexpr GLfloat vbo_data[] = {
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
	};

	QRect viewport_;
};


#endif // _OPENGLWIDGET_H_
