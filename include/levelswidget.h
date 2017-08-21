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

#include <QDebug>
#include <QDoubleSpinBox>
#include <QGridLayout>
#include <QLabel>
#include <QResizeEvent>
#include <QSlider>
#include <QWidget>

#include <cmath>
#include <memory>

#include <opengltexture.h>

class ScientificSpinBox: public QDoubleSpinBox {
private:
	const char text_format_ = 'g';
public:
	ScientificSpinBox(QWidget* parent=Q_NULLPTR, int decimals=5);

	virtual QString textFromValue(double value) const override;
	virtual double valueFromText(const QString& text) const override;
	virtual QValidator::State validate(QString& text, int& pos) const override;
	virtual void stepBy(int steps) override;
};

class SpinboxWithSlider: public QWidget {
	Q_OBJECT
private:
	static constexpr int slider_range_ = 10000;
	std::unique_ptr<ScientificSpinBox> spinbox_;
	std::unique_ptr<QSlider> slider_;

public:
	SpinboxWithSlider(Qt::Orientation orientation, QWidget* parent=Q_NULLPTR);

	inline ScientificSpinBox* spinbox() const { return spinbox_.get(); }
	int spinboxValueToSlider(double value) const;

private slots:
	void notifySliderValueChanged(int value);
	void notifySpinboxValueChanged(double value);
};


class LevelsWidget: public QWidget {
	Q_OBJECT
private:
	std::unique_ptr<SpinboxWithSlider> min_level_, max_level_;

public:
	explicit LevelsWidget(QWidget* parent);

signals:
	void valuesChanged(const std::pair<double, double>& minmax);

public slots:
	void setRange(double minimum, double maximum);
	inline void setRange(const std::pair<double, double>& minmax) { setRange(minmax.first, minmax.second); }
	void setValues(double minimum, double maximum);
	inline void setValues(const std::pair<double, double>& minmax) { setValues(minmax.first, minmax.second); }
	void notifyTextureInitialized(const OpenGLTexture* texture);

private slots:
	inline void notifyValuesChanged(const std::pair<double, double>& minmax) { emit valuesChanged(minmax); }
	inline void notifyMinValueChanged(double min) { notifyValuesChanged(std::make_pair(min, max_level_->spinbox()->value())); }
	inline void notifyMaxValueChanged(double max) { notifyValuesChanged(std::make_pair(min_level_->spinbox()->value(), max)); }
};


#endif //_LEVELSWIDGET_H
