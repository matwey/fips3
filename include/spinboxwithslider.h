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

#ifndef _SPINBOXWITHSLIDER_H
#define _SPINBOXWITHSLIDER_H

#include <QWidget>
#include <QSlider>

#include <scientificspinbox.h>

class SpinboxWithSlider: public QWidget {
	Q_OBJECT
private:
	static constexpr int slider_range_ = 10000;
	ScientificSpinBox* spinbox_;
	QSlider* slider_;

public:
	SpinboxWithSlider(Qt::Orientation orientation, QWidget* parent=Q_NULLPTR);

	inline ScientificSpinBox* spinbox() const { return spinbox_; }

protected:
	int spinboxValueToSlider(double value) const;

private slots:
	void notifySliderValueChanged(int value);
	void notifySpinboxValueChanged(double value);
	void notifySpinboxRangeChanged(double min, double max);
};

#endif //_SPINBOXWITHSLIDER_H
