#include <QFileDialog>

#include <mainwindow.h>

MainWindow::MainWindow(const QString& fits_filename):
	QMainWindow(),
	fits_filename_(fits_filename),
	open_gl_widget_(new OpenGLWidget(this, fits_filename_)) {

	resize(800, 448);
}

