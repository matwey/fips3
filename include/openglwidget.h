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
#include <QTransform>

#include <array>
#include <cmath>

#include <abstractopenglplan.h>
#include <fits.h>
#include <openglcolormap.h>
#include <openglerrors.h>
#include <openglplane.h>
#include <openglshaderprogram.h>
#include <openglshaderunifroms.h>
#include <opengltexture.h>
#include <opengltransform.h>
#include <viewrect.h>
#include <pixel.h>

class OpenGLWidget: public QOpenGLWidget, protected QOpenGLFunctions {
	Q_OBJECT
public:
	class PlanCreationError: public Utils::Exception {
	public:
		PlanCreationError();

		virtual void raise() const override;
		virtual QException* clone() const override;
	};

	class PlanInitializationError: public Utils::Exception {
	public:
		explicit PlanInitializationError(const AbstractOpenGLPlan& plan);

		virtual void raise() const override;
		virtual QException* clone() const override;
	};

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
	OpenGLWidget(QWidget *parent, const FITS::AbstractHeaderDataUnit& hdu);

private:
	void initializeGLObjects();

signals:
	void textureInitialized(const AbstractOpenGLTexture& texture);

protected:
	virtual void initializeGL() override;
	virtual void paintGL() override;
	virtual QSize sizeHint() const override;
	virtual void resizeEvent(QResizeEvent* event) override;

public:
	inline QSize image_size() const { return hdu_->data().imageDataUnit()->size(); }
	void setHDU(const FITS::AbstractHeaderDataUnit& hdu);
	Pixel pixelFromWidgetCoordinate(const QPoint &widget_coord);

private:
	const FITS::AbstractHeaderDataUnit* hdu_;
	openGL_unique_ptr<AbstractOpenGLPlan> plan_;
private:
	Viewrect viewrect_;
public:
	inline Viewrect& viewrect() { return viewrect_; }
	inline void fitViewrect() { viewrect_.fitToBorder(size()); };
private slots:
	void viewChanged(const QRectF& view_rect);

private:
	OpenGLTransform opengl_transform_;
	WidgetToFitsOpenGLTransform widget_to_fits_;
protected:
	void flipViewrect(Qt::Axis flip_axis);
public:
	inline double rotation() const { return opengl_transform_.rotation(); }
	inline const bool& horizontalFlip() const { return opengl_transform_.horizontalFlip(); }
	inline const bool& verticalFlip() const { return opengl_transform_.verticalFlip(); }
public slots:
	void setRotation(double angle);
	void setHorizontalFlip(bool flip);
	void setVerticalFlip(bool flip);
signals:
	void rotationChanged(double angle);
	void horizontalFlipChanged(bool flip);
	void verticalFlipChanged(bool flip);

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
