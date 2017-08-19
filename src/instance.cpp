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

#include <instance.h>

void Instance::mainWindowClosed(MainWindow& mainwindow) {
	Q_ASSERT(&mainwindow == mainwindow_.get());

	deleteLater();
}

/* Ownership model specification.
 *
 * Instance objects are ownered by Application's root_ QObject via QObject
 * ownership mechanism. MainWindow is ownered by Instance using
 * std::unique_ptr. When all windows are closed, Application terminates and
 * deletes all its children and all windows are deleted by unique_ptr.
 */
Instance::Instance(QObject* parent, const QString& filename):
	QObject(parent), mainwindow_(new MainWindow(filename)) {

	mainwindow_->show();

	connect(mainwindow_.get(), SIGNAL(closed(MainWindow&)), this, SLOT(mainWindowClosed(MainWindow&)));
}
Instance::~Instance() = default;
