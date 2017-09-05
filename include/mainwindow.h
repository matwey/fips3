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
#include <QMainWindow>
#include <QMenuBar>
#include <QString>

#include <memory>

#include <levelswidget.h>
#include <colormapwidget.h>
#include <scrollzoomarea.h>
#include <utils/exception.h>

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

	QString fits_filename_;
	std::unique_ptr<FITS> fits_;
	std::unique_ptr<FITS> loadFITS(const QString& fits_filename) const;
	void openInThisWindow(const QString& fits_filename);
	void updateWindowTitle();
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
	void refresh();
	void zoomIn();
	void zoomOut();
	void fitToWindow();
	void about();
	void homepage();
signals:
	void closed(MainWindow& mainwindow);
};

#endif // _MAINWINDOW_H_
