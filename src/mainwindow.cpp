#include <QFileDialog>

#include <mainwindow.h>

MainWindow::MainWindow(const QString& fits_filename):
	QMainWindow() {

	QFile* file = new QFile(fits_filename);
	// FIXME: check open return value
	// http://doc.qt.io/qt-5/qfile.html#open-1
	file->open(QIODevice::ReadOnly);
	open_gl_widget_.reset(new OpenGLWidget(this, new FITS(file)));

	resize(800, 448);
}

