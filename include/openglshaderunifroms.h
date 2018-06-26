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

#ifndef _OPENGLSHADERUNIFROMS_H
#define _OPENGLSHADERUNIFROMS_H

#include <QtGlobal>
#include <QtGui/qopengl.h>

#include <array>
#include <cmath>

class OpenGLShaderUniforms {
public:
	typedef std::array<GLfloat, 4> vec4_type;
private:
	vec4_type a_;
	vec4_type c_, z_;
	std::pair<double, double> minmax_;
	int colormap_size_;
public:
	const quint8 channels, channel_size;
	const double bzero, bscale;
	const double base;

	OpenGLShaderUniforms(quint8 channels, quint8 channel_size, double bzero, double bscale);

	void setMinMax(const std::pair<double, double>& minmax);
	void setColorMapSize(int colormap_size);
	inline const vec4_type& get_a() const { return a_; }
	inline const vec4_type& get_c() const { return c_; }
	inline const vec4_type& get_z() const { return z_; }
private:
	void update_cz();
};


#endif //_OPENGLSHADERUNIFROMS_H
