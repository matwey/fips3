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

#include <openglcolormap.h>

OpenGLColorMap::OpenGLColorMap():
	QOpenGLTexture(QOpenGLTexture::Target2D) {}

OpenGLColorMap::~OpenGLColorMap() = default;

void OpenGLColorMap::initialize() {
	setMinificationFilter(QOpenGLTexture::Linear);
	setMagnificationFilter(QOpenGLTexture::Linear);
	setWrapMode(QOpenGLTexture::MirroredRepeat);
	setFormat(QOpenGLTexture::RGBAFormat);
	setSize(size()/4);
	allocateStorage();
	setData(QOpenGLTexture::RGBA, QOpenGLTexture::UInt8, data());
	initialized = true;
}


GrayscaleColorMap::GrayscaleColorMap():
	OpenGLColorMap(),
	colormap_{{0,0,0,255,255,255,255,255}} {}

GrayscaleColorMap::~GrayscaleColorMap() = default;

int GrayscaleColorMap::size() const {
	return colormap_.size();
}

const quint8* GrayscaleColorMap::data() const {
	return colormap_.data();
}

const char* GrayscaleColorMap::name() const {
	return "Grayscale";
}


PurpleBlueColorMap::PurpleBlueColorMap():
		OpenGLColorMap(),
		colormap_{{255,0,255,255, 0,0,255,255, 0,255,255,255}} {}

PurpleBlueColorMap::~PurpleBlueColorMap() = default;

int PurpleBlueColorMap::size() const {
	return colormap_.size();
}

const quint8* PurpleBlueColorMap::data() const {
	return colormap_.data();
}

const char* PurpleBlueColorMap::name() const {
	return "Purple—Blue";
}
