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

#include <QCommandLineParser>
#include <QFileDialog>
#include <QMessageBox>
#include <QTimer>
#include <QWindow>

#include <application.h>
#include <instance.h>
#include <mainwindow.h>

Application::Application(int &argc, char **argv):
	QApplication(argc, argv) {

	QCommandLineParser parser;
	parser.addHelpOption();
	parser.addPositionalArgument("file", QCoreApplication::translate("main", "The file to open."));
	parser.process(*this);

	const QStringList args = parser.positionalArguments();

	if (args.length() == 0) {
#ifdef Q_OS_MAC
		QTimer::singleShot(0, this, [this] () { if (root_.children().length() == 0) openFile(); });
#else
		openFile();
#endif // Q_OS_MAC
	} else {
		for (const auto& x: args) addInstance(x);
	}

}
Application::~Application() = default;

void Application::addInstance(const QString& filename) {
	new Instance(&root_, filename);
}

void Application::openFile() {
	QString filename = QFileDialog::getOpenFileName(Q_NULLPTR, tr("Open FITS file"));

	if (filename.isEmpty()) return;

	try {
		Application::instance()->addInstance(filename);
	} catch (const std::exception& e) {
		QMessageBox::critical(Q_NULLPTR, "An error occured", e.what());
	}
}

#ifdef Q_OS_MAC
bool Application::event(QEvent *event) {
	if (event->type() == QEvent::FileOpen) {
		auto *openEvent = static_cast<QFileOpenEvent *>(event);
		addInstance(openEvent->file());
	}

	return QApplication::event(event);
}
#endif
