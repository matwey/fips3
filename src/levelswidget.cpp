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

#include <levelswidget.h>

ScientificSpinBox::ScientificSpinBox(QWidget *parent, int decimals): QDoubleSpinBox(parent) {
	setDecimals(decimals);
}

QString ScientificSpinBox::textFromValue(double value) const {
	return locale().toString(value, text_format_, decimals());
}

double ScientificSpinBox::valueFromText(const QString &text) const {
	bool ok = false;
	const auto value = locale().toDouble(text, &ok);
	Q_ASSERT(ok);
	return value;
}

QValidator::State ScientificSpinBox::validate(QString &text, int &pos) const {
	const auto decimal_point = locale().decimalPoint();
	bool ok = false;
	locale().toDouble(text, &ok);
	if (ok) {
		return QValidator::Acceptable;
	}
	if (text.isEmpty()) {
		return QValidator::Intermediate;
	}
	if (text == "-") {
		if (minimum() < 0) {
			return QValidator::Intermediate;
		}
		return QValidator::Invalid;
	}
	if (text == decimal_point) {
		return QValidator::Intermediate;
	}
	if (text.startsWith("e", Qt::CaseInsensitive)) {
		return QValidator::Invalid;
	}
	if (text.endsWith("e", Qt::CaseInsensitive) || text.endsWith("e+", Qt::CaseInsensitive) || text.endsWith("e-", Qt::CaseInsensitive)) {
		return QValidator::Intermediate;
	}
	if (text.contains(decimal_point + 'e', Qt::CaseInsensitive)) {
		return QValidator::Intermediate;
	}
	return QValidator::Invalid;
}

void ScientificSpinBox::setRange(double min, double max) {
	QDoubleSpinBox::setRange(min, max);
	// This is the maximum value of the form 1eN that smaller than (max - min) / 10**log10_steps_in_range_
	const auto step_size = std::pow(10.0, std::floor(std::log10(max - min)) - log10_steps_in_range_);
	setSingleStep(step_size);
}

constexpr const char ScientificSpinBox::text_format_;
constexpr const int ScientificSpinBox::log10_steps_in_range_;


SpinboxWithSlider::SpinboxWithSlider(Qt::Orientation orientation, QWidget *parent):
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
	connect(slider_, SIGNAL(valueChanged(int)), this, SLOT(notifySliderValueChanged(int)));
}

int SpinboxWithSlider::spinboxValueToSlider(double value) const {
	return qRound((value - spinbox_->minimum()) / (spinbox_->maximum() - spinbox_->minimum())
		   * (slider_->maximum() - slider_->minimum()))
		   + slider_->minimum();
}

void SpinboxWithSlider::notifySpinboxValueChanged(double value) {
	slider_->setValue(spinboxValueToSlider(value));
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


LevelsWidget::LevelsWidget(QWidget* parent):
			QWidget(parent),
			min_level_(new SpinboxWithSlider(Qt::Horizontal, this)),
			max_level_(new SpinboxWithSlider(Qt::Horizontal, this)) {
	connect(min_level_->spinbox(), SIGNAL(valueChanged(double)), this, SLOT(notifyMinValueChanged(double)));
	connect(max_level_->spinbox(), SIGNAL(valueChanged(double)), this, SLOT(notifyMaxValueChanged(double)));

	std::unique_ptr<QGridLayout> widget_layout{new QGridLayout(this)};
	widget_layout->addWidget(new QLabel(tr("Min"), this), 0, 0);
	widget_layout->addWidget(min_level_, 0, 1);
	widget_layout->addWidget(new QLabel(tr("Max"), this), 1, 0);
	widget_layout->addWidget(max_level_, 1, 1);
	widget_layout->setRowStretch(2, 1);
	setLayout(widget_layout.release());
}

void LevelsWidget::setRange(double minimum, double maximum) {
	min_level_->spinbox()->setRange(minimum, maximum);
	max_level_->spinbox()->setRange(minimum, maximum);
	setValues(minimum, maximum);
}

void LevelsWidget::setValues(double minimum, double maximum) {
	min_level_->spinbox()->setValue(minimum);
	max_level_->spinbox()->setValue(maximum);
}

void LevelsWidget::notifyTextureInitialized(const OpenGLTexture *texture) {
	setRange(texture->instrumental_minmax());
	setValues(texture->hdu_minmax());
}
