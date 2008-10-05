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

#include <QtGui>

#include "situationscene.h"

class SituationModel;
class TrackWidget;

class MainWindow : public QMainWindow {
        Q_OBJECT
    public:
        MainWindow(QWidget *parent = 0);
        ~MainWindow();
    public slots:
        void changeState(SceneState newState);
        void cleanState(bool state);
        void openFile();
        void saveFile();
        void addTrack();
        void addBoat();
        void addMark();
        void deleteTrack();
        void deleteModels();
        void animate();

    protected:
        void closeEvent(QCloseEvent *event);
    private:
        void createActions();
        void createMenus();
        void writeSettings();
        void readSettings();
        void setCurrentFile(const QString &fileName);

        SituationModel *situation;
        SituationScene *scene;
        QGraphicsView *view;
        QMenuBar *menubar;
        QToolBar *toolbar;
        QStatusBar *statusbar;
        QTimeLine *timeline;

        QAction *openFileAction;
        QAction *saveFileAction;

        QAction *addTrackAction;
        QAction *addBoatAction;
        QAction *addMarkAction;
        QAction *deleteTrackAction;
        QAction *deleteAction;

        QAction *animateAction;

        QAction *undoAction;
        QAction *redoAction;

        QMenu *fileMenu;
        QMenu *trackMenu;
        QMenu *historyMenu;
};

#endif
