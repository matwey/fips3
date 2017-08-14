#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include <QApplication>
#include <QString>

class Application:
	public QApplication {
public:
	Application(int &argc, char **argv);
	virtual ~Application() override;

	void addInstance(const QString& filename);
};

#endif // _APPLICATION_H_
