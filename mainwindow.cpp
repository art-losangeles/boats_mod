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

#include "boat.h"
#include "situationscene.h"
#include "trackwidget.h"

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent),
        situation(new SituationModel(this)),
        scene(new SituationScene(situation)),
        view(new QGraphicsView(scene)),
        menubar(new QMenuBar(this)),
        toolbar(new TrackWidget(this)) {

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
    addTrackAction = new QAction(tr("&Create Track"), this);
    addTrackAction->setShortcut(tr("Ins"));
    connect(addTrackAction, SIGNAL(triggered()),
            situation, SLOT(addTrack()));

    deleteBoatAction = new QAction(tr("&Delete Boats"), this);
    deleteBoatAction->setShortcut(tr("Del"));
    connect(deleteBoatAction, SIGNAL(triggered()),
            scene, SLOT(deleteBoatItem()));
}

void MainWindow::createMenus() {
    trackMenu = menubar->addMenu(tr("&Track"));
    trackMenu->addAction(addTrackAction);
    trackMenu->addAction(deleteBoatAction);
}
