#include <colormapwidget.h>

ColorMapWidget::ColorMapWidget(QWidget* parent, const OpenGLWidget& open_gl_widget):
	QWidget(parent),
	button_group_(new QButtonGroup(this)) {

	for (int i = 0; i < open_gl_widget.colormaps().size(); ++i) {
		button_group_->addButton(new QRadioButton(tr(open_gl_widget.colormaps()[i]->name()), this), i);
	}
	button_group_->buttons()[open_gl_widget.colorMapIndex()]->setChecked(true);

	std::unique_ptr<QVBoxLayout> widget_layout{new QVBoxLayout(this)};
	for (auto &button: button_group_->buttons()) {
		widget_layout->addWidget(button);
	}
	widget_layout->addStretch(1);
	setLayout(widget_layout.release());
}
