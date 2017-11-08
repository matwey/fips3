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

#include <QGridLayout>
#include <QLabel>

#include <memory>

#include <rotationwidget.h>

RotationWidget::RotationWidget(QWidget* parent):
	spinbox_with_slider_(new SpinboxWithSlider(Qt::Horizontal, this)),
	QWidget(parent) {

	std::unique_ptr<QGridLayout> widget_layout{new QGridLayout(this)};
	widget_layout->addWidget(spinbox_with_slider_, 0, 0);
	widget_layout->addWidget(new QLabel(tr("Deg"), this), 0, 1);
	widget_layout->setRowStretch(1, 1);
	setLayout(widget_layout.release());

	spinbox()->setRange(-180, 180);
	spinbox()->setValue(0);
}