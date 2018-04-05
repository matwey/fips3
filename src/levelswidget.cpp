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

#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>

#include <levelswidget.h>

LevelsWidget::LevelsWidget(QWidget* parent):
	QWidget(parent),
	min_level_(new SpinboxWithSlider(Qt::Horizontal, this)),
	max_level_(new SpinboxWithSlider(Qt::Horizontal, this)),
	range_check_box_(new QCheckBox(tr("Set level ranges to image min/max"), this)),
	instrumental_minmax_(min_level_->spinbox()->value(), max_level_->spinbox()->value()),
	image_minmax_(min_level_->spinbox()->value(), max_level_->spinbox()->value()) {

	std::unique_ptr<QVBoxLayout> widget_layout{new QVBoxLayout(this)};

	connect(min_level_->spinbox(), SIGNAL(valueChanged(double)), this, SLOT(notifyMinValueChanged(double)));
	connect(max_level_->spinbox(), SIGNAL(valueChanged(double)), this, SLOT(notifyMaxValueChanged(double)));

	std::unique_ptr<QWidget> sliders_spinboxes_widget{new QWidget(this)};
	std::unique_ptr<QGridLayout> sliders_spinboxes_layout{new QGridLayout(sliders_spinboxes_widget.get())};
	sliders_spinboxes_layout->addWidget(new QLabel(tr("Min"), this), 0, 0);
	sliders_spinboxes_layout->addWidget(min_level_.get(), 0, 1);
	sliders_spinboxes_layout->addWidget(new QLabel(tr("Max"), this), 1, 0);
	sliders_spinboxes_layout->addWidget(max_level_.get(), 1, 1);
	sliders_spinboxes_widget->setLayout(sliders_spinboxes_layout.release());

	std::unique_ptr<QPushButton> auto_button{new QPushButton(tr("Auto"), this)};
	connect(auto_button.get(), SIGNAL(clicked()), this, SLOT(setValuesToImageMinMax()));

	connect(range_check_box_.get(), SIGNAL(stateChanged(int)), this, SLOT(notifyRangeCheckboxStateChanged(int)));

	widget_layout->addWidget(sliders_spinboxes_widget.release());
	widget_layout->addWidget(auto_button.release());
	widget_layout->addWidget(range_check_box_.get());
	widget_layout->addStretch(1);
	setLayout(widget_layout.release());
}

void LevelsWidget::setRange(double minimum, double maximum) {
	min_level_->spinbox()->setRange(minimum, maximum);
	max_level_->spinbox()->setRange(minimum, maximum);
}

void LevelsWidget::setValues(double minimum, double maximum) {
	min_level_->spinbox()->setValue(minimum);
	max_level_->spinbox()->setValue(maximum);
}

void LevelsWidget::notifyTextureInitialized(const AbstractOpenGLTexture *texture) {
	instrumental_minmax_ = texture->instrumentalMinMax();
	setRange(texture->instrumentalMinMax());
	image_minmax_ = texture->hduMinMax();
	setValues(image_minmax_);
	if (instrumental_minmax_ == image_minmax_) {
		range_check_box_->hide();
	} else{
		notifyRangeCheckboxStateChanged(range_check_box_->checkState());
		range_check_box_->show();
	}
}

void LevelsWidget::notifyRangeCheckboxStateChanged(int state) {
	switch (state) {
	case Qt::Unchecked:
		setRangeToInstrumentalMinMax();
		return;
	case Qt::Checked:
		setRangeToImageMinMax();
		return;
	default:
		Q_ASSERT(false);
	}
}
