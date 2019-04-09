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
#include <QToolButton>
#include <QStyle>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include <memory>

#include <playbackwidget.h>

PlaybackWidget::PlaybackWidget(QWidget* parent):
	QWidget(parent),
	frame_slider_{new QSlider(Qt::Horizontal, this)},
	frame_spinbox_{new QSpinBox(this)},
	interval_spinbox_{new QSpinBox(this)},
	play_action_{new QAction(style()->standardIcon(QStyle::SP_MediaPlay), tr("Play"), this)} {

	frame_slider_->setMinimum(0);
	frame_slider_->setMaximum(0);
	connect(
		frame_slider_.get(), SIGNAL(valueChanged(int)),
		this, SIGNAL(frameChanged(int))
	);

	frame_spinbox_->setMinimum(0);
	frame_spinbox_->setMaximum(0);
	connect(
		frame_spinbox_.get(), SIGNAL(valueChanged(int)),
		this, SIGNAL(frameChanged(int))
	);

	connect(
		interval_spinbox_.get(), SIGNAL(valueChanged(int)),
		this, SIGNAL(intervalChanged(int))
	);
	interval_spinbox_->setMinimum(1);
	interval_spinbox_->setMaximum(3600000);  // One hour

	play_action_->setEnabled(false);
	play_action_->setCheckable(true);
	connect(
		play_action_.get(), SIGNAL(toggled(bool)),
		this, SIGNAL(playingChanged(bool))
	);

	std::unique_ptr<QToolButton> play_button{new QToolButton(this)};
	play_button->setDefaultAction(play_action_.get());

	std::unique_ptr<QWidget> frame_widget{new QWidget(this)};
	std::unique_ptr<QHBoxLayout> frame_layout{new QHBoxLayout(frame_widget.get())};
	frame_layout->addWidget(play_button.release());
	frame_layout->addWidget(frame_slider_.get());
	frame_layout->addWidget(frame_spinbox_.get());
	frame_widget->setLayout(frame_layout.release());

	std::unique_ptr<QWidget> interval_widget{new QWidget(this)};
	std::unique_ptr<QHBoxLayout> interval_layout{new QHBoxLayout(interval_widget.get())};
	interval_layout->addWidget(new QLabel("Frame interval"));
	interval_layout->addWidget(interval_spinbox_.get());
	interval_layout->addWidget(new QLabel("ms"));
	interval_widget->setLayout(interval_layout.release());

	std::unique_ptr<QVBoxLayout> widget_layout{new QVBoxLayout(this)};
	widget_layout->addWidget(frame_widget.release());
	widget_layout->addWidget(interval_widget.release());
	widget_layout->addStretch(1);

	setLayout(widget_layout.release());
}

void PlaybackWidget::setDuration(int duration) {
	frame_slider_->setMaximum(duration - 1);
	frame_spinbox_->setMaximum(duration - 1);
}

void PlaybackWidget::setFrame(int frame) {
	if (frame != frame_slider_->value()) frame_slider_->setValue(frame);
	if (frame != frame_spinbox_->value()) frame_spinbox_->setValue(frame);
}

void PlaybackWidget::setInterval(int interval_ms) {
	if (interval_ms == interval_spinbox_->value()) return;

	interval_spinbox_->setValue(interval_ms);
}

void PlaybackWidget::setPlaying(bool playing) {
	if (playing == play_action_->isChecked()) return;

	play_action_->setChecked(playing);
}

void PlaybackWidget::setPlayable(bool playable) {
	if (playable == play_action_->isEnabled()) return;

	play_action_->setEnabled(playable);
}
