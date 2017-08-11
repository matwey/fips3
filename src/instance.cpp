#include <instance.h>

void Instance::mainWindowClosed(MainWindow& mainwindow) {
	Q_ASSERT(&mainwindow == mainwindow_.get());

	deleteLater();
}

/* Ownership model specification.
 *
 * Instance objects are ownered by Application's root_ QObject via QObject
 * ownership mechanism. MainWindow is ownered by Instance using
 * std::unique_ptr. When all windows are closed, Application terminates and
 * deletes all its children and all windows are deleted by unique_ptr.
 */
Instance::Instance(QObject* parent, const QString& filename):
	QObject(parent), mainwindow_(new MainWindow(filename)) {

	mainwindow_->show();

	connect(mainwindow_.get(), SIGNAL(closed(MainWindow&)), this, SLOT(mainWindowClosed(MainWindow&)));
}
Instance::~Instance() = default;
