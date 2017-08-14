#include <application.h>

#include <QCommandLineParser>
#include <QFileDialog>

Application::Application(int &argc, char **argv):
	QApplication(argc, argv) {

	QCommandLineParser parser;
	parser.addHelpOption();
	parser.addPositionalArgument("file", QCoreApplication::translate("main", "The file to open."));
	parser.process(*this);

	const QStringList args = parser.positionalArguments();

	QString filename;

	if (args.length() == 0) {
		filename = QFileDialog::getOpenFileName(Q_NULLPTR, QObject::tr("Open FITS file"));
	} else if (args.length() == 1) {
		filename = args[0];
	} else {
		parser.showHelp(1);
		QApplication::exit(1);
	}

	mainwindow_.reset(new MainWindow(filename));
	mainwindow_->show();
}
Application::~Application() = default;
