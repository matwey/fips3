#ifndef _LEVELSWIDGET_H
#define _LEVELSWIDGET_H

#include <QDebug>
#include <QDoubleSpinBox>
#include <QGridLayout>
#include <QLabel>
#include <QResizeEvent>
#include <QSlider>
#include <QStyle>
#include <QWidget>

#include <memory>

#include <opengltexture.h>

class DoubleSlider: public QSlider {
	Q_OBJECT

private:
	double double_minimum_, double_maximum_;
	double double_value_;

	// Protect public methods from parent class
	int minimum() const;
	int maximum() const;
	int pageStep() const;
	int singleStep() const;
	void scroll(int, int);
	void scroll(int, int, const QRect&);
	void setMinimum(int);
	void setMaximum(int);
	void setPageStep(int);
	void setRange(int, int);
	void setSingleStep(int);
	void setSliderPosition(int);
	void setValue(int);
	int value() const;

protected:
	void resizeEvent(QResizeEvent* event) override;

public:
	DoubleSlider(Qt::Orientation orientation, QWidget* parent=Q_NULLPTR, double minimum=0, double maximum=1);

	void setRange(double min, double max);

signals:
	void valueChanged(double value);

public slots:
	void setValue(double value);

private slots:
	void notifyValueChanged(int value);
};


class LevelsWidget: public QWidget {
	Q_OBJECT
private:
	std::unique_ptr<DoubleSlider> min_slider_, max_slider_;
	std::unique_ptr<QDoubleSpinBox> min_spinbox_, max_spinbox_;

public:
	explicit LevelsWidget(QWidget* parent);

signals:
	void valuesChanged(std::pair<double, double> minmax);

public slots:
	void setRange(double minimum, double maximum);
	inline void setRange(std::pair<double, double> minmax) { setRange(minmax.first, minmax.second); }
	void setValues(double minimum, double maximum);
	inline void setValues(std::pair<double, double> minmax) { setValues(minmax.first, minmax.second); }
	void notifyTextureInitialized(const OpenGLTexture* texture);

private slots:
	inline void notifyValuesChanged(std::pair<double, double> minmax) { emit valuesChanged(minmax); }
	inline void notifyMinValueChanged(double min) { notifyValuesChanged(std::make_pair(min, max_spinbox_->value())); }
	inline void notifyMaxValueChanged(double max) { notifyValuesChanged(std::make_pair(min_spinbox_->value(), max)); }
};


#endif //_LEVELSWIDGET_H