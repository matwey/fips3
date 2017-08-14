#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include <memory>

#include <QApplication>

#include <mainwindow.h>

class Application:
	public QApplication {
private:
	std::unique_ptr<MainWindow> mainwindow_;
public:
	Application(int &argc, char **argv);
	virtual ~Application() override;
};

#endif // _APPLICATION_H_
