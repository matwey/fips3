#ifndef _MAINWINDOW_H_
#define _MAINWINDOW_H_

#include <QMainWindow>

#include <memory>

#include <openglwidget.h>

class MainWindow: public QMainWindow {
private:
	const QString fits_filename_;
	std::unique_ptr<OpenGLWidget> open_gl_widget_;
public:
	MainWindow();
};

#endif // _MAINWINDOW_H_
