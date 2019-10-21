/*
 *  Copyright (C) 2019  Matwey V. Kornilov <matwey.kornilov@gmail.com>
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

#ifndef _BORDERRECTF_H
#define _BORDERRECTF_H

#include <QPointF>
#include <QRectF>
#include <QSizeF>

class BorderRectF: public QRectF {
public:
	BorderRectF(const QRectF& rectangle);
	BorderRectF(qreal x, qreal y, qreal width, qreal height);
	BorderRectF(const QPointF& topLeft, const QPointF& bottomRight);
	BorderRectF(const QPointF& topLeft, const QSizeF& size);
	BorderRectF() =  default;

	~BorderRectF() =  default;

	QRectF alignRect(const QRectF& rect) const;
	QRectF circumRect(const QRectF& rect) const;
	QRectF circumRect(const QSizeF& size) const;
};

#endif // _BORDERRECTF_H
