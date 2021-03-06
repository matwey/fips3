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

#ifndef _OPENGLTRANSFORM_H
#define _OPENGLTRANSFORM_H

#include <abstractopengltransform.h>

class OpenGLTransform:
	public AbstractOpenGLTransform {
protected:
	mutable bool expired_;
	mutable QMatrix4x4 matrix_;

	QSize image_size_;
	float scale_x_, scale_y_;
	float angle_;
	bool h_flip_, v_flip_;
	QSize widget_size_;
	QPoint vpos_;
	float scale_;

	void updateTransformHelper(QMatrix4x4& init_matrix) const;
	virtual void updateTransform() const;
public:
	explicit OpenGLTransform(QObject* parent = Q_NULLPTR);
	OpenGLTransform(const QSize& image_size, const QSize& widget_size, float scale, QObject* parent = Q_NULLPTR);
	virtual ~OpenGLTransform() override;

	inline void updateTransform() {
		const_cast<const OpenGLTransform*>(this)->updateTransform();
	};
	const QMatrix4x4& transformMatrix() const override;
	const QRectF border() const;

	const QSize& imageSize() const { return image_size_; }
	void setImageSize(const QSize& image_size);

	const float& rotation() const { return angle_; }
	void setRotation(float angle);

	const bool& horizontalFlip() const { return h_flip_; }
	void setHorizontalFlip(bool flip);
	const bool& verticalFlip() const { return v_flip_; }
	void setVerticalFlip(bool flip);

	const QSize& widgetSize() const { return widget_size_; }
	void setWidgetSize(const QSize& widget_size);

	const QPoint& virtualPos() const { return vpos_; }
	void setVirtualPos(const QPoint& vpos);

	const float& scale() const { return scale_; }
	void setScale(float scale);
};

class WidgetToFitsOpenGLTransform:
	public OpenGLTransform {
private:
	QSize widget_size_;
protected:
	void updateTransform() const override;
public:
	explicit WidgetToFitsOpenGLTransform(QObject* parent = Q_NULLPTR);
	WidgetToFitsOpenGLTransform(const QSize& image_size, const QSize& widget_size, float scale, QObject* parent = Q_NULLPTR);
	~WidgetToFitsOpenGLTransform() override;
};

#endif // _OPENGLTRANSFORM_H
