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

#ifndef _VIEWRECT_H
#define _VIEWRECT_H

#include <QObject>
#include <QRect>
#include <QRectF>
#include <QSizeF>

class Viewrect: public QObject {
	Q_OBJECT
private:
	QRectF border_;
	QSize  vsize_;
	QPoint vpos_;
	float  scale_;

public:
	Viewrect();

	inline const QRectF& border() const { return border_; }
	void fitToBorder(const QSize& widget);
	inline const QSize&  virtualSize() const { return vsize_; }
	inline const QPoint& virtualPos() const { return vpos_; }
	inline const float&  scale() const { return scale_; }

public slots:
	void setVirtualPos(const QPoint& pos);
	void setHorizontalVirtualPos(int hpos);
	void setVerticalVirtualPos(int vpos);
	void setBorder(const QRectF& border_rect);
	void setScale(float scale);
	void zoom(double zoom_factor);

signals:
	void borderChanged(const QRectF& border_rect);
	void virtualSizeChanged(const QSize& vsize);
	void virtualPosChanged(const QPoint& vpos);
	void scaleChanged(float scale);

private:
	void resetVirtualSize();
};

#endif //_VIEWRECT_H
