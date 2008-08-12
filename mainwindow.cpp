//
// C++ Implementation: mainwindow
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

#include <QStatusBar>
#include <QAction>
#include <QMenuBar>

#include "mainwindow.h"

#include "model/situationmodel.h"
#include "model/trackmodel.h"
#include "model/boatmodel.h"

#include "undocommands.h"

#include "boat.h"
#include "situationscene.h"
#include "trackwidget.h"

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent),
        situation(new SituationModel(this)),
        scene(new SituationScene(situation)),
        view(new QGraphicsView(scene)),
        menubar(new QMenuBar(this)),
        toolbar(new TrackWidget(this)),
        undoStack(new QUndoStack(this)) {

    // Actions
    createActions();

    // View
    view->setRenderHint(QPainter::Antialiasing);
    setCentralWidget(view);

    // Bars
    createMenus();
    setMenuBar(menubar);
    addToolBar(toolbar);
    setStatusBar(new QStatusBar);
}

MainWindow::~MainWindow() {}

void MainWindow::createActions() {
    addTrackAction = new QAction(tr("Create &Track"), this);
    addTrackAction->setShortcut(tr("Ctrl+Ins"));
    connect(addTrackAction, SIGNAL(triggered()),
            this, SLOT(addTrack()));

    deleteTrackAction = new QAction(tr("Delete Track"), this);
    deleteTrackAction->setShortcut(tr("Ctrl+Del"));
    connect(deleteTrackAction, SIGNAL(triggered()),
            this, SLOT(deleteTrack()));

    addBoatAction = new QAction(tr("Create &Boat"), this);
    addBoatAction->setShortcut(tr("Ins"));
    connect(addBoatAction, SIGNAL(triggered()),
            this, SLOT(addBoat()));

    deleteBoatAction = new QAction(tr("&Delete Boats"), this);
    deleteBoatAction->setShortcut(tr("Del"));
    connect(deleteBoatAction, SIGNAL(triggered()),
            this, SLOT(deleteBoat()));

    undoAction = new QAction(tr("&Undo"), this);
    undoAction->setShortcut(tr("Ctrl+Z"));
    undoAction->setEnabled(false);
    connect(undoAction, SIGNAL(triggered()),
            undoStack, SLOT(undo()));
    connect(undoStack, SIGNAL(canUndoChanged(bool)),
            undoAction, SLOT(setEnabled(bool)));

    redoAction = new QAction(tr("&Redo"), this);
    QList<QKeySequence> redoShortcuts;
    redoShortcuts << tr("Ctrl+Y") << tr("Shift+Ctrl+Z");
    redoAction->setShortcuts(redoShortcuts);
    redoAction->setEnabled(false);
    connect(redoAction, SIGNAL(triggered()),
            undoStack, SLOT(redo()));
    connect(undoStack, SIGNAL(canRedoChanged(bool)),
            redoAction, SLOT(setEnabled(bool)));
}

void MainWindow::createMenus() {
    trackMenu = menubar->addMenu(tr("&Track"));
    trackMenu->addAction(addTrackAction);
    trackMenu->addAction(deleteTrackAction);
    trackMenu->addAction(addBoatAction);
    trackMenu->addAction(deleteBoatAction);

    historyMenu = menubar->addMenu(tr("&History"));
    historyMenu->addAction(undoAction);
    historyMenu->addAction(redoAction);
}

void MainWindow::addTrack() {
    undoStack->push(new AddTrackUndoCommand(situation));
}

void MainWindow::deleteTrack() {
    // TODO trick to delete first selected track
    if (!scene->selectedItems().isEmpty()) {
        BoatModel *boat = static_cast<BoatGraphicsItem*>(scene->selectedItems()[0])->boat();
        TrackModel * track = boat->track();
        undoStack->push(new DeleteTrackUndoCommand(situation, track));
    }
}

void MainWindow::addBoat() {
    // TODO trick to add to first selected track
    if (!scene->selectedItems().isEmpty()) {
        BoatModel *boat = static_cast<BoatGraphicsItem*>(scene->selectedItems()[0])->boat();
        TrackModel * track = boat->track();
        undoStack->push(new AddBoatUndoCommand(track));
    }
}

void MainWindow::deleteBoat() {
    if (scene->selectedItems().isEmpty())
        return;
    foreach(BoatModel *boat, scene->selectedModels()) {
        TrackModel* track = boat->track();
        undoStack->push(new DeleteBoatUndoCommand(track, boat));
    }
}
