#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include <QApplication>
#include <QEvent>
#include <QMenuBar>
#include <QString>

class Application:
	public QApplication {
	Q_OBJECT
private:
	QObject root_;
public:
	Application(int &argc, char **argv);
	virtual ~Application() override;

	void addInstance(const QString& filename);
	inline static Application* instance() {
		return static_cast<Application*>(QCoreApplication::instance());
	}

#ifdef Q_OS_MAC
	virtual bool event(QEvent* event) override;
#endif
public slots:
	static void openFile();
};

#endif // _APPLICATION_H_
