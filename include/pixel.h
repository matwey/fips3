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

#ifndef _PIXEL_H
#define _PIXEL_H

#include <QPoint>

#include <utils/optional.h>

struct Pixel {
	QPoint position;
	Utils::Optional<double> value;

	Pixel(const QPoint& position, const Utils::Optional<double>& value);
	Pixel(const QPoint& position, double value, bool inside_image);
	Pixel(const QPoint& position, double value);
	Pixel(const QPoint& position);
};

#endif //_PIXEL_H
