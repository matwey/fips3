#ifndef _MAINWINDOW_H_
#define _MAINWINDOW_H_

#include <QMainWindow>
#include <QMenuBar>
#include <QScrollArea>
#include <QScrollBar>

#include <memory>

#include <exception.h>
#include <openglwidget.h>

class MainWindow:
	public QMainWindow {
public:
	class Exception: public ::Exception {
	public:
		virtual void raise() const override;
		virtual QException* clone() const override;
	};

	class FileOpenError: public Exception {
	public:
		FileOpenError(const QString& reason);

		virtual void raise() const override;
		virtual QException* clone() const override;
	};
private:
	std::unique_ptr<QMenuBar> menu_bar_;
	std::unique_ptr<QMenu> view_menu_;
	std::unique_ptr<QAction> zoomIn_action_;
	std::unique_ptr<QAction> zoomOut_action_;

	std::unique_ptr<QScrollArea>  scroll_area_;
	std::unique_ptr<OpenGLWidget> open_gl_widget_;

	double scale_factor_ = 1.0;
	std::unique_ptr<QSize> fits_size_;

	void zoomIn();
	void zoomOut();
	void zoom(double zoom_factor);
	void adjustScrollBar(QScrollBar *scrollBar, double zoom_factor);
public:
	MainWindow(const QString& fits_filename);
	double scale_factor();
};

#endif // _MAINWINDOW_H_
