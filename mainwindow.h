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

/**
    \class MainWindow

    \brief The Main Window

    This class initialises a lot of objects. They are roughly sorted
    into three categories:
    - The GraphicsView objects
    - The Widgets available from the main window
    - The QActions triggered in the program

    There are many slot methods which will be connected to QAction signals

*/
class MainWindow : public QMainWindow {
        Q_OBJECT
    public:
        MainWindow(QWidget *parent = 0);
        ~MainWindow();
    public slots:
        // State management
        void changeState(SceneState newState);
        void cleanState(bool state);

        // File actions
        void newFile();
        void openFile();
        void openFile(const QString &fileName);
        bool saveFile();
        bool saveAs();
        void exportImage();

        // Track actions
        void addTrack();
        void addBoat();
        void addMark();
        void toggleMarkZone();
        void deleteTrack();
        void deleteModels();

        // Animation actions
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

        // Initialisation methods
        void createActions();
        void createMenus();
        void createDocks();

        // Configuration methods
        void writeSettings();
        void readSettings();
        bool maybeSave();

        // File methods
        bool saveFile(QString &fileName);
        void setCurrentFile(const QString &fileName);

        // GraphicsView Framework
        SituationModel *situation;
        SituationScene *scene;
        SituationView *view;

        // Widgets on the window
        QMenuBar *menubar;
        QToolBar *toolbar;
        QToolBar *animationBar;
        QDockWidget *situationDock;
        SituationWidget *situationWidget;
        QStatusBar *statusbar;
        QTimeLine *timeline;
        QSlider *animationSlider;

        // QActions
        QAction *newFileAction;
        QAction *openFileAction;
        QAction *saveFileAction;
        QAction *saveAsAction;
        QAction *exportImageAction;
        QAction *exitAction;

        QAction *addTrackAction;
        QAction *addBoatAction;
        QAction *addMarkAction;
        QAction *toggleMarkZoneAction;
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

        // QMenu
        QMenu *fileMenu;
        QMenu *trackMenu;
        QMenu *historyMenu;
        QMenu *zoomMenu;
        QMenu *animationMenu;
};

#endif
