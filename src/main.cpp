#include <QApplication>
#include <QDebug>
#include <QSurfaceFormat>

#include <application.h>

int main(int argc, char** argv) {
	// See related note on http://doc.qt.io/qt-5/qopenglwidget.html
	QSurfaceFormat surface_format;
	surface_format.setVersion(2, 1);
	QSurfaceFormat::setDefaultFormat(surface_format);

	try {
		Application app(argc, argv);

		return app.exec();
	} catch (const std::exception& e) {
		qCritical() << e.what();
		return 1;
	}

	return 0;
}
