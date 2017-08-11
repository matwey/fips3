#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include <QApplication>
#include <QString>

class Application:
	public QApplication {
private:
	QObject root_;
public:
	Application(int &argc, char **argv);
	virtual ~Application() override;

	void addInstance(const QString& filename);
	inline static Application* instance() {
		return static_cast<Application*>(QCoreApplication::instance());
	}
};

#endif // _APPLICATION_H_
