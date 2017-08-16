#include <application.h>
#include <instance.h>
#include <mainwindow.h>

#include <QCommandLineParser>
#include <QFileDialog>

Application::Application(int &argc, char **argv):
	QApplication(argc, argv) {

	QCommandLineParser parser;
	parser.addHelpOption();
	parser.addPositionalArgument("file", QCoreApplication::translate("main", "The file to open."));
	parser.process(*this);

	const QStringList args = parser.positionalArguments();

	if (args.length() == 0) {
		QString filename = QFileDialog::getOpenFileName(Q_NULLPTR, QObject::tr("Open FITS file"));
		if (filename.isEmpty()) QApplication::exit(0);

		addInstance(filename);
	} else if (args.length() >= 1) {
		for (const auto& x: args) addInstance(x);
	} else {
		parser.showHelp(1);
		QApplication::exit(1);
	}

}
Application::~Application() = default;

void Application::addInstance(const QString& filename) {
	new Instance(&root_, filename);
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
