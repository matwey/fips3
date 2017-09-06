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

#include <levelswidget.h>

LevelsWidget::LevelsWidget(QWidget* parent):
			QWidget(parent),
			min_level_(new SpinboxWithSlider(Qt::Horizontal, this)),
			max_level_(new SpinboxWithSlider(Qt::Horizontal, this)) {
	connect(min_level_->spinbox(), SIGNAL(valueChanged(double)), this, SLOT(notifyMinValueChanged(double)));
	connect(max_level_->spinbox(), SIGNAL(valueChanged(double)), this, SLOT(notifyMaxValueChanged(double)));

	std::unique_ptr<QGridLayout> widget_layout{new QGridLayout(this)};
	widget_layout->addWidget(new QLabel(tr("Min"), this), 0, 0);
	widget_layout->addWidget(min_level_, 0, 1);
	widget_layout->addWidget(new QLabel(tr("Max"), this), 1, 0);
	widget_layout->addWidget(max_level_, 1, 1);
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
