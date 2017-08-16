#include <QCommandLineParser>
#include <QFileDialog>
#include <QMessageBox>
#include <QTimer>
#include <QWindow>

#include <application.h>
#include <instance.h>
#include <mainwindow.h>

Application::Application(int &argc, char **argv):
	QApplication(argc, argv) {

	QCommandLineParser parser;
	parser.addHelpOption();
	parser.addPositionalArgument("file", QCoreApplication::translate("main", "The file to open."));
	parser.process(*this);

	const QStringList args = parser.positionalArguments();

	if (args.length() == 0) {
#ifdef Q_OS_MAC
		QTimer::singleShot(0, this, [this] () { if (root_.children().length() == 0) openFile(); });
#else
		openFile();
#endif // Q_OS_MAC
	} else {
		for (const auto& x: args) addInstance(x);
	}

}
Application::~Application() = default;

void Application::addInstance(const QString& filename) {
	new Instance(&root_, filename);
}

void Application::openFile() {
	QString filename = QFileDialog::getOpenFileName(Q_NULLPTR, tr("Open FITS file"));

	if (filename.isEmpty()) return;

	try {
		Application::instance()->addInstance(filename);
	} catch (const std::exception& e) {
		QMessageBox::critical(Q_NULLPTR, "An error occured", e.what());
	}
}

#ifdef Q_OS_MAC
bool Application::event(QEvent *event) {
	if (event->type() == QEvent::FileOpen) {
		auto *openEvent = static_cast<QFileOpenEvent *>(event);
		addInstance(openEvent->file());
	}

	return QApplication::event(event);
}
#endif
