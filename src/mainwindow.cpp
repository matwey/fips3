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
	scroll_area_.reset(new QScrollArea(this));
	setCentralWidget(scroll_area_.get());
	open_gl_widget_.reset(new OpenGLWidget(scroll_area_.get(), fits.release()));
	open_gl_widget_->resize(size());
	scroll_area_->setWidget(open_gl_widget_.get());

	// Set menu actions
	// Use common menu for all windows on Mac
	#ifdef Q_OS_MAC
		menu_bar_.reset(new QMenuBar(nullptr));
		// TODO: move from if-else block to support Ubuntu Unity menu bar
		menu_bar_->setNativeMenuBar(true);
	#else
		menu_bar_.reset(new QMenuBar(this));
		// TODO: delete this line to support Ubuntu Unity menu bar
		menu_bar_->setNativeMenuBar(false);
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
	scaleWidget(size());
}

void MainWindow::updateActions() {
	QSize window_size(size());
	QSize widget_size(open_gl_widget_->size());

	// Disable "Fit to Window" action if it will do nothing
	fitToWindow_action_->setEnabled(
			window_size.width()  != widget_size.width() &&
			window_size.height() != widget_size.height()
	);

	// Disable "Zoom Out" action if image is too small to zoomed in after zoomed out
	auto min_widget_side_after_zoomOut = static_cast<int>(std::min(widget_size.width(), widget_size.height()) * zoomOut_factor);
	zoomOut_action_->setEnabled(
		static_cast<int>(min_widget_side_after_zoomOut * zoomIn_factor) > min_widget_side_after_zoomOut
	);
}

void MainWindow::zoomWidget(double zoom_factor) {
	scaleWidget(open_gl_widget_->size() * zoom_factor);

	// Keep scrolls on the same relative position
	adjustScrollBar(scroll_area_->horizontalScrollBar(), zoom_factor);
	adjustScrollBar(scroll_area_->verticalScrollBar(),   zoom_factor);
}

void MainWindow::scaleWidget(const QSize& size) {
	QSize widget_size(*fits_size_);
	widget_size.scale(size, Qt::KeepAspectRatio);
	open_gl_widget_->resize(widget_size);

	updateActions();
}

void MainWindow::adjustScrollBar(QScrollBar* scroll_bar, double zoom_factor) {
	scroll_bar->setValue(
			static_cast<int>(zoom_factor * scroll_bar->value() + ((zoom_factor - 1) * scroll_bar->pageStep()/2))
	);
}

void MainWindow::resizeEvent(QResizeEvent *event) {
	QMainWindow::resizeEvent(event);
	updateActions();
}