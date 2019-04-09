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

#ifndef _PLAYBACKWIDGET_H
#define _PLAYBACKWIDGET_H

#include <QAction>
#include <QSlider>
#include <QSpinBox>
#include <QWidget>

#include <playback.h>

class PlaybackWidget: public QWidget {
	Q_OBJECT
private:
	std::unique_ptr<QSlider> frame_slider_;
	std::unique_ptr<QSpinBox> frame_spinbox_;
	std::unique_ptr<QSpinBox> interval_spinbox_;
	std::unique_ptr<QAction> play_action_;
public:
	explicit PlaybackWidget(QWidget *parent = Q_NULLPTR);

	inline int duration() const { return frame_slider_->maximum(); }
	inline int frame() const { return frame_slider_->value(); }
	inline bool playing() const { return play_action_->isChecked(); }
	inline bool playable() const { return play_action_->isEnabled(); }

public slots:
	void setDuration(int duration);
	void setFrame(int frame);
	void setInterval(int interval_ms);
	void setPlaying(bool playing);
	void setPlayable(bool playable);
signals:
	void frameChanged(int);
	void intervalChanged(int);
	void playingChanged(bool);
};

#endif //_PLAYBACKWIDGET_H
