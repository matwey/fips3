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

#include <QAbstractSpinBox>
#include <QString>

class ScientificSpinBox: public QAbstractSpinBox {
	Q_OBJECT
private:
	static constexpr const char text_format_ = 'g';
	static constexpr const int log10_steps_in_range_ = 2;
	double value_ = 0;
	const int decimals_;
	double minimum_ = 0;
	double maximum_ = 1;
	double single_step_ = 0.01;

protected:
	virtual QSize sizeHint() const override;
	virtual QSize minimumSizeHint() const override;
	virtual void stepBy(int steps) override;
	virtual QAbstractSpinBox::StepEnabled stepEnabled() const override;
	virtual QValidator::State validate(QString& text, int& pos) const override;
	virtual void fixup(QString &str) const override;
	QString textFromValue(double value) const;
	double valueFromText(const QString& text) const;

public:
	ScientificSpinBox(QWidget* parent=Q_NULLPTR, int decimals=5);

	inline double value() const { return value_; }
	inline int decimals() const { return decimals_; }
	inline double minimum() const { return minimum_; }
	inline double maximum() const { return maximum_; }

public slots:
	void setValue(double value);
	inline void setMinimum(double min) { setRange(min, maximum_); }
	inline void setMaximum(double max) { setRange(minimum_, max); }
	void setRange(double min, double max);

signals:
	void rangeChanged(double min, double max);
	void valueChanged(double value);
	void valueChanged(const QString &value);
};

#endif //_SCIENTIFICSPINBOX_H
