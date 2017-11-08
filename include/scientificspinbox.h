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

#ifndef _SCIENTIFICSPINBOX_H
#define _SCIENTIFICSPINBOX_H

#include <QDoubleSpinBox>

class ScientificSpinBox: public QDoubleSpinBox {
	Q_OBJECT
private:
	static constexpr const char text_format_ = 'g';
	static constexpr const int log10_steps_in_range_ = 2;

public:
	ScientificSpinBox(QWidget* parent=Q_NULLPTR, int decimals=5);

	virtual QString textFromValue(double value) const override;
	virtual double valueFromText(const QString& text) const override;
	virtual QValidator::State validate(QString& text, int& pos) const override;
	inline void setMaximum(double max) { setRange(minimum(), max); }
	inline void setMinimum(double min) { setRange(min, maximum()); }
	void setRange(double min, double max);

signals:
	void rangeChanged(double min, double max);
};

#endif //_SCIENTIFICSPINBOX_H
