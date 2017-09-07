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
	QRectF view_;
	QRect  scroll_;
	QRectF border_;
	static constexpr int scroll_range_ = 10000;

public:
	Viewrect();
	explicit Viewrect(const QRectF& view_rect,   const QRectF& border_rect);
	explicit Viewrect(const QRect&  scroll_rect, const QRectF& border_rect);

	inline const QRectF& view()   const { return view_;   }
	inline const QRect&  scroll() const { return scroll_; }
	inline QRectF openGLprojection() const { return {view_.left(), -view_.top(), view_.width(), -view_.height()}; }
	inline const int scrollRange() const { return scroll_range_; }  // scroll range should be set to [0, scroll_range_]
	void fitToBorder(QSizeF ratio);

public slots:
	void setView(const QRectF& view_rect);
	void setScroll(const QRect& scroll_rect);
	void setBorder(const QRectF& border_rect);

signals:
	void viewChanged(const QRectF& view_rect);
	void scrollChanged(const QRect& scroll_rect);
	void borderChanged(const QRectF& border_rect);

protected:
	QRectF alignView(const QRectF& view_rect) const;
	QRectF scrollToView(const QRect& scroll_rect) const;
	QRect viewToScroll(const QRectF& view_rect) const;
};

#endif //_VIEWRECT_H
