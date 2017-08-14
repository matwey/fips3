#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include <QApplication>

class Application:
	public QApplication {
private:

public:
	Application(int &argc, char **argv);
	virtual ~Application() override;
};

#endif // _APPLICATION_H_
