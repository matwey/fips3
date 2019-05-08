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

#include <cmath>

#include <opengltransform.h>

OpenGLTransform::OpenGLTransform(QObject* parent):
	AbstractOpenGLTransform(parent),
	expired_(true),
	matrix_(), // QMatrix4x4() constructs an unity matrix
	angle_(0),
	viewrect_(-1, -1, 2, 2),
	h_flip_(false),
	v_flip_(false),
	wcs_()
{}
OpenGLTransform::OpenGLTransform(const QRectF& viewrect, QObject* parent):
	OpenGLTransform(parent) {

	setViewrect(viewrect);
}

OpenGLTransform::~OpenGLTransform() = default;

void OpenGLTransform::updateTransform() const {
// FIXME: this function plays with mutable objects and looks like const-function,
// consider to implement it in reentable way in order to avoid threading issues.

	matrix_.setToIdentity();
	matrix_.ortho(QRectF{viewrect_.left(), -viewrect_.top(), viewrect_.width(), -viewrect_.height()});
	matrix_.rotate(angle_, static_cast<float>(0), static_cast<float>(0), static_cast<float>(1));
	matrix_.scale(h_flip_ ? -1 : 1, v_flip_ ? -1 : 1);

	auto cwcs = QMatrix4x4(wcs_);
	cwcs.setColumn(3, QVector4D(static_cast<float>(0), static_cast<float>(0), static_cast<float>(0), static_cast<float>(1)));
	auto s = std::sqrt(std::abs(cwcs.determinant()));
	cwcs.scale(1.0/s, 1.0/s, 1);
	matrix_ *= cwcs;

	expired_ = false;
}

const QMatrix4x4& OpenGLTransform::transformMatrix() const {
	if (expired_) updateTransform();

	return matrix_;
}

void OpenGLTransform::setRotation(float angle) {
	if (angle_ == angle) return;

	angle_ = angle;
	expired_ = true;
}

void OpenGLTransform::setViewrect(const QRectF& viewrect) {
	if (viewrect_ == viewrect) return;

	viewrect_ = viewrect;
	expired_ = true;
}

void OpenGLTransform::setHorizontalFlip(bool flip) {
	if (h_flip_ == flip) return;

	h_flip_ = flip;
	expired_ = true;
}

void OpenGLTransform::setVerticalFlip(bool flip) {
	if (v_flip_ == flip) return;

	v_flip_ = flip;
	expired_ = true;
}

void OpenGLTransform::setWcsMatrix(const QMatrix4x4& wcs) {
	if (wcs_ == wcs) return;

	wcs_ = wcs;
	expired_ = true;
}

void OpenGLTransform::setImageSize(const QSize& image_size) {
	if (image_size_ == image_size) return;

	image_size_ = image_size;
	expired_ = true;
}

WidgetToFitsOpenGLTransform::WidgetToFitsOpenGLTransform(QObject* parent):
	OpenGLTransform(parent) {}

WidgetToFitsOpenGLTransform::WidgetToFitsOpenGLTransform(const QSize& image_size, qreal scale, const QSize& widget_size, const QRectF& viewrect, QObject* parent):
	OpenGLTransform(viewrect, parent) {

	setImageSize(image_size);
	setScale(scale);
	setWidgetSize(widget_size);
}

WidgetToFitsOpenGLTransform::~WidgetToFitsOpenGLTransform() = default;

void WidgetToFitsOpenGLTransform::updateTransform() const {
	matrix_.setToIdentity();

	auto cwcs = QMatrix4x4(wcs_);
	cwcs.setColumn(3, QVector4D(static_cast<float>(0), static_cast<float>(0), static_cast<float>(0), static_cast<float>(1)));
	auto s = std::sqrt(std::abs(cwcs.determinant()));
	cwcs.scale(1.0/s, 1.0/s, 1);

	/* texture (0,0, 1,1) to image */
	matrix_.scale(image_size_.width(), image_size_.height());

	/* world to plane */
	matrix_.translate(static_cast<float>(0.5), static_cast<float>(0.5));
	matrix_.scale(static_cast<float>(0.5)/(scale_*image_size_.width()),
		static_cast<float>(0.5)/(scale_*image_size_.height()));

	matrix_ *= cwcs.inverted();

	/* flip */
	matrix_.scale(h_flip_ ? -1 : 1, v_flip_ ? -1 : 1);
	/* world unrotated */
	matrix_.rotate(-angle_, static_cast<float>(0), static_cast<float>(0), static_cast<float>(1));
	/* viewrect to world */
	matrix_.translate(viewrect_.center().x(), -viewrect_.center().y());
	matrix_.scale(viewrect_.width()/static_cast<float>(2), -viewrect_.height()/static_cast<float>(2));

	/* widget pixel (0,0, w,h) to viewrect (-1,-1, 2,2)*/
	const auto widget_width = widget_size_.width();
	const auto widget_height = widget_size_.height();
	const float tr_x = -static_cast<float>(widget_width-1) / static_cast<float>(widget_width);
	const float tr_y = -static_cast<float>(widget_height-1) / static_cast<float>(widget_height);
	matrix_.translate(tr_x, tr_y);
	matrix_.scale(static_cast<float>(2)/widget_width, static_cast<float>(2)/widget_height);

	qDebug() << "w2f" << matrix_;

	expired_ = false;
}

void WidgetToFitsOpenGLTransform::setScale(const qreal& scale) {
	if (scale_ == scale) return;

	scale_ = scale;
	expired_ = true;
}

void WidgetToFitsOpenGLTransform::setWidgetSize(const QSize& widget_size) {
	if (widget_size_ == widget_size) return;

	widget_size_ = widget_size;
	expired_ = true;
}

void FitsToWCSOpenGLTransform::setWcsMatrix(const QMatrix4x4& matrix) {
	if (wcs_matrix_ == matrix) return;
	
	wcs_matrix_ = matrix;
}
