#include <memory>

#include <QFileDialog>

#include <mainwindow.h>

MainWindow::Exception::Exception(const QString& what):
	::Exception(what) {
}
void MainWindow::Exception::raise() const {
	throw *this;
}
QException* MainWindow::Exception::clone() const {
	return new MainWindow::Exception(*this);
}

MainWindow::FileOpenError::FileOpenError(const QString& reason):
	MainWindow::Exception(reason) {
}
void MainWindow::FileOpenError::raise() const {
	throw *this;
}
QException* MainWindow::FileOpenError::clone() const {
	return new MainWindow::FileOpenError(*this);
}

MainWindow::MainWindow(const QString& fits_filename): QMainWindow() {

	std::unique_ptr<QFile> file{new QFile(fits_filename)};
	if (!file->open(QIODevice::ReadOnly)) {
		throw FileOpenError(file->errorString());
	}

	std::unique_ptr<FITS> fits{new FITS(file.release())};
	resize(fits->data_unit().width(), fits->data_unit().height());

	open_gl_widget_.reset(new OpenGLWidget(this, fits.release()));
	setCentralWidget(open_gl_widget_.get());
}
