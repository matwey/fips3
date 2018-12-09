/*
 *  Copyright (C) 2018  Matwey V. Kornilov <matwey.kornilov@gmail.com>
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

#include <QAction>
#include <QGridLayout>
#include <QToolButton>
#include <QStyle>
#include <QLabel>

#include <memory>

#include <playbackwidget.h>

PlaybackWidget::PlaybackWidget(QWidget* parent):
	QWidget(parent),
	slider_{new QSlider(Qt::Horizontal, this)},
	play_action_{new QAction(style()->standardIcon(QStyle::SP_MediaPlay), tr("Play"), this)} {

	slider_->setMinimum(0);
	slider_->setMaximum(0);
	connect(
		slider_.get(), SIGNAL(valueChanged(int)),
		this, SIGNAL(frameChanged(int))
	);

	play_action_->setCheckable(true);
	connect(
		play_action_.get(), SIGNAL(toggled(bool)),
		this, SIGNAL(playingChanged(bool))
	);

	std::unique_ptr<QToolButton> play_button{new QToolButton(this)};
	play_button->setDefaultAction(play_action_.get());

	std::unique_ptr<QGridLayout> widget_layout{new QGridLayout(this)};
	widget_layout->addWidget(play_button.release(), 0, 0);
	widget_layout->addWidget(slider_.get(), 0, 1);

	setLayout(widget_layout.release());
}

void PlaybackWidget::setDuration(int duration) {
	slider_->setMaximum(duration - 1);
}

void PlaybackWidget::setFrame(int frame) {
	if (frame == slider_->value()) return;

	slider_->setValue(frame);
}

void PlaybackWidget::setPlaying(bool playing) {
	if (playing == play_action_->isChecked()) return;

	play_action_->setChecked(playing);
}

void PlaybackWidget::setPlayable(bool playable) {
	if (playable == play_action_->isEnabled()) return;

	play_action_->setEnabled(playable);
}
