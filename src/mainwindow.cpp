#include <QFileDialog>

#include <mainwindow.h>

MainWindow::MainWindow(const QString& fits_filename):
	QMainWindow() {

	QFile* file = new QFile(fits_filename);
	// FIXME: check open return value
	// http://doc.qt.io/qt-5/qfile.html#open-1
	file->open(QIODevice::ReadOnly);
	auto fits = new FITS(file);
	resize(fits->data_unit().width(), fits->data_unit().height());
	open_gl_widget_.reset(new OpenGLWidget(this, fits));
//	open_gl_widget_->resize(size());
	setCentralWidget(open_gl_widget_.get());

//	QSizePolicy size_policy(QSizePolicy::Ignored, QSizePolicy::Ignored);
//	size_policy.setHeightForWidth(false);
//	size_policy.setWidthForHeight(true);
//	open_gl_widget_->setSizePolicy(size_policy);
}
