#ifndef _INSTANCE_H
#define _INSTANCE_H

#include <memory.h>

#include <QString>

#include <application.h>
#include <mainwindow.h>

class Instance:
	public QObject {

	Q_OBJECT
private:
	std::unique_ptr<MainWindow> mainwindow_;
public:
	Instance(Application* app, const QString& filename);
	virtual ~Instance() override;
};

#endif // _INSTANCE_H
