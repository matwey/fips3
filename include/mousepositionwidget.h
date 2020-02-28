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

#ifndef _MOUSEPOSITIONWIDGET_H
#define _MOUSEPOSITIONWIDGET_H

#include <QEvent>
#include <QHBoxLayout>
#include <QLabel>
#include <QLocale>
#include <QPoint>
#include <QWidget>

#include <algorithm>
#include <memory>

#include <pixel.h>

class MousePositionWidget:
	public QWidget {
private:
	static constexpr const int max_number_of_digits_image_size_ = 5;
	static constexpr const char text_format_ = 'g';
	static constexpr const int decimals_ = 5;

	QLabel* x_position_;
	QLabel* y_position_;
	QLabel* value_;
public:
	MousePositionWidget(QWidget* parent = Q_NULLPTR);

	void setPositionAndValue(const Pixel &pixel);

	class MouseMoveEventFilter:
		public QObject {
	private:
		MousePositionWidget* mouse_position_widget_;
	public:
		MouseMoveEventFilter(MousePositionWidget* mouse_position_widget, QObject* parent = Q_NULLPTR);
	protected:
		virtual bool eventFilter(QObject* object, QEvent* event) override;
	};
};

#endif //_MOUSEPOSITIONWIDGET_H
