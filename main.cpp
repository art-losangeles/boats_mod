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

#include <QtCore>

#include "boatapplication.h"
#include "mainwindow.h"

int debugLevel = 0;


int main(int argc, char *argv[]) {
    BoatApplication app(argc, argv);

    int i;
    QStringList arguments = QCoreApplication::arguments();
    arguments.removeAt(0);
    if (-1 != (i=arguments.indexOf("-debug"))) {
        debugLevel = arguments[i+1].toInt();
        std::cout << "debug level set to " << debugLevel << std::endl;
        arguments.removeAt(i+1);
        arguments.removeAt(i);
    }

    // Locale and translation setup
    QString locale = QLocale::system().name();

    QTranslator translator;
    translator.load(QString(":/locale/boats_") + locale);
    app.installTranslator(&translator);

    // MainWindow
    MainWindow window;
    app.setWindow(&window);
    window.show();
    window.openFiles(arguments);

    return app.exec();
}
