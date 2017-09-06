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
#include <QMessageBox>
#include <QResizeEvent>

#include <array>
#include <cmath>
#include <memory>

#include <fits.h>
#include <openglcolormap.h>
#include <openglerrors.h>
#include <openglshaderunifroms.h>
#include <opengltexture.h>
#include <pixel.h>

class OpenGLWidget: public QOpenGLWidget, protected QOpenGLFunctions {
	Q_OBJECT
public:
	class ShaderLoadError: public OpenGLException {
	public:
		ShaderLoadError(GLenum gl_error_code);

		virtual void raise() const override;
		virtual QException* clone() const override;
	};

	class ShaderBindError: public OpenGLException {
	public:
		ShaderBindError(GLenum gl_error_code);

		virtual void raise() const override;
		virtual QException* clone() const override;
	};

	class ShaderCompileError: public OpenGLException {
	public:
		ShaderCompileError(GLenum gl_error_code);

		virtual void raise() const override;
		virtual QException* clone() const override;
	};

	class VertexCoordinates {
	private:
		const QSize image_size_;
		const GLfloat factor_;
		std::array<GLfloat, 8> data_;
	public:
		VertexCoordinates(const QSize &image_size, GLfloat factor);
		VertexCoordinates(const QSize &image_size);

		inline GLfloat factor() { return factor_; }
		inline const GLfloat* data() { return data_.data(); }
		QRectF borderRect(GLfloat angle);
		inline GLfloat left()   { return data_[0]; }
		inline GLfloat right()  { return data_[4]; }
		inline GLfloat bottom() { return data_[1]; }
		inline GLfloat top()    { return data_[5]; }
		inline GLfloat width()  { return right() - left(); }
		inline GLfloat height() { return top() - bottom(); }
		inline QSizeF size() { return {width(), height()}; }
		inline QPointF center() { return {0, 0}; } // Always should be zero
	};

	template<class T> class OpenGLDeleter {
	private:
		QOpenGLWidget *openGL_widget_;
	public:
		OpenGLDeleter(): openGL_widget_(nullptr) {}
		OpenGLDeleter(QOpenGLWidget *openGL_widget): openGL_widget_(openGL_widget) {}
		OpenGLDeleter(const OpenGLDeleter& other) = default;
		OpenGLDeleter& operator=(const OpenGLDeleter& other) = default;
		OpenGLDeleter(OpenGLDeleter&& other) = default;
		OpenGLDeleter& operator=(OpenGLDeleter&& other) = default;
		inline void operator() (T* ptr) {
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

private:
	void initializeGLObjects();

signals:
	void textureInitialized(const OpenGLTexture* texture);

protected:
	virtual void initializeGL() override;
	virtual void paintGL() override;
	virtual QSize sizeHint() const override;
	virtual void resizeEvent(QResizeEvent* event) override;

public:
	inline QSize image_size() const { return hdu_->data().imageDataUnit()->size(); }
	void setHDU(const FITS::HeaderDataUnit& hdu);
	Pixel pixelFromWidgetCoordinate(const QPoint &widget_coord) const;

private:
	const FITS::HeaderDataUnit* hdu_;
	openGL_unique_ptr<OpenGLTexture> texture_;
	openGL_unique_ptr<QOpenGLShaderProgram> program_;
	QMatrix4x4 base_mvp_;

private:
	static const int program_vertex_coord_attribute_ = 0;
	static const int program_vertex_uv_attribute_    = 1;
	static const int program_texture_uniform_  = 0;
	static const int program_colormap_uniform_ = 1;
	// UV coordinates for triangle fan. See vertex_data_
	static constexpr const GLfloat uv_data[] = {
			0.0f, 0.0f,
			0.0f, 1.0f,
			1.0f, 1.0f,
			1.0f, 0.0f
	};
	std::unique_ptr<VertexCoordinates> vertex_coords_;

private:
	QRectF viewrect_;
	QRect pixel_viewrect_;
	// Returns true if viewrect has been corrected
	bool alignViewrect();
protected:
	QRect viewrectToPixelViewrect (const QRectF& viewrect) const;
public:
	void setViewrect(const QRectF &viewrect);
	inline const QRectF& viewrect() const { return viewrect_; }
	void setPixelViewrect(const QRect& pixel_viewrect);
	inline const QRect& pixelViewrect() const { return pixel_viewrect_; }
	void fitViewrect();
signals:
	void pixelViewrectChanged(const QRect& pixel_viewrect);

private:
	double angle_ = 0; // degrees
public slots:
	void changeRotationAngle(double angle);
signals:
	void rotationAngleChanged(double angle);

private:
	std::unique_ptr<OpenGLShaderUniforms> shader_uniforms_;
public slots:
	void changeLevels(const std::pair<double, double>& minmax);

private:
	colormaps_type colormaps_;
	int colormap_index_;
public:
	inline const colormaps_type& colormaps() const { return colormaps_; }
	inline int colorMapIndex() const {return colormap_index_; }
public slots:
	void changeColorMap(int colormap_index);
};


#endif // _OPENGLWIDGET_H_
