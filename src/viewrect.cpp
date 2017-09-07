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

#include <viewrect.h>

Viewrect::Viewrect(): Viewrect(QRectF(-1,-1,2,2), QRectF(-1,-1,2,2)) {}

Viewrect::Viewrect(const QRectF& view_rect, const QRectF& border_rect) {
	setBorder(border_rect);
	setView(view_rect);
}

Viewrect::Viewrect(const QRect& scroll_rect, const QRectF& border_rect) {
	setBorder(border_rect);
	setScroll(scroll_rect);
}

void Viewrect::setView(const QRectF& view_rect) {
	if (view_ == view_rect) return;

	const QRectF new_rect = alignView(view_rect);

	if (view_ == new_rect) return;

	view_ = new_rect;
	emit viewChanged(view_);

	setScroll(viewToScroll(view_));
}

void Viewrect::setScroll(const QRect& scroll_rect) {
	if (scroll_ == scroll_rect) return;

	scroll_ = scroll_rect;
	emit scrollChanged(scroll_);

	setView(scrollToView(scroll_));
}

QRectF Viewrect::alignView(const QRectF& view_rect) const {
	QRectF new_view(view_rect);

	if (new_view.size().width() > border_.width()) {
		new_view.moveCenter({border_.center().x(), new_view.center().y()});
	} else {
		if (new_view.left() < border_.left()) {
			new_view.moveLeft(border_.left());
		}
		if (new_view.right() > border_.right()) {
			new_view.moveRight(border_.right());
		}
	}

	if (new_view.size().height() > border_.height()) {
		new_view.moveCenter({new_view.center().x(), border_.center().y()});
	} else {
		if (new_view.top() < border_.top()) {
			new_view.moveTop(border_.top());
		}
		if (new_view.bottom() > border_.bottom()) {
			new_view.moveBottom(border_.bottom());
		}
	}

	return new_view;
}

QRect Viewrect::viewToScroll(const QRectF& view_rect) const {
	const auto left =   qRound((view_rect.left() - border_.left()) / border_.width()  * scroll_range_);
	const auto top  =   qRound((view_rect.top()  - border_.top())  / border_.height() * scroll_range_);
	const auto width =  qRound(view_rect.width()  / border_.width()  * (scroll_range_ + 1));
	const auto height = qRound(view_rect.height() / border_.height() * (scroll_range_ + 1));
	return QRect{left, top, width, height};
}

QRectF Viewrect::scrollToView(const QRect& scroll_rect) const {
	const auto left   = scroll_rect.left()   * border_.width()  / scroll_range_ + border_.left();
	const auto top    = scroll_rect.top()    * border_.height() / scroll_range_ + border_.top();
	const auto width  = scroll_rect.width()  * border_.width()  / (scroll_range_ + 1);
	const auto height = scroll_rect.height() * border_.height() / (scroll_range_ + 1);
	return QRectF{left, top, width, height};
}

void Viewrect::fitToBorder(QSizeF ratio) {
	ratio.scale(border_.size(), Qt::KeepAspectRatioByExpanding);
	// It will be aligned in align(), so top left corner value doesn't matter
	const QRectF new_view(QPointF(0, 0), ratio);
	setView(new_view);
}

void Viewrect::setBorder(const QRectF &border_rect) {
	if (border_ == border_rect) return;

	border_ = border_rect;

	emit borderChanged(border_);

	setView(view_);
}
