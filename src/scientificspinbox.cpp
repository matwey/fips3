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

#include <cmath>

#include <scientificspinbox.h>

constexpr const char ScientificSpinBox::text_format_;
constexpr const int ScientificSpinBox::log10_steps_in_range_;

ScientificSpinBox::ScientificSpinBox(QWidget* parent, int decimals): QDoubleSpinBox(parent) {
	setDecimals(decimals);
}

QString ScientificSpinBox::textFromValue(double value) const {
	return locale().toString(value, text_format_, decimals());
}

double ScientificSpinBox::valueFromText(const QString& text) const {
	bool ok = false;
	const auto value = locale().toDouble(text, &ok);
	Q_ASSERT(ok);
	return value;
}

QValidator::State ScientificSpinBox::validate(QString& text, int&) const {
	const auto decimal_point = QString(locale().decimalPoint());
	const auto e = QString(locale().exponential());
	const auto minus = QString(locale().negativeSign());
	const auto plus = QString(locale().positiveSign());
	bool ok = false;
	locale().toDouble(text, &ok);
	if (ok) {
		return QValidator::Acceptable;
	}
	if (text.isEmpty()) {
		return QValidator::Intermediate;
	}
	if (text == minus || text == "-") {
		if (minimum() < 0) {
			return QValidator::Intermediate;
		}
		return QValidator::Invalid;
	}
	if (text == decimal_point) {
		return QValidator::Intermediate;
	}
	if (text.startsWith(e, Qt::CaseInsensitive)) {
		return QValidator::Invalid;
	}
	if (text.endsWith(e, Qt::CaseInsensitive) || text.endsWith(e + plus, Qt::CaseInsensitive) ||
			text.endsWith(e + minus, Qt::CaseInsensitive) || text.endsWith(e + "-", Qt::CaseInsensitive)) {
		return QValidator::Intermediate;
	}
	if (text.contains(decimal_point + e, Qt::CaseInsensitive)) {
		return QValidator::Intermediate;
	}
	return QValidator::Invalid;
}

void ScientificSpinBox::setRange(double min, double max) {
	QDoubleSpinBox::setRange(min, max);
	// This is the maximum value of the form 1eN that smaller than (max - min) / 10**log10_steps_in_range_
	const auto step_size = std::pow(10.0, std::floor(std::log10(max - min)) - log10_steps_in_range_);
	setSingleStep(step_size);
	emit rangeChanged(min, max);
}
