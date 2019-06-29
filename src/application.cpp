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

#include <algorithm>

#include <application.h>
#include <instance.h>

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
		if (openFile() == 0) {
			/* Remember that showHelp() exits applications */
			parser.showHelp();
		}
#endif // Q_OS_MAC
	} else {
		const auto failed = std::none_of(args.begin(), args.end(), [this](const QString& filename) {
			return addInstance(filename);
		});

		if (failed)
			::exit(1); /* exactly what parser.showHelp() does under the hood */
	}

}
Application::~Application() = default;

std::size_t Application::addInstance(const QString& filename) {
	try {
		new Instance(&root_, filename);

		return 1;
	} catch (const std::exception& e) {
		QMessageBox::critical(
				Q_NULLPTR, "An error occured",
				filename + tr("<br/><br/>") + e.what()
		);
	}

	return 0;
}

std::size_t Application::openFile() {
	QString filename = QFileDialog::getOpenFileName(Q_NULLPTR, tr("Open FITS file"));

	if (filename.isEmpty()) return 0;

	return Application::instance()->addInstance(filename);
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
