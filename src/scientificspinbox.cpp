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

#include <QDebug>
#include <QFontDatabase>
#include <QLineEdit>

#include <scientificspinbox.h>

constexpr const char ScientificSpinBox::text_format_;
constexpr const int ScientificSpinBox::log10_steps_in_range_;

ScientificSpinBox::ScientificSpinBox(QWidget* parent, int decimals):
	QAbstractSpinBox(parent),
	decimals_(decimals) {
	const auto font = QFontDatabase::systemFont(QFontDatabase::GeneralFont);
	const auto family = font.family();
}

QSize ScientificSpinBox::sizeHint() const {
	const auto char_width = fontMetrics().averageCharWidth();
	// +1 for number sign, +1 for integer part, +1 for decimal point, + decimals() for decimals,
	// +2 for exponent sign and +/- after it, +3 for exponent digits
	const auto width = (decimals() + 8) * char_width;
	const auto height = lineEdit()->height();
	return {width, height};
}

QSize ScientificSpinBox::minimumSizeHint() const {
	return sizeHint();
}

void ScientificSpinBox::setValue(double value) {
	const auto new_text = textFromValue(value);
	if (new_text == text()) return;
	value_ = valueFromText(new_text);
	lineEdit()->setText(new_text);
	emit valueChanged(value_);
	emit valueChanged(new_text);
}

void ScientificSpinBox::stepBy(int steps) {
	auto new_value = value() + steps * single_step_;
	if (new_value > maximum()){
		new_value = maximum();
	}
	if (new_value < minimum()){
		new_value = minimum();
	}
	setValue(new_value);
}

QAbstractSpinBox::StepEnabled ScientificSpinBox::stepEnabled() const {
	QAbstractSpinBox::StepEnabled se = QAbstractSpinBox::StepNone;
	if (value() > minimum()){
		se |= QAbstractSpinBox::StepDownEnabled;
	}
	if (value() < maximum()){
		se |= QAbstractSpinBox::StepUpEnabled;
	}
	return se;
}

QString ScientificSpinBox::textFromValue(double value) const {
	return locale().toString(value, text_format_, decimals_);
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

void ScientificSpinBox::fixup(QString &str) const {
	str.remove(locale().groupSeparator());
}

void ScientificSpinBox::setRange(double min, double max) {
	Q_ASSERT(min < max);
	minimum_ = min;
	maximum_ = max;
	// This is the maximum value of the form 1eN that smaller than (max - min) / 10**log10_steps_in_range_
	single_step_ = std::pow(10.0, std::floor(std::log10(max - min)) - log10_steps_in_range_);
	emit rangeChanged(min, max);
}
