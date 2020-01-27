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

#include <limits>

#include <viewrect.h>

Viewrect::Viewrect():
	view_(-1, -1, 2, 2),
	border_(-1, -1, 2, 2),
	widget_(1, 1),
	vsize_(1, 1),
	vpos_(0, 0),
	scale_(1) {
};

void Viewrect::setViewOrigin(const QPointF& origin) {
	if (view_.topLeft() == origin) return;

	const QRectF new_view{origin, view_.size()};

	setView(new_view);
}

void Viewrect::setVirtualPos(const QPoint& pos) {
	if (vpos_ == pos) return;

	const auto wb = static_cast<float>(border_.width());
	const auto hb = static_cast<float>(border_.height());

	const QPointF vpos = pos;
	const QPointF new_origin{
		static_cast<float>(-0.5) * wb - vpos.x() / scale_,
		static_cast<float>(-0.5) * hb - vpos.y() / scale_};

	setViewOrigin(new_origin);
}

void Viewrect::setHorizontalVirtualPos(int hpos) {
	const QPoint new_vpos{hpos, vpos_.y()};

	setVirtualPos(new_vpos);
}

void Viewrect::setVerticalVirtualPos(int vpos) {
	const QPoint new_vpos{vpos_.x(), vpos};

	setVirtualPos(new_vpos);
}

void Viewrect::resetViewSize() {
	const auto w = static_cast<float>(widget_.width());
	const auto h = static_cast<float>(widget_.height());
	const auto scale_x = w / scale_;
	const auto scale_y = h / scale_;
	const QSizeF new_size{scale_x, scale_y};
	const QRectF new_view{view_.topLeft(), new_size};

	setView(new_view);
}

void Viewrect::resetVirtualSize() {
	const auto wb = static_cast<float>(border_.width());
	const auto hb = static_cast<float>(border_.height());
	const QSize new_size {
		static_cast<int>(scale_ * wb),
		static_cast<int>(scale_ * hb)};

	if (vsize_ == new_size) return;

	vsize_ = new_size;
	emit virtualSizeChanged(vsize_);
}

void Viewrect::resetVirtualPos() {
	const auto wb = static_cast<float>(border_.width());
	const auto hb = static_cast<float>(border_.height());
	const QPoint new_pos {
		-static_cast<int>(scale_ * (wb / 2 + view_.left())),
		-static_cast<int>(scale_ * (hb / 2 + view_.top()))};

	if (vpos_ == new_pos) return;

	vpos_ = new_pos;
	emit virtualPosChanged(vpos_);
}

void Viewrect::setView(const QRectF& view_rect) {
	if (view_ == view_rect) return;

	const QRectF new_rect = alignView(view_rect);

	if (view_ == new_rect) return;

	view_ = new_rect;

	resetVirtualSize();
	resetVirtualPos();
}

void Viewrect::setWidget(const QSize& widget_size) {
	if (widget_ == widget_size) return;

	widget_ = widget_size;

	resetViewSize();
}

void Viewrect::setScale(float scale) {
	const auto wb = static_cast<float>(border_.width());
	const auto hb = static_cast<float>(border_.height());
	const auto lo_scale = static_cast<float>(1) / std::min(wb, hb);
	const auto hi_scale = static_cast<float>(std::numeric_limits<int>::max()) / std::max(wb, hb);

	scale = std::min(std::max(lo_scale, scale), hi_scale);

	if (scale_ == scale) return;

	scale_ = scale;

	emit scaleChanged(scale_);
}

void Viewrect::zoom(double zoom_factor) {
	setScale(zoom_factor * scale());

	resetViewSize();
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

void Viewrect::fitToBorder() {
	const auto w = static_cast<float>(widget_.width());
	const auto h = static_cast<float>(widget_.height());
	const auto wb = static_cast<float>(border_.width());
	const auto hb = static_cast<float>(border_.height());
	const auto hscale = w / wb;
	const auto vscale = h / hb;

	setScale(std::min(hscale, vscale));

	resetViewSize();
}

void Viewrect::setBorder(const QRectF &border_rect) {
	if (border_ == border_rect) return;

	border_ = border_rect;

	emit borderChanged(border_);

	resetVirtualSize();
	resetVirtualPos();
}
