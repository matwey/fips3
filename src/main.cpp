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

#include <QApplication>
#include <QDebug>
#include <QSurfaceFormat>

#include <application.h>

int main(int argc, char** argv) {
	/*
	 * OpenGL version cannot be required per widget on MacOS
	 * systems. We have to require it globaly.
	 */
	QSurfaceFormat format;
	format.setVersion(4, 1);
	format.setProfile(QSurfaceFormat::CoreProfile);
	QSurfaceFormat::setDefaultFormat(format);

	try {
		Application app(argc, argv);

		return app.exec();
	} catch (const std::exception& e) {
		qCritical() << e.what();
		return 1;
	}

	return 0;
}
