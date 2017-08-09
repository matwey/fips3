#include <palettewidget.h>

PaletteWidget::PaletteWidget(QWidget* parent):
		QWidget(parent),
		button_group_(new QButtonGroup(this)) {
	button_group_->addButton(new QRadioButton(tr("Grayscale"), this), GrayPalette);
	button_group_->addButton(new QRadioButton(tr("Black–Yellow–Green–Blue"), this), HalfHSLCirclePalette);
	button_group_->addButton(new QRadioButton(tr("Purple–Blue"), this), PurpleBluePalette);

	button_group_->buttons().first()->setChecked(true);

	std::unique_ptr<QVBoxLayout> widget_layout{new QVBoxLayout(this)};
	for (auto &button: button_group_->buttons()) {
		widget_layout->addWidget(button);
	}
	widget_layout->addStretch(1);
	setLayout(widget_layout.release());
}