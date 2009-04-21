//
// C++ Interface: mainwindow
//
// Description:
//
//
// Author: Thibaut GRIDEL <tgridel@free.fr>
//
// Copyright (c) 2008-2009 Thibaut GRIDEL
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
//
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>

#include "situationwidget.h"
#include "situationscene.h"
#include "situationview.h"
#include "situationprint.h"

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
        virtual QMenu *createPopupMenu(){return 0;}

    public slots:
        // State management
        void changeState(SceneState newState);
        void cleanState(bool state);
        void updateActions();

        // File actions
        void newTab();
        void setTab(int index);
        void removeTab();
        void newFile();
        void restoreFiles();
        void openFile();
        void openFiles(QStringList fileList);
        void openFile(const QString &fileName, bool inNewTab = false);
        void openRecent();
        bool saveFile();
        bool saveAs();
        void print();
        void printPreview();
        void exportPdf();
        void exportImage();
#ifdef GIF_EXPORT
        void exportAnimation();
#endif
        // Track actions
        void addTrack();
        void addBoat();
        void addMark();
        void togglePortOverlap();
        void toggleStarboardOverlap();
        void toggleMarkZone();
        void deleteTrack();
        void deleteModels();

        // Animation actions
        void animate(bool state, bool interactive=true);
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
        void unsetTab();
        void writeSettings();
        void readSettings();
        void updateRecentList();
        bool maybeSave(SituationModel *situation);

        // File methods
        bool saveSituation(SituationModel *situation, QString name);
        void setCurrentFile(SituationModel *situation, const QString &fileName);

        // GraphicsView Framework
        QList<SituationModel *> situationList;
        QList<SituationScene *> sceneList;
        QList<SituationView *> viewList;
        int currentSituation;
        static const int maxRecent;
        QStringList recentList;
        QStringList fileList;

        // Widgets on the window
        QMenuBar *menubar;
        QToolBar *toolbar;
        QTabWidget *tabWidget;
        QPushButton *newTabButton;
        QPushButton *removeTabButton;
        QToolBar *animationBar;
        QDockWidget *situationDock;
        SituationWidget *situationWidget;
        QStatusBar *statusbar;
        QTimeLine *timeline;
        QSlider *animationSlider;

        // QActions
        QAction *newFileAction;
        QAction *openFileAction;
        QAction *restoreFilesAction;
        QAction *saveFileAction;
        QAction *saveAsAction;
        QAction *newTabAction;
        QAction *removeTabAction;
        QAction *printAction;
        QAction *printPreviewAction;
        QAction *exportPdfAction;
        QAction *exportImageAction;
#ifdef GIF_EXPORT
        QAction *exportAnimationAction;
#endif
        QAction *exitAction;

        QAction *addTrackAction;
        QAction *addBoatAction;
        QAction *addMarkAction;
        QAction *togglePortOverlapAction;
        QAction *toggleStarboardOverlapAction;
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

        QAction *toggleMainToolbarAction;
        QAction *toggleAnimationToolbarAction;
        QAction *toggleScenarioDockAction;

        QAction *aboutAction;

        // QMenu
        QMenu *fileMenu;
        QMenu *recentMenu;
        QMenu *trackMenu;
        QMenu *historyMenu;
        QMenu *zoomMenu;
        QMenu *animationMenu;
        QMenu *viewMenu;
};

#endif
