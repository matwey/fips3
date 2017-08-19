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

#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include <QApplication>
#include <QEvent>
#include <QMenuBar>
#include <QString>

class Application:
	public QApplication {
	Q_OBJECT
private:
	QObject root_;
public:
	Application(int &argc, char **argv);
	virtual ~Application() override;

	void addInstance(const QString& filename);
	inline static Application* instance() {
		return static_cast<Application*>(QCoreApplication::instance());
	}

#ifdef Q_OS_MAC
	virtual bool event(QEvent* event) override;
#endif
public slots:
	static void openFile();
};

#endif // _APPLICATION_H_
