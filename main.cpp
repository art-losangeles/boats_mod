//
// C++ Implementation: main
//
// Description:
//
//
// Author: Thibaut GRIDEL <tgridel@free.fr>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include <iostream>

#include <QApplication>

#include "mainwindow.h"

int debugLevel = 0;

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

	int i;
	QStringList arguments = QCoreApplication::arguments();
	if (-1 != (i=arguments.indexOf("-debug"))) {
		debugLevel = arguments[i+1].toInt();
		std::cout << "debug level set to " << debugLevel << std::endl;
	}
    // MainWindow
    MainWindow window;
    window.show();

    return app.exec();
}
