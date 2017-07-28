#include <memory>

#include <QFile>
#include <QVBoxLayout>

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
	// Open FITS file
	std::unique_ptr<QFile> file{new QFile(fits_filename)};
	if (!file->open(QIODevice::ReadOnly)) {
		throw FileOpenError(file->errorString());
	}

	// Read FITS from file
	std::unique_ptr<FITS> fits{new FITS(file.release())};
	fits_size_.reset(new QSize(fits->data_unit().width(), fits->data_unit().height()));

	// Resize window to fit FITS image
	// FIXME: reduce window size if FITS larger than Desktop and change scale_factor_
	resize(*fits_size_);

	// Create scroll area and put there open_gl_widget
	scroll_zoom_area_.reset(new ScrollZoomArea(this, fits.release()));
	setCentralWidget(scroll_zoom_area_.get());

	// Set menu actions
	// Use common menu for all windows on Mac
	#ifdef Q_OS_MAC
		menu_bar_.reset(new QMenuBar(nullptr));
	#else
		menu_bar_.reset(new QMenuBar(this));
		setMenuBar(menu_bar_.get());
	#endif
	view_menu_.reset(menu_bar_->addMenu(tr("&View")));

	zoomIn_action_.reset (view_menu_->addAction(tr("Zoom &In"),  this, &MainWindow::zoomIn));
	zoomIn_action_->setShortcut (QKeySequence::ZoomIn);

	zoomOut_action_.reset(view_menu_->addAction(tr("Zoom &Out"), this, &MainWindow::zoomOut));
	zoomOut_action_->setShortcut(QKeySequence::ZoomOut);

	fitToWindow_action_.reset(view_menu_->addAction(tr("&Fit to Window"), this, &MainWindow::fitToWindow));
	fitToWindow_action_->setShortcut(tr("Ctrl+F"));

	updateActions();
}

void MainWindow::zoomIn() {
	scroll_zoom_area_->zoomViewport(zoomIn_factor);
	updateActions();
}

void MainWindow::zoomOut() {
	scroll_zoom_area_->zoomViewport(zoomOut_factor);
	updateActions();
}

void MainWindow::fitToWindow() {
//	scaleWidget(scroll_zoom_area_->size());
//	updateActions();
}

void MainWindow::updateActions() {
//	QSize window_size(scroll_zoom_area_->viewport()->size());
//	QSize widget_size(open_gl_widget_->size());
//
//	// Disable "Fit to Window" action if it will do nothing
//	fitToWindow_action_->setEnabled(
//			window_size.width()  != widget_size.width() &&
//			window_size.height() != widget_size.height()
//	);
//
//	// Disable "Zoom Out" action if image is too small to zoomed in after zoomed out
//	auto min_widget_side_after_zoomOut = static_cast<int>(std::min(widget_size.width(), widget_size.height()) * zoomOut_factor);
//	zoomOut_action_->setEnabled(
//		static_cast<int>(min_widget_side_after_zoomOut * zoomIn_factor) > min_widget_side_after_zoomOut
//	);
}

void MainWindow::resizeEvent(QResizeEvent *event) {
	QMainWindow::resizeEvent(event);
	updateActions();
}