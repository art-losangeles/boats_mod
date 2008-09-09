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

#include "model/situationmodel.h"
#include "model/trackmodel.h"
#include "model/boatmodel.h"

#include "undocommands.h"
#include "xmlsituationreader.h"
#include "xmlsituationwriter.h"

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
        statusbar(new QStatusBar(this)) {

    // Actions
    createActions();

    // View
    view->setRenderHint(QPainter::Antialiasing);
    view->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    view->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    setCentralWidget(view);

    // Bars
    createMenus();
    setMenuBar(menubar);
    addToolBar(toolbar);
    setStatusBar(statusbar);


    readSettings();
}

MainWindow::~MainWindow() {}

void MainWindow::createActions() {
    openFileAction = new QAction(tr("&Open File"), this);
    openFileAction->setShortcut(tr("Ctrl+O"));
    connect(openFileAction, SIGNAL(triggered()),
            this, SLOT(openFile()));

    saveFileAction = new QAction(tr("&Save File"), this);
    saveFileAction->setShortcut(tr("Ctrl+S"));
    connect(saveFileAction, SIGNAL(triggered()),
            this, SLOT(saveFile()));

    addTrackAction = new QAction(tr("Create &Track"), this);
    addTrackAction->setShortcut(tr("Ctrl+Ins"));
    connect(addTrackAction, SIGNAL(triggered()),
            this, SLOT(addTrack()));

    addBoatAction = new QAction(tr("Create &Boat"), this);
    addBoatAction->setShortcut(tr("Ins"));
    connect(addBoatAction, SIGNAL(triggered()),
            this, SLOT(addBoat()));

    addMarkAction = new QAction(tr("Create &Mark"), this);
    addMarkAction->setShortcut(tr("Alt+Ins"));
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

    undoAction = new QAction(tr("&Undo"), this);
    undoAction->setShortcut(tr("Ctrl+Z"));
    undoAction->setEnabled(false);
    connect(undoAction, SIGNAL(triggered()),
            situation->undoStack(), SLOT(undo()));
    connect(situation->undoStack(), SIGNAL(canUndoChanged(bool)),
            undoAction, SLOT(setEnabled(bool)));

    redoAction = new QAction(tr("&Redo"), this);
    QList<QKeySequence> redoShortcuts;
    redoShortcuts << tr("Ctrl+Y") << tr("Shift+Ctrl+Z");
    redoAction->setShortcuts(redoShortcuts);
    redoAction->setEnabled(false);
    connect(redoAction, SIGNAL(triggered()),
            situation->undoStack(), SLOT(redo()));
    connect(situation->undoStack(), SIGNAL(canRedoChanged(bool)),
            redoAction, SLOT(setEnabled(bool)));
}

void MainWindow::createMenus() {
    fileMenu = menubar->addMenu(tr("&File"));
    fileMenu->addAction(openFileAction);
    fileMenu->addAction(saveFileAction);

    trackMenu = menubar->addMenu(tr("&Track"));
    trackMenu->addAction(addTrackAction);
    trackMenu->addAction(addBoatAction);
    trackMenu->addAction(addMarkAction);
    trackMenu->addAction(deleteTrackAction);
    trackMenu->addAction(deleteAction);

    historyMenu = menubar->addMenu(tr("&History"));
    historyMenu->addAction(undoAction);
    historyMenu->addAction(redoAction);
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

void MainWindow::closeEvent(QCloseEvent *event) {
    writeSettings();
    event->accept();
}

void MainWindow::openFile()
{
    QString fileName =
             QFileDialog::getOpenFileName(this, tr("Open Situation File"),
                                          QDir::currentPath(),
                                          tr("xmlsituation Files (*.xboat *.xml)"));
    if (fileName.isEmpty())
        return;

//    delete situation;

    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Open Situation File"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return;
    }

    XmlSituationReader reader(situation);
    if (!reader.read(&file)) {
        QMessageBox::warning(this, tr("Open Situation file"),
                             tr("Parse error in file %1 at line %2, column %3:\n%4")
                             .arg(fileName)
                             .arg(reader.lineNumber())
                             .arg(reader.columnNumber())
                             .arg(reader.errorString()));
    } else {
        statusbar->showMessage(tr("File loaded"), 2000);
    }
}


void MainWindow::saveFile()
{
    QString fileName =
            QFileDialog::getSaveFileName(this, tr("Save Situation"),
                                         QDir::currentPath(),
                                         tr("Situation Files (*.xboat *.xml)"));
    if (fileName.isEmpty())
        return;

    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Save Situation"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return;
    }

    XmlSituationWriter writer(situation);
    writer.writeFile(&file);
    statusbar->showMessage(tr("File saved"), 2000);
}

void MainWindow::addTrack() {
    scene->setState(CREATE_TRACK);
    view->setCursor(Qt::CrossCursor);
    statusbar->showMessage(tr("CREATE_TRACK"));
}

void MainWindow::deleteTrack() {
    // TODO trick to delete first selected track
    if (!scene->selectedModels().isEmpty()) {
        BoatModel *boat = scene->selectedModels()[0];
        TrackModel * track = boat->track();
        situation->undoStack()->push(new DeleteTrackUndoCommand(situation, track));
    }
}

void MainWindow::addBoat() {
    if (scene->state() == CREATE_BOAT) {
        scene->setState(NO_STATE);
        view->unsetCursor();
        statusbar->clearMessage();
    } else {
        scene->setState(CREATE_BOAT);
        view->setCursor(Qt::CrossCursor);
        statusbar->showMessage(tr("CREATE_BOAT"));
    }
}

void MainWindow::deleteModels() {
    foreach(BoatModel *boat, scene->selectedModels()) {
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
        view->unsetCursor();
        statusbar->clearMessage();
    } else {
        scene->setState(CREATE_MARK);
        view->setCursor(Qt::CrossCursor);
        statusbar->showMessage(tr("CREATE_MARK"));
    }
}
