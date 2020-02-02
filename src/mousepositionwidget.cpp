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

#include <QMouseEvent>

#include <mousepositionwidget.h>
#include <openglwidget.h>

MousePositionWidget::MousePositionWidget(QWidget *parent):
	QWidget(parent),
	x_position_(new QLabel(this)),
	y_position_(new QLabel(this)),
	value_(new QLabel(this)) {

	x_position_->setAlignment(Qt::AlignRight);
	y_position_->setAlignment(Qt::AlignRight);
	value_->setAlignment(Qt::AlignRight);

	const auto char_width = fontMetrics().averageCharWidth();
	const auto position_text_width = (max_number_of_digits_image_size_+1) * char_width; // +1 for sign
	x_position_->setFixedWidth(position_text_width);
	y_position_->setFixedWidth(position_text_width);
	// +1 for number sign, +1 for decimal point, +2 for exponent sign and +/- after it, +3 for exponent digits
	const auto value_text_width = (decimals_ + 6) * char_width;
	value_->setFixedWidth(value_text_width);

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

MousePositionWidget::MouseMoveEventFilter::MouseMoveEventFilter(MousePositionWidget *mouse_position_widget, QObject* parent):
	QObject(parent),
	mouse_position_widget_(mouse_position_widget) {
}

bool MousePositionWidget::MouseMoveEventFilter::eventFilter(QObject* object, QEvent* event) {
	auto watched = static_cast<OpenGLWidget*>(object);
	switch (event->type()) {
		case QEvent::MouseMove: {
			const auto mouse_event = static_cast<QMouseEvent*>(event);
			mouse_position_widget_->setPositionAndValue(watched->pixelFromWidgetCoordinate(mouse_event->pos()));
			return true;
		}
		default:
			return false;
	}
}

constexpr const int MousePositionWidget::max_number_of_digits_image_size_;
constexpr const char MousePositionWidget::text_format_;
constexpr const int MousePositionWidget::decimals_;
