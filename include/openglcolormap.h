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

#ifndef _OPENGL_COLORMAP_H
#define _OPENGL_COLORMAP_H

#include <QDebug>
#include <QOpenGLTexture>

#include <array>

class OpenGLColorMap: public QOpenGLTexture {
private:
	bool initialized = false;
public:
	OpenGLColorMap();
	virtual ~OpenGLColorMap() = 0;

	void initialize();
	inline bool isInitialized() { return initialized; }

	virtual const char* name() const = 0;
protected:
	virtual int size() const = 0;
	virtual const quint8* data() const = 0;
};


class GrayscaleColorMap: public OpenGLColorMap {
private:
	std::array<quint8, 8> colormap_;
public:
	GrayscaleColorMap();
	virtual ~GrayscaleColorMap() override;
	virtual const char* name() const override;
protected:
	virtual int size() const override;
	virtual const quint8* data() const override;
};

class PurpleBlueColorMap: public OpenGLColorMap {
private:
	std::array<quint8, 12> colormap_;
public:
	PurpleBlueColorMap();
	virtual ~PurpleBlueColorMap() override;
	virtual const char* name() const override;
protected:
	virtual int size() const override;
	virtual const quint8* data() const override;
};

#endif //_OPENGL_COLORMAP_H
