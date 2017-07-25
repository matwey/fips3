#include <memory>

#include <QFile>

#include <mainwindow.h>

void MainWindow::Exception::raise() const {
	throw *this;
}
QException* MainWindow::Exception::clone() const {
	return new MainWindow::Exception(*this);
}

MainWindow::FileOpenError::FileOpenError(const QString& reason) {
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

	scroll_area.reset(new QScrollArea(this));
	scroll_area->resize(size());
	open_gl_widget_.reset(new OpenGLWidget(scroll_area.get(), fits.release()));
	open_gl_widget_->resize(size() * 2);
	scroll_area->setWidget(open_gl_widget_.get());
}
