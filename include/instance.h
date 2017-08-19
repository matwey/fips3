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

#ifndef _INSTANCE_H
#define _INSTANCE_H

#include <memory.h>

#include <QString>

#include <application.h>
#include <mainwindow.h>

class Instance:
	public QObject {

	Q_OBJECT
private:
	std::unique_ptr<MainWindow> mainwindow_;
private slots:
	void mainWindowClosed(MainWindow& mainwindow);
public:
	Instance(QObject* parent, const QString& filename);
	virtual ~Instance() override;
};

#endif // _INSTANCE_H
