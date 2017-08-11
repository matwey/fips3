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
private slots:
	void mainWindowClosed(MainWindow& mainwindow);
public:
	Instance(QObject* parent, const QString& filename);
	virtual ~Instance() override;
};

#endif // _INSTANCE_H
