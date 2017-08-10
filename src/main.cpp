#include <QApplication>
#include <QCommandLineParser>
#include <QFileDialog>

#include <mainwindow.h>

int main(int argc, char** argv) {
	// See related note on http://doc.qt.io/qt-5/qopenglwidget.html
	QSurfaceFormat surface_format;
	surface_format.setVersion(2, 0);
	QSurfaceFormat::setDefaultFormat(surface_format);

	QApplication app(argc, argv);

	QCoreApplication::setApplicationName("fips");
	QCoreApplication::setApplicationVersion("3.0");

	QCommandLineParser parser;
	parser.addHelpOption();
	parser.addPositionalArgument("file", QCoreApplication::translate("main", "The file to open."));

	parser.process(app);

	const QStringList args = parser.positionalArguments();

	QString filename;

	if (args.length() == 0) {
		filename = QFileDialog::getOpenFileName(Q_NULLPTR, QObject::tr("Open FITS file"));
	} else if (args.length() == 1) {
		filename = args[0];
	} else {
		parser.showHelp(1);
		return 1;
	}

	try {
		MainWindow w(filename);
		w.show();

		return app.exec();
	} catch (const std::exception& e) {
		qCritical() << e.what();
		return 1;
	}

	return 0;
}
