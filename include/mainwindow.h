#ifndef _MAINWINDOW_H_
#define _MAINWINDOW_H_

#include <QMainWindow>
#include <QMenuBar>
#include <QString>

#include <memory>

#include <exception.h>
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
	static constexpr double zoomIn_factor  = 1.25;
	static constexpr double zoomOut_factor = 0.8;

	std::unique_ptr<FITS> fits_;
protected:
	virtual void resizeEvent(QResizeEvent* event) override;
public:
	MainWindow(const QString& fits_filename);

	inline ScrollZoomArea* scrollZoomArea() const {
		return static_cast<ScrollZoomArea*>(centralWidget());
	}
public slots:
	void zoomIn();
	void zoomOut();
};

#endif // _MAINWINDOW_H_
