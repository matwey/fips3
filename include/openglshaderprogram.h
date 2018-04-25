/*
 *  Copyright (C) 2018  Matwey V. Kornilov <matwey.kornilov@gmail.com>
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

#ifndef _OPENGLSHADERPROGRAM_H
#define _OPENGLSHADERPROGRAM_H

#include <array>

#include <QOpenGLShaderProgram>
#include <QString>

class OpenGLShaderProgram: public QOpenGLShaderProgram {
public:
	OpenGLShaderProgram(QObject *parent = Q_NULLPTR);
	virtual ~OpenGLShaderProgram() override;

	bool addFragmentShaderFromSourceCode(const QString& source);
	bool addVertexShaderFromSourceCode(const QString& source);

	void setVertexCoordArray(const GLfloat *values, int tupleSize);
	void setVertexUVArray(const GLfloat *values, int tupleSize);

	void setMVPUniform(const QMatrix4x4& mvp);
	void setCUniform(const std::array<GLfloat, 4>& array, const quint8& channels);
	void setZUniform(const std::array<GLfloat, 4>& array, const quint8& channels);

	virtual bool link() override;

	static constexpr const char vertex_coord_name[] = "vertexCoord";
	static constexpr const char vertex_UV_name[] = "vertexUV";
	static constexpr const int vertex_coord_index = 0;
	static constexpr const int vertex_UV_index = 1;
	static constexpr const int image_texture_index = 0;
	static constexpr const int colormap_texture_index = 1;
};

#endif // _OPENGLSHADERPROGRAM_H
