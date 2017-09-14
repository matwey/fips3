/*
 *  Copyright (C) 2017  Matwey V. Kornilov <matwey.kornilov@gmail.com>
 *                      Konstantin Malanchev <hombit@gmail.com>
 *
 *  This program is free software: you can redistribute it and/or modify it
 *  under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or (at
 *  your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful, but
 *  WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser
 *  General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <memory>

#include <QApplication>
#include <QDesktopServices>
#include <QDesktopWidget>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QListWidget>
#include <QMessageBox>

#include <application.h>
#include <mainwindow.h>

MouseMoveEventFilter::MouseMoveEventFilter(MousePositionWidget *mouse_position_widget, QObject* parent):
		QObject(parent),
		mouse_position_widget_(mouse_position_widget) {}

bool MouseMoveEventFilter::eventFilter(QObject* open_gl_widget, QEvent* event) {
	auto watched = static_cast<OpenGLWidget*>(open_gl_widget);
	switch (event->type()) {
		case QEvent::MouseMove: {
			auto mouse_event = static_cast<QMouseEvent *>(event);
			mouse_position_widget_->setPositionAndValue(watched->pixelFromWidgetCoordinate(mouse_event->pos()));
			return true;
		}
		default:
			return false;
	}
}

MainWindowState::MainWindowState(const QString& filename, bool watch):
	filename_(QFileInfo(filename).absoluteFilePath()),
	watcher_(new QFileSystemWatcher(this)) {

	std::unique_ptr<QFile> file{new QFile{filename}};
	if (!file->open(QIODevice::ReadOnly)) {
		throw MainWindow::FileOpenError(file->errorString());
	}
	fits_.reset(new FITS{file.release()});

	if (!fits_->first_hdu().data().imageDataUnit()) {
		throw MainWindow::NoImageInFITS();
	}

	connect(watcher_, SIGNAL(fileChanged(const QString&)), this, SLOT(fileChanged(const QString&)));

	setWatch(watch);
}

bool MainWindowState::isWatched() const {
	return watcher_->files().contains(filename_);
}
bool MainWindowState::setWatch(bool watch) {
	if (watch == isWatched())
		return true;

	if (watch) {
		return watcher_->addPath(filename_);
	} else {
		return watcher_->removePath(filename_);
	}
}
void MainWindowState::fileChanged(const QString& path) {
	Q_ASSERT(path == filename_);

	emit fileChanged();
}

MainWindow::Exception::Exception(const QString& what):
	Utils::Exception(what) {
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

MainWindow::MainWindow(const QString& fits_filename, QWidget *parent):
	QMainWindow(parent),
	state_{new MainWindowState{fits_filename, false}} {

	connect(state_.get(), SIGNAL(fileChanged()), this, SLOT(refresh()));

	// Resize window to fit FITS image
	const auto desktop_size = QApplication::desktop()->screenGeometry();
	const QSize maximum_initial_window_size(desktop_size.width() * 2 / 3, desktop_size.height() * 2 / 3);
	resize(state_->fits().first_hdu().data().imageDataUnit()->size().boundedTo(maximum_initial_window_size));

	// Create scroll area and put there open_gl_widget
	std::unique_ptr<ScrollZoomArea> scroll_zoom_area{new ScrollZoomArea(this, state_->fits().first_hdu())};
	/* setCentralWidget promises to take ownership */
	setCentralWidget(scroll_zoom_area.release());

	std::unique_ptr<QMenuBar> menu_bar{new QMenuBar(this)};
	// File menu
	auto file_menu = menu_bar->addMenu(tr("&File"));
	auto file_open_here_action = file_menu->addAction(tr("Open in this &window"), this, SLOT(openFileHere()));
	file_open_here_action->setShortcut(QKeySequence::Open);
	auto file_open_action = file_menu->addAction(tr("&Open in new window"), Application::instance(), SLOT(openFile(void)));
	file_open_action->setShortcut(tr("ctrl+shift+o"));
	auto file_close_action = file_menu->addAction(tr("&Close"), this, SLOT(close()));
	file_close_action->setShortcut(QKeySequence::Close);
	file_menu->addSeparator();
	auto refresh_action = file_menu->addAction(tr("&Refresh file"), this, SLOT(refresh()));
	QList<QKeySequence> refresh_shortcuts {QKeySequence::Refresh, QKeySequence(Qt::Key_F5)};
	refresh_action->setShortcuts(refresh_shortcuts);
	auto autorefresh_action = file_menu->addAction(tr("Auto refresh"));
	autorefresh_action->setShortcut(tr("Ctrl+R"));
	autorefresh_action->setCheckable(true);
	connect(autorefresh_action, SIGNAL(toggled(bool)), this, SLOT(setAutoRefresh(bool)));
	// View menu
	auto view_menu = menu_bar->addMenu(tr("&View"));
	auto zoomIn_action = view_menu->addAction(tr("Zoom &In"), this, SLOT(zoomIn(void)));
	zoomIn_action->setShortcut(QKeySequence::ZoomIn);
	auto zoomOut_action = view_menu->addAction(tr("Zoom &Out"), this, SLOT(zoomOut(void)));
	zoomOut_action->setShortcut(QKeySequence::ZoomOut);
	auto fit_to_window_action = view_menu->addAction(tr("&Fit to Window"), this, SLOT(fitToWindow(void)));
	fit_to_window_action->setShortcut(tr("Ctrl+F"));
	view_menu->addSeparator();
	// To be continued in docks block
	// Help menu
	auto help_menu = menu_bar->addMenu(tr("&Help"));
	help_menu->addAction(tr("&About"), this, SLOT(about()));
	help_menu->addAction(tr("&Homepage"), this, SLOT(homepage()));
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

	std::unique_ptr<QDockWidget> colormap_dock{new QDockWidget(tr("Color map"), this)};
	colormap_dock->setAllowedAreas(Qt::AllDockWidgetAreas);
	view_menu->addAction(colormap_dock->toggleViewAction());
	colormap_dock->toggleViewAction()->setShortcut(tr("Ctrl+E"));
	std::unique_ptr<ColorMapWidget> colormap_widget{new ColorMapWidget(colormap_dock.get(), *scrollZoomArea()->viewport())};
	connect(
			colormap_widget->buttonGroup(), SIGNAL(buttonClicked(int)),
			scrollZoomArea()->viewport(), SLOT(changeColorMap(int))
	);
	colormap_dock->setWidget(colormap_widget.release());
	addDockWidget(Qt::RightDockWidgetArea, colormap_dock.release());

	std::unique_ptr<QStatusBar> status_bar{new QStatusBar(this)};
	std::unique_ptr<MousePositionWidget> mouse_position_widget{new MousePositionWidget(this)};
	mouse_move_event_filter_.reset(new MouseMoveEventFilter(mouse_position_widget.get(), this));
	scrollZoomArea()->viewport()->installEventFilter(mouse_move_event_filter_.get());
	status_bar->addWidget(mouse_position_widget.release());
	setStatusBar(status_bar.release());
	setWindowTitle(state_->filename());
}

void MainWindow::setWindowTitle(const QString& filename) {
#ifdef Q_OS_MAC
	QMainWindow::setWindowTitle(QFileInfo(filename).fileName());
#else
	QMainWindow::setWindowTitle(QFileInfo(filename).fileName() + " — FIPS");
#endif
}

void MainWindow::setState(MainWindowState* new_state) {
	return setState(std::unique_ptr<MainWindowState>{new_state});
}

void MainWindow::setState(std::unique_ptr<MainWindowState>&& new_state) {
	if (new_state.get() == state_.get())
		return;

	scrollZoomArea()->viewport()->setHDU(new_state->fits().first_hdu());
	setWindowTitle(new_state->filename());

	state_ = std::move(new_state);
	connect(state_.get(), SIGNAL(fileChanged()), this, SLOT(refresh()));
}

void MainWindow::openFileHere() {
	const auto filename = QFileDialog::getOpenFileName(this, tr("Open FITS file in current window"));

	if (filename.isEmpty())
		return;

	try {
		setState(new MainWindowState{filename, state_->isWatched()});
	} catch (const std::exception& e) {
		QMessageBox::critical(this, "An error occured", e.what());
	}
}

void MainWindow::refresh() {
	try {
		setState(new MainWindowState{state_->filename(), state_->isWatched()});
	} catch (const FITS::UnexpectedEnd&) {
		/* When the file is being overwritten, QFileSystemWatcher is
		 * quick enough to report multiple times. Just ignore if file
		 * is not ready yet.
		 */
	} catch (const std::exception& e) {
		QMessageBox::critical(this, "An error occured", e.what());
	}
}

void MainWindow::setAutoRefresh(bool autorefresh) {
	state_->setWatch(autorefresh);
}

void MainWindow::zoomIn() {
	scrollZoomArea()->zoomViewport(zoomIn_factor_);
}

void MainWindow::zoomOut() {
	scrollZoomArea()->zoomViewport(zoomOut_factor_);
}

void MainWindow::fitToWindow() {
	scrollZoomArea()->fitToViewport();
}

void MainWindow::closeEvent(QCloseEvent *event) {
	emit closed(*this);
	QMainWindow::closeEvent(event);
}

void MainWindow::resizeEvent(QResizeEvent *event) {
	QMainWindow::resizeEvent(event);
}

void MainWindow::about() {
	QMessageBox::about(
			this,
			tr("Fips"),
			tr("<b>Fips.</b> Copyright © 2017 Matwey Kornilov, Konstantin Malanchev. ") + QString(homePageURL())
	);
}

void MainWindow::homepage() {
	QDesktopServices::openUrl(QUrl(homePageURL()));
}
