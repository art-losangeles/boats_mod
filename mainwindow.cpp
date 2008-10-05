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

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent),
        situation(new SituationModel(this)),
        scene(new SituationScene(situation)),
        view(new QGraphicsView(scene)),
        menubar(new QMenuBar(this)),
        toolbar(new QToolBar(this)),
        situationDock(new QDockWidget(this)),
        statusbar(new QStatusBar(this)),
        timeline(new QTimeLine(1000,this)) {

    // Actions
    createActions();

    // Scene
    connect(scene, SIGNAL(stateChanged(SceneState)),
            this, SLOT(changeState(SceneState)));

    // View
    view->setRenderHint(QPainter::Antialiasing);
    view->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    view->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    setCentralWidget(view);

    // Timeline
    timeline->setCurveShape(QTimeLine::LinearCurve);
    timeline->setLoopCount(0);

    // Bars
    createMenus();
    setMenuBar(menubar);
    addToolBar(toolbar);
    setStatusBar(statusbar);

    // Docks
    createDocks();
    addDockWidget(Qt::LeftDockWidgetArea, situationDock);

    readSettings();
    setCurrentFile("");
}

MainWindow::~MainWindow() {}

void MainWindow::createActions() {
    openFileAction = new QAction(QIcon(":/images/fileopen.png"), tr("&Open File"), this);
    openFileAction->setShortcut(tr("Ctrl+O"));
    connect(openFileAction, SIGNAL(triggered()),
            this, SLOT(openFile()));

    saveFileAction = new QAction(QIcon(":/images/filesave.png"), tr("&Save File"), this);
    saveFileAction->setShortcut(tr("Ctrl+S"));
    connect(saveFileAction, SIGNAL(triggered()),
            this, SLOT(saveFile()));

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

    animateAction = new QAction(tr("&Animate"), this);
    animateAction->setShortcut(tr("Ctrl+A"));
    connect(animateAction, SIGNAL(triggered()),
            this, SLOT(animate()));

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
}

void MainWindow::changeState(SceneState newState) {
    switch(newState) {
        case CREATE_TRACK:
            view->setCursor(Qt::CrossCursor);
            statusbar->showMessage(tr("CREATE_TRACK"));
            addTrackAction->setChecked(true);
            addBoatAction->setChecked(false);
            addMarkAction->setChecked(false);
            break;
        case CREATE_BOAT:
            view->setCursor(Qt::CrossCursor);
            statusbar->showMessage(tr("CREATE_BOAT"));
            addTrackAction->setChecked(false);
            addBoatAction->setChecked(true);
            addMarkAction->setChecked(false);
            break;
        case CREATE_MARK:
            view->setCursor(Qt::CrossCursor);
            statusbar->showMessage(tr("CREATE_MARK"));
            addTrackAction->setChecked(false);
            addBoatAction->setChecked(false);
            addMarkAction->setChecked(true);
            break;
        default:
            view->unsetCursor();
            statusbar->clearMessage();
            addTrackAction->setChecked(false);
            addBoatAction->setChecked(false);
            addMarkAction->setChecked(false);
    }
}

void MainWindow::cleanState(bool state) {
    saveFileAction->setEnabled(!state);
    setWindowModified(!state);
}

void MainWindow::createMenus() {
    fileMenu = menubar->addMenu(tr("&File"));
    fileMenu->addAction(openFileAction);
    fileMenu->addAction(saveFileAction);

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

    toolbar->addAction(openFileAction);
    toolbar->addAction(saveFileAction);
    toolbar->addSeparator();
    toolbar->addAction(undoAction);
    toolbar->addAction(redoAction);
    toolbar->addSeparator();
    toolbar->addAction(addTrackAction);
    toolbar->addAction(addBoatAction);
    toolbar->addAction(addMarkAction);
}

void MainWindow::createDocks() {
    QGroupBox *scenarioGroup = new QGroupBox(tr("Scenario"),situationDock);
    QFormLayout *situationForm = new QFormLayout(scenarioGroup);

    QSpinBox *laylineSpin = new QSpinBox(scenarioGroup);
    laylineSpin->setRange(0, 359);
    laylineSpin->setWrapping(true);
    laylineSpin->setValue(situation->laylineAngle());
    connect (laylineSpin, SIGNAL(valueChanged(int)),
            situation, SLOT(setLaylineAngle(int)));
    situationForm->addRow(new QLabel(tr("Laylines"),scenarioGroup),laylineSpin);

    QComboBox *seriesCombo = new QComboBox(scenarioGroup);
    seriesCombo->addItems(situation->seriesNames());
    seriesCombo->setCurrentIndex(situation->situationSeries());
    connect (seriesCombo, SIGNAL(currentIndexChanged(int)),
            situation, SLOT(setSeries(int)));
    situationForm->addRow(new QLabel(tr("Series"),scenarioGroup),seriesCombo);

    situationDock->setWidget(scenarioGroup);
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
        setCurrentFile(fileName);
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
    setCurrentFile(fileName);
    statusbar->showMessage(tr("File saved"), 2000);
}

void MainWindow::setCurrentFile(const QString &fileName)
 {
    situation->setFileName(fileName);
    situation->undoStack()->setClean();

    QString shownName = QFileInfo(fileName).fileName();
    setWindowTitle(tr("%1 - %2 [*]").arg(tr("Boats Scenario")).arg(shownName));
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

void MainWindow::animate() {
    if(scene->state() != ANIMATE) {
        scene->setState(ANIMATE);
        scene->setAnimation(timeline);
        timeline->start();
    } else {
        scene->setState(NO_STATE);
        scene->unSetAnimation();
        timeline->stop();
    }
}
