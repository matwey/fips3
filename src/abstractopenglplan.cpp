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

#include <abstractopenglplan.h>

AbstractOpenGLPlan::AbstractOpenGLPlan(const QString& name, const FITS::AbstractHeaderDataUnit& hdu,
	const std::pair<double, double>& hdu_minmax,
	const std::pair<double, double>& instrumental_minmax,
	quint8 channels, quint8 channel_size, QObject* parent):
	QObject(parent),
	name_(name),
	plane_(hdu.data().imageDataUnit()->size()),
	program_(),
	hdu_minmax_(hdu_minmax),
	instrumental_minmax_(instrumental_minmax),
	channels_(channels),
	channel_size_(channel_size) {
}

AbstractOpenGLPlan::~AbstractOpenGLPlan() = default;

QString AbstractOpenGLPlan::vertexShaderSourceCode() {
	static const QString source = R"(
		attribute vec2 vertexCoord;
		attribute vec2 VertexUV;
		varying vec2 UV;
		uniform mat4 MVP;

		void main() {
			gl_Position = MVP * vec4(vertexCoord,0,1);
			UV = VertexUV;
		}
	)";

	return source;
}

bool AbstractOpenGLPlan::initialize() {
	QOpenGLFunctions::initializeOpenGLFunctions();

	const QString vsrc = vertexShaderSourceCode();
	const QString fsrc = fragmentShaderSourceCode();

	imageTexture().initialize();

	if (!program_.addFragmentShaderFromSourceCode(fsrc)) {
		return false;
	}
	if (!program_.addVertexShaderFromSourceCode(vsrc)) {
		return false;
	}
	program_.setVertexCoordArray(plane().vertexArray(), 2);
	program_.setVertexUVArray(plane().uv_data, 2);

	return program_.link();
}

void AbstractOpenGLPlan::draw() {
	this->glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}
