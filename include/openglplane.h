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

#ifndef _OPENGLPLANE_H
#define _OPENGLPLANE_H

#include <QObject>
#include <QSize>
#include <QOpenGLShaderProgram>

#include <array>

class OpenGLPlane: public QObject {
private:
	QSize image_size_;
	qreal scale_;

	std::array<float, 8> vertices_;

	void updateVertexArray();
	void updateScale();
	void setImageSize(const QSize& image_size);
public:
	OpenGLPlane(const QSize& image_size, QObject* parent = Q_NULLPTR);
	virtual ~OpenGLPlane();

	inline const qreal& scale() const { return scale_; }

	QRectF planeRect() const;
	QRectF borderRect(float angle) const;

	inline const float* vertexArray() const { return vertices_.data(); }
	inline std::size_t vertexArraySize() const { return vertices_.size(); }
public:
	// UV coordinates for triangle fan. See vertex_data_
	static constexpr const GLfloat uv_data[] = {
			0.0f, 0.0f,
			0.0f, 1.0f,
			1.0f, 1.0f,
			1.0f, 0.0f
	};
};

#endif // _OPENGLPLANE_H
