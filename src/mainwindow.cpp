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

	menu_bar_.reset(new QMenuBar(nullptr));
	view_menu_.reset(menu_bar_->addMenu(tr("&View")));

	zoomIn_action_.reset (view_menu_->addAction(tr("Zoom &In"),  this, &MainWindow::zoomIn));
	zoomIn_action_->setShortcut (QKeySequence::ZoomIn);

	zoomOut_action_.reset(view_menu_->addAction(tr("Zoom &Out"), this, &MainWindow::zoomOut));
	zoomOut_action_->setShortcut(QKeySequence::ZoomOut);

	fitToWindow_action_.reset(view_menu_->addAction(tr("&Fit to Window"), this, &MainWindow::fitToWindow));
	fitToWindow_action_->setShortcut(tr("Ctrl+F"));
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
}

void MainWindow::adjustScrollBar(QScrollBar* scroll_bar, double zoom_factor) {
	scroll_bar->setValue(
			static_cast<int>(zoom_factor * scroll_bar->value() + ((zoom_factor - 1) * scroll_bar->pageStep()/2))
	);
}
