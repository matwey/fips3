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

#ifndef _PLAYBACK_H
#define _PLAYBACK_H

#include <cstddef>

#include <QObject>
#include <QTimer>

class Playback:
	public QObject {
Q_OBJECT
private:
	class Timer:
		public QTimer {
	protected:
		virtual void timerEvent(QTimerEvent *e) override;
	public:
		explicit Timer(Playback* parent);
		virtual ~Timer() override = default;
	};

public:
	explicit Playback(QObject* parent = Q_NULLPTR);
	virtual ~Playback() override = default;

	inline int frame()    const { return frame_; }
	inline int duration() const { return duration_; }
	inline bool loop() const { return loop_; }
	inline int interval() const { return timer_.interval(); }
	inline bool playing() const { return timer_.isActive(); }
	inline bool playable() const { return duration_ > 1; }

private slots:
	void setPlayable();

public slots:
	void setDuration(int frames);
	void setFrame(int frame);
	void setInterval(int interval);
	void setLoop(bool loop);
	void setPlaying(bool playing);
	void advanceFrame();

signals:
	void durationChanged(int duration);
	void frameChanged(int frame);
	void intervalChanged(int interval);
	void loopChanged(bool loop);
	void playingChanged(bool playing);
	void playableChanged(bool playable);

private:
	int frame_;
	int duration_;
	bool loop_;
	Timer timer_;
};

#endif // _PLAYBACK_H
