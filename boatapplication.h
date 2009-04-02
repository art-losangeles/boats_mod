//
// C++ Interface: BoatApplication
//
// Description:
//
//
// Author: Thibaut GRIDEL <tgridel@free.fr>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef BOATAPPLICATION_H
#define BOATAPPLICATION_H

#include <QApplication>
#include <QObject>

class MainWindow;

class BoatApplication : public QApplication {
    Q_OBJECT
public:
    BoatApplication(int & argc, char ** argv) :
    QApplication(argc, argv) {}
    ~BoatApplication() {}
    void setWindow(MainWindow *window) { m_window = window; }

protected:
    bool event(QEvent *event);

private:
    MainWindow *m_window;
};

#endif // BOATAPPLICATION_H
