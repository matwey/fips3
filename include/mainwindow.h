#ifndef _MAINWINDOW_H_
#define _MAINWINDOW_H_

#include <QMainWindow>
#include <QScrollArea>

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
	std::unique_ptr<QScrollArea>  scroll_area_;
	std::unique_ptr<OpenGLWidget> open_gl_widget_;
public:
	MainWindow(const QString& fits_filename);
};

#endif // _MAINWINDOW_H_
