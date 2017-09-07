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

Viewrect::Viewrect(const QRectF& view_rect, const QRectF& border_rect): border_(border_rect) {
	set(view_rect);
}

Viewrect::Viewrect(const QRect& scroll_rect, const QRectF& border_rect): border_(border_rect) {
	set(scroll_rect);
}

void Viewrect::set(const QRectF& view_rect) {
	view_ = view_rect;
	align();
	const QRect old_scroll_rect(scroll_);
	scroll_ = viewToScroll(view_);
	if (scroll_ != old_scroll_rect) {
		emit scrollRectChanged(scroll_);
	}
}

void Viewrect::set(const QRect& scroll_rect) {
	const auto left   = scroll_rect.left()   * border_.width()  / scroll_range_ + border_.left();
	const auto top    = scroll_rect.top()    * border_.height() / scroll_range_ + border_.top();
	const auto width  = scroll_rect.width()  * border_.width()  / (scroll_range_ + 1);
	const auto height = scroll_rect.height() * border_.height() / (scroll_range_ + 1);
	const QRectF new_view{left, top, width, height};
	set(new_view);
}

void Viewrect::align() {
	QRectF new_view(view_);
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
	if (new_view != view_) {
		set(new_view);
	}
}

QRect Viewrect::viewToScroll(const QRectF& view_rect) const {
	const int left =   qRound((view_.left() - border_.left()) / border_.width()  * scroll_range_);
	const int top  =   qRound((view_.top()  - border_.top())  / border_.height() * scroll_range_);
	const int width =  qRound(view_.width()  / border_.width()  * (scroll_range_ + 1));
	const int height = qRound(view_.height() / border_.height() * (scroll_range_ + 1));
	return {left, top, width, height};
}

void Viewrect::fitToBorder(QSizeF ratio) {
	ratio.scale(border_.size(), Qt::KeepAspectRatioByExpanding);
	// It will be aligned in align(), so top left corner value doesn't matter
	const QRectF new_view(QPointF(0, 0), ratio);
	set(new_view);
}

void Viewrect::setBorder(const QRectF &border_rect) {
	if (border_ != border_rect) {
		border_ = border_rect;
		set(view_);
	}
}