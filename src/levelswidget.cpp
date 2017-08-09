#include <levelswidget.h>

SpinboxWithSlider::SpinboxWithSlider(Qt::Orientation orientation, QWidget *parent):
		QWidget(parent),
		spinbox_(new QDoubleSpinBox(this)),
		slider_(new QSlider(orientation, this)) {
	slider_->setRange(0, slider_range_);

	std::unique_ptr<QGridLayout> widget_layout{new QGridLayout(this)};
	widget_layout->addWidget(slider_.get(), 0, 0);
	if (orientation == Qt::Horizontal) {
		widget_layout->addWidget(spinbox_.get(), 0, 1);
		widget_layout->setRowStretch(1, 1);
	} else {
		widget_layout->addWidget(spinbox_.get(), 1, 0);
		widget_layout->setColumnStretch(1, 1);
	}
	setLayout(widget_layout.release());

	connect(spinbox_.get(), SIGNAL(valueChanged(double)), this, SLOT(notifySpinboxValueChanged(double)));
	connect(slider_.get(), SIGNAL(valueChanged(int)), this, SLOT(notifySliderValueChanged(int)));
}

int SpinboxWithSlider::spinboxValueToSlider(double value) const {
	return qRound((value - spinbox_->minimum()) / (spinbox_->maximum() - spinbox_->minimum())
		   * (slider_->maximum() - slider_->minimum()))
		   + slider_->minimum();
}

void SpinboxWithSlider::notifySpinboxValueChanged(double value) {
	slider_->setValue(spinboxValueToSlider(value));
}

void SpinboxWithSlider::notifySliderValueChanged(int value) {
	if (spinboxValueToSlider(spinbox_->value()) != value) {
		const double spinbox_value =
				static_cast<double>(value - slider_->minimum()) / (slider_->maximum() - slider_->minimum())
				* (spinbox_->maximum() - spinbox_->minimum())
				+ spinbox_->minimum();
		spinbox_->setValue(spinbox_value);
	}
}


LevelsWidget::LevelsWidget(QWidget* parent):
			QWidget(parent),
			min_level_(new SpinboxWithSlider(Qt::Horizontal, this)),
			max_level_(new SpinboxWithSlider(Qt::Horizontal, this)) {
	connect(min_level_->spinbox(), SIGNAL(valueChanged(double)), this, SLOT(notifyMinValueChanged(double)));
	connect(max_level_->spinbox(), SIGNAL(valueChanged(double)), this, SLOT(notifyMaxValueChanged(double)));

	std::unique_ptr<QGridLayout> widget_layout{new QGridLayout(this)};
	widget_layout->addWidget(new QLabel(tr("Min"), this), 0, 0);
	widget_layout->addWidget(min_level_.get(), 0, 1);
	widget_layout->addWidget(new QLabel(tr("Max"), this), 1, 0);
	widget_layout->addWidget(max_level_.get(), 1, 1);
	widget_layout->setRowStretch(2, 1);
	setLayout(widget_layout.release());
}

void LevelsWidget::setRange(double minimum, double maximum) {
	min_level_->spinbox()->setRange(minimum, maximum);
	max_level_->spinbox()->setRange(minimum, maximum);
	setValues(minimum, maximum);
}

void LevelsWidget::setValues(double minimum, double maximum) {
	min_level_->spinbox()->setValue(minimum);
	max_level_->spinbox()->setValue(maximum);
}

void LevelsWidget::notifyTextureInitialized(const OpenGLTexture *texture) {
	setRange(texture->instrumental_minmax());
	setValues(texture->hdu_minmax());
}