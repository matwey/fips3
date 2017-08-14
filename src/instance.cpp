#include <instance.h>

/* Ownership model specification.
 *
 * Instance objects are ownered by Application via QObject ownership mechanism.
 * MainWindow is ownered by Instance using std::unique_ptr. When all windows
 * are closed, Application terminates and deletes all its children and all
 * windows are deleted by unique_ptr.
 */
Instance::Instance(Application* app, const QString& filename):
	QObject(app), mainwindow_(new MainWindow(filename)) {

	mainwindow_->show();
}
Instance::~Instance() = default;
