#ifndef _MAINWINDOW_H_
#define _MAINWINDOW_H_

#include <QDockWidget>
#include <QMainWindow>
#include <QMenuBar>
#include <QString>

#include <memory>

#include <exception.h>
#include <levelswidget.h>
#include <palettewidget.h>
#include <scrollzoomarea.h>

class MainWindow:
	public QMainWindow {
	Q_OBJECT
public:
	class Exception: public ::Exception {
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

	std::unique_ptr<FITS> fits_;
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
	void zoomIn();
	void zoomOut();
	void fitToWindow();
	void about();
	void homepage();
signals:
	void closed(MainWindow& mainwindow);
};

#endif // _MAINWINDOW_H_
