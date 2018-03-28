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

#ifndef _LEVELSWIDGET_H
#define _LEVELSWIDGET_H

#include <QCheckBox>
#include <QWidget>

#include <memory>

#include <opengltexture.h>
#include <scientificspinbox.h>
#include <spinboxwithslider.h>


class LevelsWidget: public QWidget {
	Q_OBJECT
private:
	std::unique_ptr<SpinboxWithSlider> min_level_;
	std::unique_ptr<SpinboxWithSlider> max_level_;
	std::unique_ptr<QCheckBox> range_check_box_;
	std::pair<double, double> instrumental_minmax_;
	std::pair<double, double> image_minmax_;

public:
	explicit LevelsWidget(QWidget* parent);

signals:
	void valuesChanged(const std::pair<double, double>& minmax);

public slots:
	void setRange(double minimum, double maximum);
	inline void setRange(const std::pair<double, double>& minmax) { setRange(minmax.first, minmax.second); }
	void setValues(double minimum, double maximum);
	inline void setValues(const std::pair<double, double>& minmax) { setValues(minmax.first, minmax.second); }
	void notifyTextureInitialized(const AbstractOpenGLTexture& texture);

private slots:
	inline void notifyValuesChanged(const std::pair<double, double>& minmax) { emit valuesChanged(minmax); }
	inline void notifyMinValueChanged(double min) { notifyValuesChanged(std::make_pair(min, max_level_->spinbox()->value())); }
	inline void notifyMaxValueChanged(double max) { notifyValuesChanged(std::make_pair(min_level_->spinbox()->value(), max)); }
	inline void setValuesToImageMinMax() { setValues(image_minmax_); }
	void notifyRangeCheckboxStateChanged(int state);
	inline void setRangeToImageMinMax() { setRange(image_minmax_); }
	inline void setRangeToInstrumentalMinMax() { setRange(instrumental_minmax_); }
};


#endif //_LEVELSWIDGET_H
