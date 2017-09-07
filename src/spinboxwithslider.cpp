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

#include <QGridLayout>

#include <memory>

#include <spinboxwithslider.h>

SpinboxWithSlider::SpinboxWithSlider(Qt::Orientation orientation, QWidget* parent):
	QWidget(parent),
	spinbox_(new ScientificSpinBox(this)),
	slider_(new QSlider(orientation, this)) {

	slider_->setRange(0, slider_range_);

	std::unique_ptr<QGridLayout> widget_layout{new QGridLayout(this)};
	widget_layout->addWidget(slider_, 0, 0);
	if (orientation == Qt::Horizontal) {
		widget_layout->addWidget(spinbox_, 0, 1);
		widget_layout->setRowStretch(1, 1);
	} else {
		widget_layout->addWidget(spinbox_, 1, 0);
		widget_layout->setColumnStretch(1, 1);
	}
	setLayout(widget_layout.release());

	connect(spinbox_, SIGNAL(valueChanged(double)), this, SLOT(notifySpinboxValueChanged(double)));
	connect(spinbox_, SIGNAL(rangeChanged(double, double)), this, SLOT(notifySpinboxRangeChanged(double, double)));
	connect(slider_, SIGNAL(valueChanged(int)), this, SLOT(notifySliderValueChanged(int)));
}

int SpinboxWithSlider::spinboxValueToSlider(double value) const {
	return qRound((value - spinbox_->minimum()) / (spinbox_->maximum() - spinbox_->minimum())
		   * (slider_->maximum() - slider_->minimum()))
		   + slider_->minimum();
}

void SpinboxWithSlider::notifySliderValueChanged(int value) {
	if (spinboxValueToSlider(spinbox_->value()) != value) {
		const double spinbox_value =
				static_cast<double>(value - slider_->minimum()) / (slider_->maximum() - slider_->minimum())
				* (spinbox_->maximum() - spinbox_->minimum())
				+ spinbox_->minimum();
		spinbox_->setValue(spinbox_value);
	}
}

void SpinboxWithSlider::notifySpinboxValueChanged(double value) {
	slider_->setValue(spinboxValueToSlider(value));
}

void SpinboxWithSlider::notifySpinboxRangeChanged(double min, double max) {
	notifySpinboxValueChanged(spinbox_->value());
}
