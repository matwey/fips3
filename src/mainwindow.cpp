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
	scroll_area_.reset(new ScrollArea(this, fits.release()));
	setCentralWidget(scroll_area_.get());

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
	zoomWidget(zoomIn_factor);
}

void MainWindow::zoomOut() {
	zoomWidget(zoomOut_factor);
}

void MainWindow::fitToWindow() {
//	scaleWidget(scroll_area_->size());
}

void MainWindow::updateActions() {
//	QSize window_size(scroll_area_->viewport()->size());
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

void MainWindow::zoomWidget(double zoom_factor) {
	const auto old_viewport = scroll_area_->viewportViewport();
	const auto new_viewport_size = scroll_area_->viewport()->size().scaled(old_viewport.size() / zoom_factor, Qt::KeepAspectRatio);
	const auto new_viewport_top_left = old_viewport.topLeft()
						  + QPoint((old_viewport.width() - new_viewport_size.width()) / 2, (old_viewport.height() - new_viewport_size.height()) / 2);
	scaleWidget(QRect(new_viewport_top_left, new_viewport_size));
}

void MainWindow::scaleWidget(const QRect& viewport) {
	scroll_area_->setViewportViewport(viewport);
//	qDebug() << (scroll_area_->viewport()->isVisible() && scroll_area_->viewport()->updatesEnabled());
//	scroll_area_->viewport()->update();
	updateActions();
}

void MainWindow::adjustScrollBar(QScrollBar* scroll_bar, double zoom_factor) {
//	scroll_bar->setValue(
//			static_cast<int>(zoom_factor * scroll_bar->value() + ((zoom_factor - 1) * scroll_bar->pageStep()/2))
//	);
}

void MainWindow::resizeEvent(QResizeEvent *event) {
	QMainWindow::resizeEvent(event);
	updateActions();
}