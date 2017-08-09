#ifndef _LEVELSWIDGET_H
#define _LEVELSWIDGET_H

#include <QDebug>
#include <QDoubleSpinBox>
#include <QGridLayout>
#include <QLabel>
#include <QResizeEvent>
#include <QSlider>
#include <QWidget>

#include <memory>

#include <opengltexture.h>

class SpinboxWithSlider: public QWidget {
	Q_OBJECT
private:
	static constexpr int slider_range_ = 10000;
	std::unique_ptr<QDoubleSpinBox> spinbox_;
	std::unique_ptr<QSlider> slider_;

public:
	SpinboxWithSlider(Qt::Orientation orientation, QWidget* parent=Q_NULLPTR);

	inline QDoubleSpinBox* spinbox() const { return spinbox_.get(); }
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
