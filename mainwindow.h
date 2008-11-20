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

#include "situationwidget.h"
#include "situationscene.h"
#include "situationview.h"

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
        void exportImage();
        void addTrack();
        void addBoat();
        void addMark();
        void deleteTrack();
        void deleteModels();
        void animate(bool state);
        void play();
        void pause(bool pause);
        void stop();
        void loop(bool loop);
        void changeAnimationState(QTimeLine::State newState);
        void about();

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
        SituationView *view;
        QMenuBar *menubar;
        QToolBar *toolbar;
        QToolBar *animationBar;
        QDockWidget *situationDock;
        SituationWidget *situationWidget;
        QStatusBar *statusbar;
        QTimeLine *timeline;
        QSlider *animationSlider;

        QAction *newFileAction;
        QAction *openFileAction;
        QAction *saveFileAction;
        QAction *saveAsAction;
        QAction *exportImageAction;
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
        QAction *loopAction;

        QAction *undoAction;
        QAction *redoAction;

        QAction *zoomInAction;
        QAction *zoomOutAction;
        QAction *zoomFitAction;

        QAction *aboutAction;

        QMenu *fileMenu;
        QMenu *trackMenu;
        QMenu *historyMenu;
        QMenu *zoomMenu;
        QMenu *animationMenu;
};

#endif
