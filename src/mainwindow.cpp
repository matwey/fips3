#include <memory>

#include <QFile>
#include <QVBoxLayout>

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
	// Open FITS file
	std::unique_ptr<QFile> file{new QFile(fits_filename)};
	if (!file->open(QIODevice::ReadOnly)) {
		throw FileOpenError(file->errorString());
	}

	// Read FITS from file
	std::unique_ptr<FITS> fits{new FITS(file.release())};

	// Resize window to fit FITS image
	// FIXME: reduce window size if FITS larger than Desktop and change scale_factor_
	resize(fits->data_unit().size());

	// Create scroll area and put there open_gl_widget
	std::unique_ptr<ScrollZoomArea> scroll_zoom_area{new ScrollZoomArea(this, fits.release())};
	/* setCentralWidget promises to take ownership */
	setCentralWidget(scroll_zoom_area.release());

	std::unique_ptr<QMenuBar> menu_bar{new QMenuBar()};
	auto view_menu = menu_bar->addMenu(tr("&View"));
	auto zoomIn_action = view_menu->addAction(tr("Zoom &In"), this, SLOT(zoomIn(void)));
	zoomIn_action->setShortcut(QKeySequence::ZoomIn);
	auto zoomOut_action = view_menu->addAction(tr("Zoom &Out"), this, SLOT(zoomOut(void)));
	zoomOut_action->setShortcut(QKeySequence::ZoomOut);

#if 0 // Not implemented
	auto fitToWindow_action = view_menu->addAction(tr("&Fit to Window"), this, SLOT(zoomOut(void)));
	fitToWindow_action->setShortcut(tr("Ctrl+F"));
#endif

	/* setMenuBar promises to take ownership */
	setMenuBar(menu_bar.release());
}

void MainWindow::zoomIn() {
	scrollZoomArea()->zoomViewport(zoomIn_factor);
}

void MainWindow::zoomOut() {
	scrollZoomArea()->zoomViewport(zoomOut_factor);
}

void MainWindow::resizeEvent(QResizeEvent *event) {
	QMainWindow::resizeEvent(event);
}
