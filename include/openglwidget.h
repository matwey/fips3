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
#include <QResizeEvent>

#include <cmath>

#include <exception.h>
#include <fits.h>
#include <openglcolormap.h>
#include <openglshaderunifroms.h>
#include <opengltexture.h>

class OpenGLWidget: public QOpenGLWidget, protected QOpenGLFunctions {
	Q_OBJECT
public:
	class Exception: public ::Exception {
	public:
		Exception(const QString &reason, GLenum gl_error_code);

		virtual void raise() const override;
		virtual QException* clone() const override;
		static QString glErrorString(GLenum gl_error_code);
	};

	class ShaderLoadError: public Exception {
	public:
		ShaderLoadError(GLenum gl_error_code);

		virtual void raise() const override;
		virtual QException* clone() const override;
	};

	class ShaderBindError: public Exception {
	public:
		ShaderBindError(GLenum gl_error_code);

		virtual void raise() const override;
		virtual QException* clone() const override;
	};

	class ShaderCompileError: public Exception {
	public:
		ShaderCompileError(GLenum gl_error_code);

		virtual void raise() const override;
		virtual QException* clone() const override;
	};

	class TextureCreateError: public Exception {
	public:
		TextureCreateError(GLenum gl_error_code);

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
	typedef std::array<openGL_unique_ptr<OpenGLColorMap>, 2> colormaps_type;
	typedef OpenGLDeleter<OpenGLColorMap> colormap_deleter_type;

public:
	OpenGLWidget(QWidget *parent, const FITS::HeaderDataUnit& hdu);
	~OpenGLWidget() override;

	void setViewrect(const QRectF &viewrect);
	inline const QRectF& viewrect() const { return viewrect_; }
	void setPixelViewrect(const QRect& pixel_viewrect);
	inline const QRect& pixelViewrect() const { return pixel_viewrect_; }
	inline QSize image_size() const { return hdu_->data().imageDataUnit()->size(); }
	QRect viewrectToPixelViewrect (const QRectF& viewrect) const;
	inline const colormaps_type& colormaps() const { return colormaps_; }
	inline int colorMapIndex() const {return colormap_index_; }

signals:
	void pixelViewrectChanged(const QRect& pixel_viewrect);
	void textureInitialized(const OpenGLTexture* texture);

public slots:
	void changeLevels(const std::pair<double, double>& minmax);
	void changeColorMap(int colormap_index);

protected:
	void initializeGL() override;
	void paintGL() override;
	QSize sizeHint() const override;
	void resizeEvent(QResizeEvent* event) override;

private:
	const FITS::HeaderDataUnit* hdu_;
	OpenGLDeleter<OpenGLTexture> texture_deleter_;
	openGL_unique_ptr<OpenGLTexture> texture_;
	OpenGLDeleter<QOpenGLPixelTransferOptions> pixel_transfer_options_deleter_;
	openGL_unique_ptr<QOpenGLPixelTransferOptions> pixel_transfer_options_;
	OpenGLDeleter<QOpenGLShaderProgram> program_deleter_;
	openGL_unique_ptr<QOpenGLShaderProgram> program_;
	QOpenGLBuffer vbo_;
	QMatrix4x4 base_mvp_;

	static const int program_vertex_coord_attribute_ = 0;
	static const int program_vertex_uv_attribute_    = 1;
	static const int program_texture_uniform_  = 0;
	static const int program_colormap_uniform_ = 1;
	// Square which is made from two triangles. Each line is xyz coordinates of triangle vertex (0-2 - first triangle,
	// 3-5 - second triangle). First and seconds columns are used as corresponding UV-coordinates.
	static constexpr const GLfloat vbo_data[] = {
			0.0f, 0.0f, 0.0f,
			1.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			1.0f, 0.0f, 0.0f,
			1.0f, 1.0f, 0.0f,
	};

	QRectF viewrect_;
	QRect pixel_viewrect_;

	// Returns true if viewrect has been corrected
	bool correct_viewrect();

	template<class T> void throwIfGLError() throw(T) {
		const auto gl_error_code = glGetError();
		if (gl_error_code) {
			throw T(gl_error_code);
		}
	}

	std::unique_ptr<OpenGLShaderUniforms> shader_uniforms_;

	colormaps_type colormaps_;
	int colormap_index_;
};


#endif // _OPENGLWIDGET_H_
