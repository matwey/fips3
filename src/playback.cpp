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

#include <playback.h>

Playback::Timer::Timer(Playback* parent):
	QTimer(parent) {

	/* This is default value. We probably need Qt::PreciseTimer for smooth
	 * playback */
	setTimerType(Qt::CoarseTimer);
}

void Playback::Timer::timerEvent(QTimerEvent *e) {
	auto playback = reinterpret_cast<Playback*>(parent());

	playback->advanceFrame();
}

Playback::Playback(QObject* parent):
	QObject(parent),
	frame_(0),
	duration_(1),
	timer_(this) {
}

void Playback::setPlayable() {
	const bool playable = duration_ > 1;

	if (!playable) {
		setPlaying(false);
	}

	emit playableChanged(playable);
}

void Playback::setDuration(int duration) {
	if (duration_ == duration) return;

	if (frame_ >= duration_) {
		setFrame(duration_ - 1);
	}

	duration_ = duration;
	emit durationChanged(duration_);

	setPlayable();
}

void Playback::setFrame(int frame) {
	if (frame >= duration_)
		frame = duration_ - 1;

	if (frame_ == frame) return;

	frame_ = frame;
	emit frameChanged(frame_);
}

void Playback::setInterval(int interval) {
	if (timer_.interval() == interval) return;

	timer_.setInterval(interval);
	emit intervalChanged(interval);
}

void Playback::advanceFrame() {
	frame_ = (frame_ + 1) % duration_;
	emit frameChanged(frame_);
}

void Playback::setPlaying(bool playing) {
	if (playing == timer_.isActive()) return;

	if (playing) {
		timer_.start();
	} else {
		timer_.stop();
	}
	emit playingChanged(playing);
}
