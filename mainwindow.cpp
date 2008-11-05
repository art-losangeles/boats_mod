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

#include <QtGui>

#include "mainwindow.h"

#include "commontypes.h"
#include "model/situationmodel.h"
#include "model/trackmodel.h"
#include "model/boatmodel.h"

#include "undocommands.h"
#include "xmlsituationreader.h"
#include "xmlsituationwriter.h"

#include "situationwidget.h"
#include "situationscene.h"
#include "situationview.h"

extern int debugLevel;

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent),
        situation(new SituationModel(this)),
        scene(new SituationScene(situation)),
        view(new SituationView(scene)),
        menubar(new QMenuBar(this)),
        toolbar(new QToolBar(this)),
        animationBar(new QToolBar(this)),
        situationDock(new QDockWidget(this)),
        situationWidget(new SituationWidget(situationDock)),
        statusbar(new QStatusBar(this)),
        timeline(new QTimeLine(1000,this)) {

    // Actions
    createActions();

    // Scene
    connect(scene, SIGNAL(stateChanged(SceneState)),
            this, SLOT(changeState(SceneState)));

    // Timeline
    timeline->setCurveShape(QTimeLine::LinearCurve);
    //timeline->setLoopCount(0);
    connect(timeline, SIGNAL(stateChanged(QTimeLine::State)),
            this, SLOT(changeAnimationState(QTimeLine::State)));

    // Bars
    createMenus();
    setMenuBar(menubar);
    addToolBar(toolbar);
    addToolBar(Qt::BottomToolBarArea, animationBar);
    setStatusBar(statusbar);

    // Docks
    createDocks();
    addDockWidget(Qt::LeftDockWidgetArea, situationDock);

    // View
    view->setRenderHints( QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    view->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    view->setResizeAnchor(QGraphicsView::AnchorViewCenter);
    setCentralWidget(view);
    view->setFocus();

    readSettings();
    setCurrentFile("");
}

MainWindow::~MainWindow() {}

void MainWindow::createActions() {
    newFileAction = new QAction(QIcon(":/images/filenew.png"), tr("&New File"), this);
    newFileAction->setShortcut(tr("Ctrl+N"));
    connect(newFileAction, SIGNAL(triggered()),
            this, SLOT(newFile()));

    openFileAction = new QAction(QIcon(":/images/fileopen.png"), tr("&Open File"), this);
    openFileAction->setShortcut(tr("Ctrl+O"));
    connect(openFileAction, SIGNAL(triggered()),
            this, SLOT(openFile()));

    saveFileAction = new QAction(QIcon(":/images/filesave.png"), tr("&Save File"), this);
    saveFileAction->setShortcut(tr("Ctrl+S"));
    saveFileAction->setEnabled(false);
    connect(saveFileAction, SIGNAL(triggered()),
            this, SLOT(saveFile()));

    saveAsAction = new QAction(QIcon(":/images/filesaveas.png"), tr("Save &As..."), this);
    saveAsAction->setShortcut(tr("Ctrl+Shift+S"));
    saveAsAction->setEnabled(false);
    connect(saveAsAction, SIGNAL(triggered()),
            this, SLOT(saveAs()));

    exitAction = new QAction(tr("E&xit"), this);
    exitAction->setShortcut(tr("Ctrl+Q"));
    exitAction->setStatusTip(tr("Exit the application"));
    connect(exitAction, SIGNAL(triggered()),
        this, SLOT(close()));

    addTrackAction = new QAction(QIcon(":/images/addtrack.png"), tr("Create &Track"), this);
    addTrackAction->setShortcut(tr("Ctrl+Ins"));
    addTrackAction->setCheckable(true);
    connect(addTrackAction, SIGNAL(triggered()),
            this, SLOT(addTrack()));

    addBoatAction = new QAction(QIcon(":/images/addboat.png"), tr("Create &Boat"), this);
    addBoatAction->setShortcut(tr("Ins"));
    addBoatAction->setCheckable(true);
    connect(addBoatAction, SIGNAL(triggered()),
            this, SLOT(addBoat()));

    addMarkAction = new QAction(QIcon(":/images/addmark.png"), tr("Create &Mark"), this);
    addMarkAction->setShortcut(tr("Alt+Ins"));
    addMarkAction->setCheckable(true);
    connect(addMarkAction, SIGNAL(triggered()),
            this, SLOT(addMark()));

    deleteTrackAction = new QAction(tr("Delete Track"), this);
    deleteTrackAction->setShortcut(tr("Ctrl+Del"));
    connect(deleteTrackAction, SIGNAL(triggered()),
            this, SLOT(deleteTrack()));

    deleteAction = new QAction(tr("&Delete Selection"), this);
    deleteAction->setShortcut(tr("Del"));
    connect(deleteAction, SIGNAL(triggered()),
            this, SLOT(deleteModels()));

    animateAction = new QAction(QIcon(":/images/animate.png"), tr("&Animate"), this);
    animateAction->setShortcut(tr("Ctrl+A"));
    animateAction->setCheckable(true);
    connect(animateAction, SIGNAL(toggled(bool)),
            this, SLOT(animate(bool)));

    startAction  = new QAction(QIcon(":/images/player_play.png"), tr("&Play"), this);
    startAction->setShortcut(tr("P"));
    startAction->setEnabled(false);
    connect(startAction, SIGNAL(triggered()),
            this, SLOT(play()));

    pauseAction  = new QAction(QIcon(":/images/player_pause.png"), tr("&Pause"), this);
    pauseAction->setShortcut(tr("M"));
    pauseAction->setEnabled(false);
    pauseAction->setCheckable(true);
    connect(pauseAction, SIGNAL(toggled(bool)),
            this, SLOT(pause(bool)));

    stopAction = new QAction(QIcon(":/images/player_stop.png"), tr("&Stop"), this);
    stopAction->setShortcut(tr("Space"));
    stopAction->setEnabled(false);
    connect(stopAction, SIGNAL(triggered()),
            this, SLOT(stop()));

    undoAction = new QAction(QIcon(":/images/undo.png"), tr("&Undo"), this);
    undoAction->setShortcut(tr("Ctrl+Z"));
    undoAction->setEnabled(false);
    connect(undoAction, SIGNAL(triggered()),
            situation->undoStack(), SLOT(undo()));
    connect(situation->undoStack(), SIGNAL(canUndoChanged(bool)),
            undoAction, SLOT(setEnabled(bool)));

    redoAction = new QAction(QIcon(":/images/redo.png"), tr("&Redo"), this);
    QList<QKeySequence> redoShortcuts;
    redoShortcuts << tr("Ctrl+Y") << tr("Shift+Ctrl+Z");
    redoAction->setShortcuts(redoShortcuts);
    redoAction->setEnabled(false);
    connect(redoAction, SIGNAL(triggered()),
            situation->undoStack(), SLOT(redo()));
    connect(situation->undoStack(), SIGNAL(canRedoChanged(bool)),
            redoAction, SLOT(setEnabled(bool)));
    connect(situation->undoStack(), SIGNAL(cleanChanged(bool)),
            this, SLOT(cleanState(bool)));

    aboutAction = new QAction(tr("&About"), this);
    connect(aboutAction, SIGNAL(triggered()),
            this, SLOT(about()));
}

void MainWindow::changeState(SceneState newState) {
    switch(newState) {
        case CREATE_TRACK:
            view->setCursor(Qt::CrossCursor);
            statusbar->showMessage(tr("CREATE_TRACK"));
            addTrackAction->setChecked(true);
            addBoatAction->setChecked(false);
            addMarkAction->setChecked(false);
            animateAction->setChecked(false);
            break;
        case CREATE_BOAT:
            view->setCursor(Qt::CrossCursor);
            statusbar->showMessage(tr("CREATE_BOAT"));
            addTrackAction->setChecked(false);
            addBoatAction->setChecked(true);
            addMarkAction->setChecked(false);
            animateAction->setChecked(false);
            break;
        case CREATE_MARK:
            view->setCursor(Qt::CrossCursor);
            statusbar->showMessage(tr("CREATE_MARK"));
            addTrackAction->setChecked(false);
            addBoatAction->setChecked(false);
            addMarkAction->setChecked(true);
            animateAction->setChecked(false);
            break;
        case ANIMATE:
            statusbar->showMessage(tr("ANIMATE"));
            addTrackAction->setChecked(false);
            addBoatAction->setChecked(false);
            addMarkAction->setChecked(false);
            animateAction->setChecked(true);
            break;
        default:
            view->unsetCursor();
            statusbar->clearMessage();
            addTrackAction->setChecked(false);
            addBoatAction->setChecked(false);
            addMarkAction->setChecked(false);
            animateAction->setChecked(false);
    }
}

void MainWindow::cleanState(bool state) {
    if (situation->fileName().isEmpty())
        saveFileAction->setEnabled(false);
    else
        saveFileAction->setEnabled(!state);
    saveAsAction->setEnabled(!state);
    setWindowModified(!state);
}

void MainWindow::createMenus() {
    fileMenu = menubar->addMenu(tr("&File"));
    fileMenu->addAction(newFileAction);
    fileMenu->addAction(openFileAction);
    fileMenu->addAction(saveFileAction);
    fileMenu->addAction(saveAsAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);

    trackMenu = menubar->addMenu(tr("&Edit"));
    trackMenu->addAction(addTrackAction);
    trackMenu->addAction(deleteTrackAction);
    trackMenu->addSeparator();
    trackMenu->addAction(addBoatAction);
    trackMenu->addAction(addMarkAction);
    trackMenu->addAction(deleteAction);
    trackMenu->addSeparator();
    trackMenu->addAction(animateAction);

    historyMenu = menubar->addMenu(tr("&History"));
    historyMenu->addAction(undoAction);
    historyMenu->addAction(redoAction);

    animationMenu = menubar->addMenu(tr("&Animation"));
    animationMenu->addAction(startAction);
    animationMenu->addAction(pauseAction);
    animationMenu->addAction(stopAction);

    menubar->addAction(aboutAction);

    toolbar->addAction(newFileAction);
    toolbar->addAction(openFileAction);
    toolbar->addAction(saveFileAction);
    toolbar->addAction(saveAsAction);
    toolbar->addSeparator();
    toolbar->addAction(undoAction);
    toolbar->addAction(redoAction);
    toolbar->addSeparator();
    toolbar->addAction(addTrackAction);
    toolbar->addAction(addBoatAction);
    toolbar->addAction(addMarkAction);
    toolbar->addSeparator();
    toolbar->addAction(animateAction);


    animationSlider = new QSlider(Qt::Horizontal, this);
    animationSlider->setTickInterval(2000);
    animationSlider->setTickPosition(QSlider::TicksBelow);
    animationSlider->setSingleStep(400);
    animationSlider->setPageStep(2000);
    animationSlider->setEnabled(false);

    animationBar->addAction(startAction);
    animationBar->addAction(pauseAction);
    animationBar->addAction(stopAction);
    animationBar->addSeparator();
    animationBar->addWidget(animationSlider);
    connect(timeline, SIGNAL(frameChanged(int)),
            animationSlider, SLOT(setValue(int)));
    connect(animationSlider, SIGNAL(valueChanged(int)),
            timeline, SLOT(setCurrentTime(int)));
}

void MainWindow::createDocks() {
    situationWidget->setSituation(situation);
    situationDock->setWidget(situationWidget);
    situationDock->setFeatures(QDockWidget::NoDockWidgetFeatures);
}

void MainWindow::writeSettings() {
    QSettings settings("Boats");

    settings.beginGroup("MainWindow");
    settings.setValue("size", size());
    settings.setValue("pos", pos());
    settings.endGroup();
}

void MainWindow::readSettings() {
    QSettings settings("Boats");

    settings.beginGroup("MainWindow");
    resize(settings.value("size", QSize(400, 400)).toSize());
    move(settings.value("pos", QPoint(200, 200)).toPoint());
    settings.endGroup();
}

bool MainWindow::maybeSave() {
    if (!situation->undoStack()->isClean()) {
        QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(this, tr("Application"),
                    tr("The document has been modified.\n"
                        "Do you want to save your changes?"),
                    QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        if (ret == QMessageBox::Save) {
            return saveAs();
        } else if (ret == QMessageBox::Cancel) {
            return false;
        }
    }
    return true;
}

void MainWindow::closeEvent(QCloseEvent *event) {
    if (maybeSave()) {
        writeSettings();
        event->accept();
    } else
        event->ignore();
}

void MainWindow::newFile() {
    if (maybeSave()) {
        scene->setState(NO_STATE);
        situation->undoStack()->setIndex(0);
        situation->undoStack()->clear();
        situation->setTitle("");
        situation->setRules("");
        situationWidget->unSetSituation();
        situationWidget->setSituation(situation);
        setCurrentFile("");
        view->centerOn(0,0);
        view->resetMatrix();
    }
}

void MainWindow::openFile() {
    QString fileName =
             QFileDialog::getOpenFileName(this, tr("Open Scenario File"),
                                          QDir::currentPath(),
                                          tr("xmlscenario Files (*.xbs)"));
    if (fileName.isEmpty())
        return;

    // delete situation;
    newFile();

    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Open Scenario File"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return;
    }

    XmlSituationReader reader(situation);
    if (!reader.read(&file)) {
        QMessageBox::warning(this, tr("Open Scenario file"),
                             tr("Parse error in file %1 at line %2, column %3:\n%4")
                             .arg(fileName)
                             .arg(reader.lineNumber())
                             .arg(reader.columnNumber())
                             .arg(reader.errorString()));
    } else {
        situationWidget->update();
        setCurrentFile(fileName);
        view->centerOn(scene->itemsBoundingRect().center());
        statusbar->showMessage(tr("File loaded"), 2000);
    }
}

bool MainWindow::saveFile(QString &fileName) {
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Save Scenario"),
                            tr("Cannot write file %1:\n%2.")
                            .arg(fileName)
                            .arg(file.errorString()));
        return false;
    }
    XmlSituationWriter writer(situation);
    writer.writeFile(&file);
    setCurrentFile(fileName);
    statusbar->showMessage(tr("File saved"), 2000);
    return true;
}

bool MainWindow::saveFile() {
    QString fileName = situation->fileName();
    if (fileName.isEmpty()) {
        return saveAs();
    }
    return saveFile(fileName);
}

bool MainWindow::saveAs() {
    QString defaultFile = QDateTime::currentDateTime().toString("yyMMdd") + ".xbs";
    QString fileName =
            QFileDialog::getSaveFileName(this, tr("Save Scenario"),
                                         defaultFile,
                                         tr("xmlscenario Files (*.xbs)"));
    if (fileName.isEmpty()) {
        return false;
    }
    return saveFile(fileName);
}

void MainWindow::setCurrentFile(const QString &fileName) {
    situation->setFileName(fileName);
    situation->undoStack()->setClean();

    QString shownName = QFileInfo(fileName).fileName();
    setWindowTitle(tr("%1 - %2 [*]").arg(tr("Boat Scenario")).arg(shownName));
}

void MainWindow::addTrack() {
    if(scene->state() == CREATE_TRACK) {
        scene->setState(NO_STATE);
    } else {
        scene->setState(CREATE_TRACK);
    }
}

void MainWindow::deleteTrack() {
    // TODO trick to delete first selected track
    if (!scene->selectedBoatModels().isEmpty()) {
        BoatModel *boat = scene->selectedBoatModels()[0];
        TrackModel * track = boat->track();
        situation->undoStack()->push(new DeleteTrackUndoCommand(situation, track));
    }
}

void MainWindow::addBoat() {
    if (scene->state() == CREATE_BOAT) {
        scene->setState(NO_STATE);
    } else {
        scene->setState(CREATE_BOAT);
    }
}

void MainWindow::deleteModels() {
    foreach(BoatModel *boat, scene->selectedBoatModels()) {
        TrackModel* track = boat->track();
        situation->undoStack()->push(new DeleteBoatUndoCommand(track, boat));
    }
    foreach(MarkModel *mark, scene->selectedMarkModels()) {
        situation->undoStack()->push(new DeleteMarkUndoCommand(situation, mark));
    }
}

void MainWindow::addMark() {
    if (scene->state() == CREATE_MARK) {
        scene->setState(NO_STATE);
    } else {
        scene->setState(CREATE_MARK);
    }
}

void MainWindow::animate(bool state) {
    if (state) {
        if (scene->state() != ANIMATE) {
            scene->setState(ANIMATE);
            scene->setAnimation(timeline);
            animationSlider->setRange(0,timeline->duration());
            animationSlider->setEnabled(true);
            timeline->setFrameRange(0,timeline->duration());
            startAction->setEnabled(true);
        }
    } else {
        if (scene->state() == ANIMATE) {
            scene->setState(NO_STATE);
        }
        scene->unSetAnimation();
        animationSlider->setEnabled(false);
        timeline->stop();
        startAction->setEnabled(false);
        stopAction->setEnabled(false);
    }
}

void MainWindow::play() {
    if (debugLevel & 1 << ANIMATION) std::cout << "playing" << std::endl;
    animationSlider->blockSignals(true);
    pauseAction->setChecked(false);
    timeline->setCurrentTime(0);
    timeline->start();
}

void MainWindow::pause(bool pause) {
    if (pause) {
        if (debugLevel & 1 << ANIMATION) std::cout << "pausing" << std::endl;
        timeline->setPaused(true);
        animationSlider->blockSignals(false);
    } else {
        if (debugLevel & 1 << ANIMATION) std::cout << "resuming" << std::endl;
        animationSlider->blockSignals(true);
        timeline->resume();
    }
}

void MainWindow::stop() {
    if (debugLevel & 1 << ANIMATION) std::cout << "stopping" << std::endl;
    pauseAction->setChecked(false);
    timeline->stop();
    timeline->setCurrentTime(0);
    animationSlider->blockSignals(false);
}

void MainWindow::changeAnimationState(QTimeLine::State newState) {
    switch(newState) {
        case QTimeLine::Running:
            if (debugLevel & 1 << ANIMATION) std::cout << "state running" << std::endl;
            startAction->setEnabled(false);
            pauseAction->setEnabled(true);
            stopAction->setEnabled(true);
            break;

        case QTimeLine::Paused:
            if (debugLevel & 1 << ANIMATION) std::cout << "state paused" << std::endl;
            startAction->setEnabled(true);
            pauseAction->setEnabled(true);
            stopAction->setEnabled(true);
            break;

        case QTimeLine::NotRunning:
            if (debugLevel & 1 << ANIMATION) std::cout << "state not running" << std::endl;
            startAction->setEnabled(true);
            pauseAction->setEnabled(false);
            stopAction->setEnabled(false);
            break;
    }
}

void MainWindow::about() {
    QMessageBox::about(this, tr("About Boat Scenario"),
        tr("<center><img src=\":/images/about.png\">"
            "<p><b>Boat Scenario</b> - a Race Scenario drawing tool.</p>"
            "<p>Version %1</p></center>"
            "<p>Copyright (C) 2008 Thibaut GRIDEL </p>"
            "<p></p>"
            "<p>visit <a href=\"http://boats.berlios.de\">http://boats.berlios.de</a></p>"
            "<p>This program is free software; you can redistribute it and/or modify "
            "it under the terms of the GNU General Public License as published by "
            "the Free Software Foundation; either version 2 of the License, or "
            "(at your option) any later version.</p>"
            "<p></p>"
            "<p>This program is distributed in the hope that it will be useful, "
            "but WITHOUT ANY WARRANTY; without even the implied warranty of "
            "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the "
            "GNU General Public License for more details.</p>").arg(VERSION));
}
