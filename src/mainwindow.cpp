#include <memory>

#include <QFile>
#include <QFileInfo>
#include <QListWidget>

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
MainWindow::NoImageInFITS::NoImageInFITS():
	MainWindow::Exception("The file has no image content") {
}
void MainWindow::NoImageInFITS::raise() const {
	throw *this;
}
QException* MainWindow::NoImageInFITS::clone() const {
	return new MainWindow::NoImageInFITS(*this);
}

MainWindow::MainWindow(const QString& fits_filename): QMainWindow() {
	// Open FITS file
	std::unique_ptr<QFile> file{new QFile(fits_filename)};
	if (!file->open(QIODevice::ReadOnly)) {
		throw FileOpenError(file->errorString());
	}

	// Read FITS from file
	fits_.reset(new FITS(file.release()));
	const FITS::HeaderDataUnit* hdu = &fits_->primary_hdu();

	for (auto it = fits_->begin();
		it != fits_->end() && !hdu->data().imageDataUnit();
		++it) {

		hdu = &(*it);
	}

	if (!hdu->data().imageDataUnit()) {
		throw NoImageInFITS();
	}

	// Resize window to fit FITS image
	// FIXME: reduce window size if FITS larger than Desktop and change scale_factor_
	resize(hdu->data().imageDataUnit()->size());

	#ifdef Q_OS_MAC
		setWindowTitle(QFileInfo(fits_filename).fileName());
	#else
		setWindowTitle(QFileInfo(fits_filename).fileName() + " — FIPS");
	#endif

	// Create scroll area and put there open_gl_widget
	std::unique_ptr<ScrollZoomArea> scroll_zoom_area{new ScrollZoomArea(this, *hdu)};
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

	std::unique_ptr<QDockWidget> levels_dock{new QDockWidget(tr("Levels"), this)};
	levels_dock->setAllowedAreas(Qt::AllDockWidgetAreas);
	view_menu->addAction(levels_dock->toggleViewAction());
	levels_dock->toggleViewAction()->setShortcut(tr("Ctrl+L"));
	std::unique_ptr<LevelsWidget> levels_widget{new LevelsWidget(levels_dock.get())};
	connect(
			scrollZoomArea()->viewport(), SIGNAL(textureInitialized(const OpenGLTexture*)),
			levels_widget.get(), SLOT(notifyTextureInitialized(const OpenGLTexture*))
	);
	connect(
			levels_widget.get(), SIGNAL(valuesChanged(const std::pair<double, double>&)),
			scrollZoomArea()->viewport(), SLOT(changeLevels(const std::pair<double, double>&))
	);
	levels_dock->setWidget(levels_widget.release());
	addDockWidget(Qt::RightDockWidgetArea, levels_dock.release());
	levels_dock->setWidget(levels_widget.release());
	addDockWidget(Qt::RightDockWidgetArea, levels_dock.release());

	palette_dock_.reset(new QDockWidget(tr("Palette"), this));
	palette_dock_->setAllowedAreas(Qt::AllDockWidgetAreas);
	view_menu->addAction(palette_dock_->toggleViewAction());
	palette_dock_->toggleViewAction()->setShortcut(tr("Ctrl+E"));
	std::unique_ptr<PaletteWidget> palette_widget{new PaletteWidget(palette_dock_.get())};
	connect(
			palette_widget->buttonGroup(), SIGNAL(buttonClicked(int)),
			scrollZoomArea()->viewport(), SLOT(changePalette(int))
	);
	palette_dock_->setWidget(palette_widget.release());
	addDockWidget(Qt::RightDockWidgetArea, palette_dock_.get());
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
