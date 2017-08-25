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

#include <mousepositionwidget.h>

MousePositionWidget::MousePositionWidget(QWidget *parent):
	QWidget(parent),
	x_position_(new QLabel(this)),
	y_position_(new QLabel(this)),
	value_(new QLabel(this)) {

	std::unique_ptr<QHBoxLayout> widget_layout{new QHBoxLayout(this)};

	auto x_label = new QLabel(tr("x:"), this);
	widget_layout->addWidget(x_label);
	widget_layout->addWidget(x_position_);

	auto y_label = new QLabel(tr("y:"), this);
	widget_layout->addWidget(y_label);
	widget_layout->addWidget(y_position_);

	auto value_label = new QLabel(tr("value:"), this);
	widget_layout->addWidget(value_label);
	widget_layout->addWidget(value_);

	setLayout(widget_layout.release());
}

void MousePositionWidget::setPositionAndValue(const Pixel& pixel) {
	if (pixel.value) {
		x_position_->setText(locale().toString(pixel.position.x()));
		y_position_->setText(locale().toString(pixel.position.y()));
		value_->setText(locale().toString(pixel.value.value(), text_format_, decimals_));
	}
}

constexpr const char MousePositionWidget::text_format_;
constexpr const int MousePositionWidget::decimals_;
