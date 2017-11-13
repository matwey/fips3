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

#include <QVBoxLayout>

#include <flipwidget.h>

FlipWidget::FlipWidget(QWidget *parent):
	QWidget(parent),
	h_flip_check_box_(new QCheckBox(tr("Horizontal flipping"), this)),
	v_flip_check_box_(new QCheckBox(tr("Vertical flipping"), this)) {

	std::unique_ptr<QVBoxLayout> widget_layout{new QVBoxLayout(this)};
	widget_layout->addWidget(h_flip_check_box_);
	widget_layout->addWidget(v_flip_check_box_);
	widget_layout->addStretch(1);
	setLayout(widget_layout.release());
}
