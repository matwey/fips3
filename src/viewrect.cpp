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
	border_(-1, -1, 2, 2),
	vsize_(1, 1),
	vpos_(0, 0),
	scale_(1) {
};

void Viewrect::setVirtualPos(const QPoint& pos) {
	if (vpos_ == pos) return;

	vpos_ = pos;

	emit virtualPosChanged(vpos_);
}

void Viewrect::setHorizontalVirtualPos(int hpos) {
	const QPoint new_vpos{hpos, vpos_.y()};

	setVirtualPos(new_vpos);
}

void Viewrect::setVerticalVirtualPos(int vpos) {
	const QPoint new_vpos{vpos_.x(), vpos};

	setVirtualPos(new_vpos);
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

void Viewrect::setScale(float scale) {
	const auto wb = static_cast<float>(border_.width());
	const auto hb = static_cast<float>(border_.height());
	const auto lo_scale = static_cast<float>(1) / std::min(wb, hb);
	const auto hi_scale = static_cast<float>(std::numeric_limits<int>::max()) / std::max(wb, hb);

	scale = std::min(std::max(lo_scale, scale), hi_scale);

	if (scale_ == scale) return;

	scale_ = scale;

	emit scaleChanged(scale_);

	resetVirtualSize();
}

void Viewrect::zoom(double zoom_factor) {
	setScale(zoom_factor * scale());
}

void Viewrect::fitToBorder(const QSize& widget) {
	const auto w = static_cast<float>(widget.width());
	const auto h = static_cast<float>(widget.height());
	const auto wb = static_cast<float>(border_.width());
	const auto hb = static_cast<float>(border_.height());
	const auto hscale = w / wb;
	const auto vscale = h / hb;

	setScale(std::min(hscale, vscale));
}

void Viewrect::setBorder(const QRectF &border_rect) {
	if (border_ == border_rect) return;

	border_ = border_rect;

	emit borderChanged(border_);

	resetVirtualSize();
}
