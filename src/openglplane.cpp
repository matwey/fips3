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

OpenGLPlane::OpenGLPlane(QObject* parent):
	QObject(parent),
	vertex_buffer_(QOpenGLBuffer::VertexBuffer),
	UV_buffer_(QOpenGLBuffer::VertexBuffer) {

}

OpenGLPlane::~OpenGLPlane() = default;

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
	return initializeBufferHelper(vertex_buffer_, OpenGLPlane::vertex_data, sizeof(OpenGLPlane::vertex_data), OpenGLShaderProgram::vertex_coord_index);
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
constexpr const GLfloat OpenGLPlane::vertex_data[];
