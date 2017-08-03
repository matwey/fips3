#include <levelswidget.h>

DoubleSlider::DoubleSlider(Qt::Orientation orientation, QWidget *parent, double minimum, double maximum):
		QSlider(orientation, parent),
		double_minimum_(minimum),
		double_maximum_(maximum),
		double_value_(double_minimum_) {
	connect(this, SIGNAL(valueChanged(int)), this, SLOT(notifyValueChanged(int)));
}

void DoubleSlider::resizeEvent(QResizeEvent *event) {
	int length;
	if (orientation() == Qt::Horizontal) {
		length = event->size().width();
	} else {
		length = event->size().height();
	}
	QSlider::setRange(0, length-1);
	setValue(double_value_);
}

void DoubleSlider::setValue(double value) {
	if (value < double_minimum_) {
		value = double_minimum_;
	} else if (value > double_maximum_) {
		value = double_maximum_;
	}

	const int new_int_value = qRound((double_value_ - double_minimum_) / (double_maximum_ - double_minimum_)
			* (QSlider::maximum() - QSlider::minimum()));
	QSlider::setValue(new_int_value);

	if (double_value_ != value) {
		double_value_ = value;
		emit valueChanged(double_value_);
	}
}

void DoubleSlider::setRange(double minimum, double maximum) {
	double_minimum_ = minimum;
	double_maximum_ = maximum;
	setValue(double_value_);
}

void DoubleSlider::notifyValueChanged(int value) {
	const auto new_double_value = static_cast<double>(value - QSlider::minimum()) / (QSlider::maximum() - QSlider::minimum())
				* (double_maximum_ - double_minimum_)
				+ double_minimum_;
	setValue(new_double_value);
}


LevelsWidget::LevelsWidget(QWidget* parent):
			QWidget(parent),
			min_slider_(new DoubleSlider(Qt::Horizontal, this)),
			max_slider_(new DoubleSlider(Qt::Horizontal, this)),
			min_spinbox_(new QDoubleSpinBox(this)),
			max_spinbox_(new QDoubleSpinBox(this)) {
	connect(min_spinbox_.get(), SIGNAL(valueChanged(double)), min_slider_.get(), SLOT(setValue(double)));
	connect(min_slider_.get(), SIGNAL(valueChanged(double)), min_spinbox_.get(), SLOT(setValue(double)));

	connect(max_spinbox_.get(), SIGNAL(valueChanged(double)), max_slider_.get(), SLOT(setValue(double)));
	connect(max_slider_.get(), SIGNAL(valueChanged(double)), max_spinbox_.get(), SLOT(setValue(double)));

	std::unique_ptr<QGridLayout> grid_layout{new QGridLayout(this)};
	grid_layout->addWidget(min_slider_ .get(), 0, 0);
	grid_layout->addWidget(min_spinbox_.get(), 0, 1);
	grid_layout->addWidget(max_slider_ .get(), 1, 0);
	grid_layout->addWidget(max_spinbox_.get(), 1, 1);
	setLayout(grid_layout.release());
}

void LevelsWidget::setRange(double minimum, double maximum) {
	min_slider_->setRange(minimum, maximum);
	max_slider_->setRange(minimum, maximum);
	min_spinbox_->setRange(minimum, maximum);
	max_spinbox_->setRange(minimum, maximum);

	min_slider_->setValue(minimum);
	max_slider_->setValue(maximum);
}