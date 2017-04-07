#include <QApplication>
#include <QCommandLineParser>

#include <mainwindow.h>

int main(int argc, char** argv) {
	// See related note on http://doc.qt.io/qt-5/qopenglwidget.html
	QSurfaceFormat::setDefaultFormat(QSurfaceFormat::defaultFormat());

	QApplication app(argc, argv);

	QCoreApplication::setApplicationName("fips");
	QCoreApplication::setApplicationVersion("3.0");

	QCommandLineParser parser;
	parser.addHelpOption();
	parser.addPositionalArgument("file", QCoreApplication::translate("main", "The file to open."));

	parser.process(app);

	const QStringList args = parser.positionalArguments();

	MainWindow w;
	w.show();

	return app.exec();
}
