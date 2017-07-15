#include <QFileDialog>

#include <mainwindow.h>

MainWindow::MainWindow():
		QMainWindow(),
		fits_filename_( QFileDialog::getOpenFileName(this, tr("Open FITS file")) ),
		open_gl_widget_(new OpenGLWidget(this, fits_filename_)) {
	resize(800, 448);
}

