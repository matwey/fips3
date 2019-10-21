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

#include <borderrectf.h>

BorderRectF::BorderRectF(const QRectF& rectangle):
	QRectF(rectangle) {
}

BorderRectF::BorderRectF(qreal x, qreal y, qreal width, qreal height):
	QRectF(x, y, width, height) {
}

BorderRectF::BorderRectF(const QPointF& topLeft, const QPointF& bottomRight):
	QRectF(topLeft, bottomRight) {
}

BorderRectF::BorderRectF(const QPointF& topLeft, const QSizeF& size):
	QRectF(topLeft, size) {
}

QRectF BorderRectF::alignRect(const QRectF& rect) const {
	QRectF new_rect{rect};

	if (new_rect.size().width() > width()) {
		new_rect.moveCenter({center().x(), new_rect.center().y()});
	} else if (new_rect.left() < left()) {
		new_rect.moveLeft(left());
	} else if (new_rect.right() > right()) {
		new_rect.moveRight(right());
	}

	if (new_rect.size().height() > height()) {
		new_rect.moveCenter({new_rect.center().x(), center().y()});
	} else if (new_rect.top() < top()) {
		new_rect.moveTop(top());
	} else if (new_rect.bottom() > bottom()) {
		new_rect.moveBottom(bottom());
	}

	return new_rect;
}

QRectF BorderRectF::circumRect(const QRectF& rect) const {
	return circumRect(rect.size());
}

QRectF BorderRectF::circumRect(const QSizeF& size) const {
	QSizeF new_size{size};
	new_size.scale(this->size(), Qt::KeepAspectRatioByExpanding);

	QRectF new_rect{0.0, 0.0, new_size.width(), new_size.height()};
	new_rect.moveCenter(center());

	return new_rect;
}
