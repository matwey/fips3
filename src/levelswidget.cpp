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

void DoubleSlider::notifyValueChanged(int value) {
	const auto new_double_value = static_cast<double>(value - QSlider::minimum()) / (QSlider::maximum() - QSlider::minimum())
				* (double_maximum_ - double_minimum_)
				+ double_minimum_;
	setValue(new_double_value);
}

LevelsWidget::LevelsWidget(QWidget* parent, double minimum, double maximum): QWidget(parent) {
	std::unique_ptr<DoubleSlider> min_slider{new DoubleSlider(Qt::Horizontal, this, minimum, maximum)};
	std::unique_ptr<QDoubleSpinBox> min_spinbox{new QDoubleSpinBox(this)};
	min_spinbox->setRange(minimum, maximum);
	connect(min_spinbox.get(), SIGNAL(valueChanged(double)), min_slider.get(), SLOT(setValue(double)));
	connect(min_slider.get(), SIGNAL(valueChanged(double)), min_spinbox.get(), SLOT(setValue(double)));

	std::unique_ptr<DoubleSlider> max_slider{new DoubleSlider(Qt::Horizontal, this, minimum, maximum)};
	std::unique_ptr<QDoubleSpinBox> max_spinbox{new QDoubleSpinBox(this)};
	max_spinbox->setRange(minimum, maximum);
	connect(max_spinbox.get(), SIGNAL(valueChanged(double)), max_slider.get(), SLOT(setValue(double)));
	connect(max_slider.get(), SIGNAL(valueChanged(double)), max_spinbox.get(), SLOT(setValue(double)));

	std::unique_ptr<QGridLayout> grid_layout{new QGridLayout(this)};
	grid_layout->addWidget(min_slider .release(), 0, 0);
	grid_layout->addWidget(min_spinbox.release(), 0, 1);
	grid_layout->addWidget(max_slider .release(), 1, 0);
	grid_layout->addWidget(max_spinbox.release(), 1, 1);
	setLayout(grid_layout.release());
}