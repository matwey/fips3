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

#ifndef _MAINWINDOW_H_
#define _MAINWINDOW_H_

#include <QDockWidget>
#include <QFileSystemWatcher>
#include <QMainWindow>
#include <QMouseEvent>
#include <QMenuBar>
#include <QStatusBar>
#include <QString>

#include <memory>

#include <colormapwidget.h>
#include <levelswidget.h>
#include <mousepositionwidget.h>
#include <scrollzoomarea.h>
#include <utils/exception.h>

class MouseMoveEventFilter: public QObject {
private:
	MousePositionWidget* mouse_position_widget_;
public:
	MouseMoveEventFilter(MousePositionWidget* mouse_position_widget, QObject* parent = Q_NULLPTR);
protected:
	virtual bool eventFilter(QObject* open_gl_widget, QEvent* event) override;
};

class MainWindowState:
	public QObject {
	Q_OBJECT
private:
	QString               filename_;
	std::unique_ptr<FITS> fits_;
	QFileSystemWatcher*   watcher_;

public:
	MainWindowState(const QString& filename, bool watch);

	bool isWatched() const;
	bool setWatch(bool watch);

	inline const QString& filename() const { return filename_; }
	inline const FITS&    fits()     const { return *fits_; }
public slots:
	void fileChanged(const QString& path);
signals:
	void fileChanged();
};

class MainWindow:
	public QMainWindow {
	Q_OBJECT
public:
	class Exception: public Utils::Exception {
	public:
		explicit Exception(const QString& what);

		virtual void raise() const override;
		virtual QException* clone() const override;
	};

	class FileOpenError: public Exception {
	public:
		explicit FileOpenError(const QString& reason);

		virtual void raise() const override;
		virtual QException* clone() const override;
	};

	class NoImageInFITS: public Exception {
	public:
		NoImageInFITS();

		virtual void raise() const override;
		virtual QException* clone() const override;
	};
private:
	static constexpr double zoomIn_factor_  = 1.25;
	static constexpr double zoomOut_factor_ = 0.8;
	static constexpr const char homepage_url_[] = "http://fips.space";

	std::unique_ptr<MainWindowState> state_;
	std::unique_ptr<MouseMoveEventFilter> mouse_move_event_filter_;
private:
	void setState(MainWindowState* state);
	void setState(std::unique_ptr<MainWindowState>&& state);

	void setWindowTitle(const QString& filename);
protected:
	virtual void resizeEvent(QResizeEvent* event) override;
	virtual void closeEvent(QCloseEvent *event) override;
public:
	MainWindow(const QString& fits_filename, QWidget *parent = Q_NULLPTR);

	inline ScrollZoomArea* scrollZoomArea() const {
		return static_cast<ScrollZoomArea*>(centralWidget());
	}
	inline const char* homePageURL() const { return "http://fips.space"; }
public slots:
	void openFileHere();
	void viewHeaders();
	void refresh();
	void setAutoRefresh(bool autorefresh);
	void zoomIn();
	void zoomOut();
	void fitToWindow();
	void about();
	void homepage();
signals:
	void closed(MainWindow& mainwindow);
};

#endif // _MAINWINDOW_H_
