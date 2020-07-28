// ForegroundExtractionApp.cpp : Defines the entry point for the application.
//

#include "ForegroundExtractionApp.h"

using namespace std;

int main(int argc, char** argv)
{
	cout << "Hello CMake." << endl;
	QApplication app(argc, argv);
	MainWindow mainWindow;
	mainWindow.show();
	return app.exec();
}
