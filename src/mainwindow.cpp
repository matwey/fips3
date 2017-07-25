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

	zoomIn_action_.reset (view_menu_->addAction(tr("Zoom &In (25%)"),  this, &MainWindow::zoomIn));
	zoomIn_action_->setShortcut (QKeySequence::ZoomIn);

	zoomOut_action_.reset(view_menu_->addAction(tr("Zoom &Out (25%)"), this, &MainWindow::zoomOut));
	zoomOut_action_->setShortcut(QKeySequence::ZoomOut);
}

void MainWindow::zoomIn() {
	zoom(1.25);
}

void MainWindow::zoomOut() {
	zoom(0.80);
}

void MainWindow::zoom(double zoom_factor) {
	scale_factor_ *= zoom_factor;

	// Resize OpenGL widget
	auto widget_size = QSize(*fits_size_);
	widget_size.scale(widget_size * scale_factor_, Qt::KeepAspectRatio);
	open_gl_widget_->resize(widget_size);

	// Keep scrolls on the same relative position
	adjustScrollBar(scroll_area_->horizontalScrollBar(), zoom_factor);
	adjustScrollBar(scroll_area_->verticalScrollBar(),   zoom_factor);
}

void MainWindow::adjustScrollBar(QScrollBar* scroll_bar, double zoom_factor) {
	scroll_bar->setValue(
			static_cast<int>(zoom_factor * scroll_bar->value() + ((zoom_factor - 1) * scroll_bar->pageStep()/2))
	);
}

double MainWindow::scale_factor() {
	return scale_factor_;
}