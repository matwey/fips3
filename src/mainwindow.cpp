#include <QFileDialog>

#include <mainwindow.h>

MainWindow::MainWindow(const QString& fits_filename):
	QMainWindow() {

	QFile* file = new QFile(fits_filename);
	file->open(QIODevice::ReadOnly);
	open_gl_widget_.reset(new OpenGLWidget(this, new FITS(file)));

	resize(800, 448);
}

