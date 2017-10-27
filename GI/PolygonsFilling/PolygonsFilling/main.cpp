#include "glwidget.h"
#include"MainWindow.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainWindow win;
	win.show();
	return a.exec();
}
