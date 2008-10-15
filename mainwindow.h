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
        void newFile();
        void openFile();
        bool saveFile();
        bool saveAs();
        void setLayline(int angle);
        void setSeries(int series);
        void addTrack();
        void addBoat();
        void addMark();
        void deleteTrack();
        void deleteModels();
        void animate();
        void play();
        void pause(bool pause);
        void stop();
        void changeAnimationState(QTimeLine::State newState);

    protected:
        void closeEvent(QCloseEvent *event);
    private:
        void createActions();
        void createMenus();
        void createDocks();
        void writeSettings();
        void readSettings();
        bool maybeSave();
        bool saveFile(QString &fileName);
        void setCurrentFile(const QString &fileName);

        SituationModel *situation;
        SituationScene *scene;
        QGraphicsView *view;
        QMenuBar *menubar;
        QToolBar *toolbar;
        QToolBar *animationBar;
        QDockWidget *situationDock;
        QStatusBar *statusbar;
        QTimeLine *timeline;
        QSlider *animationSlider;

        QAction *newFileAction;
        QAction *openFileAction;
        QAction *saveFileAction;
        QAction *saveAsAction;
        QAction *exitAction;

        QAction *addTrackAction;
        QAction *addBoatAction;
        QAction *addMarkAction;
        QAction *deleteTrackAction;
        QAction *deleteAction;

        QAction *animateAction;
        QAction *startAction;
        QAction *pauseAction;
        QAction *stopAction;

        QAction *undoAction;
        QAction *redoAction;

        QMenu *fileMenu;
        QMenu *trackMenu;
        QMenu *historyMenu;
        QMenu *animationMenu;
};

#endif
