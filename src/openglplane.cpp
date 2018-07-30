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

#include <algorithm>

#include <QMatrix4x4>
#include <QRectF>

#include <openglplane.h>

OpenGLPlane::OpenGLPlane(const QSize& image_size, QObject* parent):
	QObject(parent),
	vertex_buffer_(QOpenGLBuffer::VertexBuffer),
	UV_buffer_(QOpenGLBuffer::VertexBuffer) {

	setImageSize(image_size);
}

OpenGLPlane::~OpenGLPlane() = default;

void OpenGLPlane::updateScale() {
	const auto w = image_size_.width();
	const auto h = image_size_.height();
	const qreal scale = static_cast<qreal>(1) / std::max(w,h);

	if (scale_ == scale) return;

	scale_ = scale;
}

void OpenGLPlane::updateVertexArray() {
	const auto p = planeRect();

	vertices_[0] = p.left();
	vertices_[1] = p.top();

	vertices_[2] = p.left();
	vertices_[3] = p.bottom();

	vertices_[4] = p.right();
	vertices_[5] = p.bottom();

	vertices_[6] = p.right();
	vertices_[7] = p.top();
}

void OpenGLPlane::setImageSize(const QSize& image_size) {
	if (image_size_ == image_size) return;

	image_size_ = image_size;

	updateScale();
	updateVertexArray();
}

QRectF OpenGLPlane::planeRect() const {
	const auto w = image_size_.width();
	const auto h = image_size_.height();
	const auto x = (w < h ? static_cast<qreal>(w)/h : static_cast<qreal>(1));
	const auto y = (w < h ? static_cast<qreal>(1) : static_cast<qreal>(h)/w);
	const QRectF p{QPointF{-x,-y}, QPointF{x,y}};

	return p;
}

QRectF OpenGLPlane::borderRect(float angle) const {
	const auto p = planeRect();
	QMatrix4x4 rotation_matrix;
	// Rotation in viewrect coordinates is clockwise, but it doesn't matter in
	// the case of rectangle with the center in (0,0)
	rotation_matrix.rotate(-angle, 0, 0, 1);
	// Arguments are top left and bottom right corners in viewrect coordinates:
	return rotation_matrix.mapRect(p);
}

bool OpenGLPlane::initializeBufferHelper(QOpenGLBuffer& buffer, const void* data, int count, GLuint index) {
	Q_ASSERT(&buffer == &vertex_buffer_ || &buffer == &UV_buffer_);

	if (!buffer.create()) {
		return false;
	}

	buffer.bind();
	buffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
	buffer.allocate(data, count);
	buffer.release();

	return true;
}

bool OpenGLPlane::initializeVertexBuffer() {
	return initializeBufferHelper(vertex_buffer_, vertices_.data(), sizeof(float) * vertices_.size(), OpenGLShaderProgram::vertex_coord_index);
}

bool OpenGLPlane::initializeUVBuffer() {
	return initializeBufferHelper(UV_buffer_, OpenGLPlane::uv_data, sizeof(OpenGLPlane::uv_data), OpenGLShaderProgram::vertex_UV_index);

}

bool OpenGLPlane::initialize() {
	if (!initializeVertexBuffer()) {
		return false;
	}
	if (!initializeUVBuffer()) {
		return false;
	}

	return true;
}

constexpr const GLfloat OpenGLPlane::uv_data[];
