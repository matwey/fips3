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

#include <openglshaderprogram.h>

OpenGLShaderProgram::OpenGLShaderProgram(QObject *parent):
	QOpenGLShaderProgram(parent) {
}

OpenGLShaderProgram::~OpenGLShaderProgram() = default;

bool OpenGLShaderProgram::addFragmentShaderFromSourceCode(const QString& source) {
	return addShaderFromSourceCode(QOpenGLShader::Fragment, source);
}

bool OpenGLShaderProgram::addVertexShaderFromSourceCode(const QString& source) {
	return addShaderFromSourceCode(QOpenGLShader::Vertex, source);
}

void OpenGLShaderProgram::setVertexCoordArray(const GLfloat *values, int tupleSize) {
	setAttributeArray(OpenGLShaderProgram::vertex_coord_index, values, tupleSize);
}

void OpenGLShaderProgram::setVertexUVArray(const GLfloat *values, int tupleSize) {
	setAttributeArray(OpenGLShaderProgram::vertex_UV_index, values, tupleSize);
}

void OpenGLShaderProgram::setMVPUniform(const QMatrix4x4& mvp) {
	setUniformValue("MVP", mvp);
}

void OpenGLShaderProgram::setCUniform(const std::array<GLfloat, 4>& array, const quint8& channels) {
	setUniformValueArray("c", array.data(), 1, channels);
}

void OpenGLShaderProgram::setZUniform(const std::array<GLfloat, 4>& array, const quint8& channels) {
	setUniformValueArray("z", array.data(), 1, channels);
}

bool OpenGLShaderProgram::link() {
	bindAttributeLocation(vertex_coord_name, vertex_coord_index);
	bindAttributeLocation(vertex_UV_name, vertex_UV_index);

	if (!QOpenGLShaderProgram::link()) return false;

	enableAttributeArray(vertex_coord_index);
	enableAttributeArray(vertex_UV_index);

	if (!bind()) return false;

	setUniformValue("texture", image_texture_index);
	setUniformValue("colormap", colormap_texture_index);

	return true;
}

constexpr const char OpenGLShaderProgram::vertex_coord_name[];
constexpr const char OpenGLShaderProgram::vertex_UV_name[];
constexpr const int OpenGLShaderProgram::vertex_coord_index;
constexpr const int OpenGLShaderProgram::vertex_UV_index;
constexpr const int OpenGLShaderProgram::image_texture_index;
constexpr const int OpenGLShaderProgram::colormap_texture_index;
