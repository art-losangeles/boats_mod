//
// C++ Interface: mainwindow
//
// Description:
//
//
// Author: Thibaut GRIDEL <tgridel@free.fr>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>

class SituationModel;
class SituationScene;
class TrackWidget;

class MainWindow : public QMainWindow {
        Q_OBJECT
    public:
        MainWindow(QWidget *parent = 0);
        ~MainWindow();
    private:
        void createActions();
        void createMenus();

        SituationModel *situation;
        SituationScene *scene;
        QGraphicsView *view;
        QMenuBar *menubar;
        TrackWidget *toolbar;

        QAction *addTrackAction;
        QAction *deleteTrackAction;
        QAction *addBoatAction;
        QAction *deleteBoatAction;
        QAction *changeHeadingAction;

        QMenu *trackMenu;
};

#endif
