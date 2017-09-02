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

#include <colormapwidget.h>

ColorMapWidget::ColorMapWidget(QWidget* parent, const OpenGLWidget& open_gl_widget) noexcept:
	QWidget(parent),
	button_group_(new QButtonGroup(this)) {

	for (std::size_t i = 0; i < open_gl_widget.colormaps().size(); ++i) {
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
