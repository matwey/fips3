#include <mainwindow.h>

MainWindow::MainWindow():
	QMainWindow(),
	open_gl_widget_(new OpenGLWidget(this)) {
}
