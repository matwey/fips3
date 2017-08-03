#ifndef _LEVELSWIDGET_H
#define _LEVELSWIDGET_H

#include <QDebug>
#include <QDoubleSpinBox>
#include <QGridLayout>
#include <QResizeEvent>
#include <QSlider>
#include <QStyle>
#include <QWidget>

#include <memory>

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

signals:
	void valueChanged(double value);

public slots:
	void setValue(double value);

private slots:
	void notifyValueChanged(int value);
};

class LevelsWidget: public QWidget {
public:
	explicit LevelsWidget(QWidget* parent);
};


#endif //_LEVELSWIDGET_H
