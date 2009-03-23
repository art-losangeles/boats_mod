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
#include "situationmodel.h"
#include "trackmodel.h"
#include "boatmodel.h"

#include "undocommands.h"
#include "xmlsituationreader.h"
#include "xmlsituationwriter.h"

#include "situationwidget.h"
#include "situationscene.h"
#include "situationview.h"

extern int debugLevel;

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent),
        menubar(new QMenuBar(this)),
        toolbar(new QToolBar(this)),
        tabWidget(new QTabWidget(this)),
        animationBar(new QToolBar(this)),
        situationDock(new QDockWidget(this)),
        situationWidget(new SituationWidget(situationDock)),
        statusbar(new QStatusBar(this)),
        timeline(new QTimeLine(1000,this)) {

    // Actions
    createActions();

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
    newTabButton = new QPushButton(QIcon(":/images/tab_new.png"),QString(""), this);
    connect(newTabButton, SIGNAL(clicked()),
            newTabAction, SLOT(trigger()));
    tabWidget->setCornerWidget(newTabButton, Qt::TopLeftCorner);
    removeTabButton = new QPushButton(QIcon(":/images/tab_remove.png"),QString(""), this);
    removeTabButton->setEnabled(removeTabAction->isEnabled());
    connect(removeTabButton, SIGNAL(clicked()),
            removeTabAction, SLOT(trigger()));
    tabWidget->setCornerWidget(removeTabButton, Qt::TopRightCorner);
    connect(tabWidget, SIGNAL(currentChanged(int)),
            this, SLOT(setTab(int)));
    newTab();
    setCentralWidget(tabWidget);

    readSettings();
}

MainWindow::~MainWindow() {}

void MainWindow::createActions() {
    newFileAction = new QAction(QIcon(":/images/filenew.png"), tr("&New File"), this);
    newFileAction->setShortcut(tr("Ctrl+N"));
    connect(newFileAction, SIGNAL(triggered()),
            this, SLOT(newFile()));

    openFileAction = new QAction(QIcon(":/images/fileopen.png"), tr("&Open File..."), this);
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

    newTabAction = new QAction(QIcon(":/images/tab_new.png"), tr("New &Tab"), this);
    newTabAction->setShortcut(tr("Ctrl+T"));
    connect(newTabAction, SIGNAL(triggered()),
            this, SLOT(newTab()));

    removeTabAction = new QAction(QIcon(":/images/tab_remove.png"), tr("&Close Tab"), this);
    removeTabAction->setShortcut(tr("Ctrl+W"));
    removeTabAction->setEnabled(false);
    connect(removeTabAction, SIGNAL(triggered()),
            this, SLOT(removeTab()));

    printAction = new QAction(QIcon(":images/fileprint.png"), tr("&Print..."), this);
    printAction->setShortcut(tr("Ctrl+p"));
    connect(printAction, SIGNAL(triggered()),
            this, SLOT(print()));

    printPreviewAction = new QAction(QIcon(":images/filequickprint.png"), tr("Print P&review..."), this);
    printPreviewAction->setShortcut(tr("Ctrl+r"));
    connect(printPreviewAction, SIGNAL(triggered()),
            this, SLOT(printPreview()));

    exportPdfAction = new QAction(QIcon(":/images/pdf.png"), tr("&Export Pdf..."), this);
    exportPdfAction->setShortcut(tr("Ctrl+E"));
    connect(exportPdfAction, SIGNAL(triggered()),
            this, SLOT(exportPdf()));

    exportImageAction = new QAction(QIcon(":/images/export.png"), tr("Export &Image..."), this);
    exportImageAction->setShortcut(tr("Ctrl+I"));
    connect(exportImageAction, SIGNAL(triggered()),
            this, SLOT(exportImage()));

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

    togglePortOverlapAction = new QAction(tr("&Port Overlap"), this);
    togglePortOverlapAction->setShortcut(tr("Alt+<"));
    connect(togglePortOverlapAction, SIGNAL(triggered()),
            this, SLOT(togglePortOverlap()));

    toggleStarboardOverlapAction = new QAction(tr("&Starboard Overlap"), this);
    toggleStarboardOverlapAction->setShortcut(tr("Alt+>"));
    connect(toggleStarboardOverlapAction, SIGNAL(triggered()),
            this, SLOT(toggleStarboardOverlap()));

    toggleMarkZoneAction = new QAction(QIcon(":/images/zone.png"), tr("Toggle Mark &Zone"), this);
    toggleMarkZoneAction->setShortcut(tr("Z"));
    connect(toggleMarkZoneAction, SIGNAL(triggered()),
            this, SLOT(toggleMarkZone()));

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

    loopAction = new QAction(QIcon(":/images/player_loop.png"), tr("&Loop"), this);
    loopAction->setShortcut(tr("L"));
    loopAction->setEnabled(false);
    loopAction->setCheckable(true);
    connect(loopAction, SIGNAL(toggled(bool)),
            this, SLOT(loop(bool)));

    undoAction = new QAction(QIcon(":/images/undo.png"), tr("&Undo"), this);
    undoAction->setShortcut(tr("Ctrl+Z"));
    undoAction->setEnabled(false);

    redoAction = new QAction(QIcon(":/images/redo.png"), tr("&Redo"), this);
    QList<QKeySequence> redoShortcuts;
    redoShortcuts << tr("Ctrl+Y") << tr("Shift+Ctrl+Z");
    redoAction->setShortcuts(redoShortcuts);
    redoAction->setEnabled(false);

    zoomInAction = new QAction(QIcon(":/images/zoomin.png"), tr("Zoom &In"), this);
    zoomInAction->setShortcut(tr("Ctrl++"));

    zoomOutAction = new QAction(QIcon(":/images/zoomout.png"), tr("Zoom &Out"), this);
    zoomOutAction->setShortcut(tr("Ctrl+-"));

    zoomFitAction = new QAction(QIcon(":/images/zoomfit.png"), tr("Zoom &Fit"), this);
    zoomFitAction->setShortcut(tr("Ctrl+F"));

    toggleMainToolbarAction = new QAction(tr("Main Toolbar"), this);
    toggleMainToolbarAction->setCheckable(true);
    toggleMainToolbarAction->setChecked(true);
    connect(toggleMainToolbarAction, SIGNAL(toggled(bool)),
            toolbar, SLOT(setVisible(bool)));

    toggleAnimationToolbarAction = new QAction(tr("Animation Toolbar"), this);
    toggleAnimationToolbarAction->setCheckable(true);
    toggleAnimationToolbarAction->setChecked(true);
    connect(toggleAnimationToolbarAction, SIGNAL(toggled(bool)),
            animationBar, SLOT(setVisible(bool)));

    toggleScenarioDockAction = new QAction(tr("Scenario Dock"), this);
    toggleScenarioDockAction->setCheckable(true);
    toggleScenarioDockAction->setChecked(true);
    connect(toggleScenarioDockAction, SIGNAL(toggled(bool)),
            situationDock, SLOT(setVisible(bool)));

    aboutAction = new QAction(tr("&About"), this);
    connect(aboutAction, SIGNAL(triggered()),
            this, SLOT(about()));
}

void MainWindow::changeState(SceneState newState) {
    SituationView *view = viewList.at(tabWidget->currentIndex());

    switch(newState) {
        case CREATE_TRACK:
            view->setCursor(Qt::CrossCursor);
            statusbar->showMessage(tr("Create Track"));
            addTrackAction->setChecked(true);
            addBoatAction->setChecked(false);
            addMarkAction->setChecked(false);
            animateAction->setChecked(false);
            break;
        case CREATE_BOAT:
            view->setCursor(Qt::CrossCursor);
            statusbar->showMessage(tr("Create Boat"));
            addTrackAction->setChecked(false);
            addBoatAction->setChecked(true);
            addMarkAction->setChecked(false);
            animateAction->setChecked(false);
            break;
        case CREATE_MARK:
            view->setCursor(Qt::CrossCursor);
            statusbar->showMessage(tr("Create Mark"));
            addTrackAction->setChecked(false);
            addBoatAction->setChecked(false);
            addMarkAction->setChecked(true);
            animateAction->setChecked(false);
            break;
        case ANIMATE:
            statusbar->showMessage(tr("Animate"));
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
    SituationModel *situation = situationList.at(tabWidget->currentIndex());
    if (situation->fileName().isEmpty())
        saveFileAction->setEnabled(false);
    else
        saveFileAction->setEnabled(!state);
    saveAsAction->setEnabled(!state);
    QString shownName = QFileInfo(situation->fileName()).fileName();
    setWindowTitle(tr("%1 - %2 [*]").arg(tr("Boat Scenario")).arg(shownName));
    if (!state) {
        tabWidget->setTabText(tabWidget->currentIndex(),shownName.append(" *"));
    } else {
        tabWidget->setTabText(tabWidget->currentIndex(),shownName);
    }
    setWindowModified(!state);
}

void MainWindow::createMenus() {
    fileMenu = menubar->addMenu(tr("&File"));
    fileMenu->addAction(newFileAction);
    fileMenu->addAction(openFileAction);
    fileMenu->addAction(saveFileAction);
    fileMenu->addAction(saveAsAction);
    fileMenu->addSeparator();
    fileMenu->addAction(printAction);
    fileMenu->addAction(printPreviewAction);
    fileMenu->addAction(exportPdfAction);
    fileMenu->addAction(exportImageAction);
    fileMenu->addSeparator();
    fileMenu->addAction(newTabAction);
    fileMenu->addAction(removeTabAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);

    trackMenu = menubar->addMenu(tr("&Edit"));
    trackMenu->addAction(addTrackAction);
    trackMenu->addAction(deleteTrackAction);
    trackMenu->addSeparator();
    trackMenu->addAction(addBoatAction);
    trackMenu->addAction(addMarkAction);
    trackMenu->addAction(togglePortOverlapAction);
    trackMenu->addAction(toggleStarboardOverlapAction);
    trackMenu->addAction(toggleMarkZoneAction);
    trackMenu->addAction(deleteAction);

    historyMenu = menubar->addMenu(tr("&History"));
    historyMenu->addAction(undoAction);
    historyMenu->addAction(redoAction);

    animationMenu = menubar->addMenu(tr("&Animation"));
    animationMenu->addAction(animateAction);
    animationMenu->addSeparator();
    animationMenu->addAction(startAction);
    animationMenu->addAction(pauseAction);
    animationMenu->addAction(stopAction);
    animationMenu->addAction(loopAction);

    zoomMenu = menubar->addMenu(tr("&Zoom"));
    zoomMenu->addAction(zoomInAction);
    zoomMenu->addAction(zoomFitAction);
    zoomMenu->addAction(zoomOutAction);

    viewMenu = menubar->addMenu(tr("&View"));
    viewMenu->addAction(toggleMainToolbarAction);
    viewMenu->addAction(toggleAnimationToolbarAction);
    viewMenu->addAction(toggleScenarioDockAction);

    menubar->addAction(aboutAction);

    toolbar->addAction(newFileAction);
    toolbar->addAction(openFileAction);
    toolbar->addAction(saveFileAction);
    toolbar->addAction(saveAsAction);
    toolbar->addAction(exportImageAction);
    toolbar->addSeparator();
    toolbar->addAction(undoAction);
    toolbar->addAction(redoAction);
    toolbar->addSeparator();
    toolbar->addAction(addTrackAction);
    toolbar->addAction(addBoatAction);
    toolbar->addAction(addMarkAction);
    toolbar->addSeparator();
    toolbar->addAction(animateAction);
    toolbar->addSeparator();
    toolbar->addAction(zoomOutAction);
    toolbar->addAction(zoomFitAction);
    toolbar->addAction(zoomInAction);


    animationSlider = new QSlider(Qt::Horizontal, this);
    animationSlider->setTickInterval(2000);
    animationSlider->setTickPosition(QSlider::TicksBelow);
    animationSlider->setSingleStep(400);
    animationSlider->setPageStep(2000);
    animationSlider->setEnabled(false);

    animationBar->addAction(startAction);
    animationBar->addAction(pauseAction);
    animationBar->addAction(stopAction);
    animationBar->addAction(loopAction);
    animationBar->addSeparator();
    animationBar->addWidget(animationSlider);
    connect(timeline, SIGNAL(frameChanged(int)),
            animationSlider, SLOT(setValue(int)));
    connect(animationSlider, SIGNAL(valueChanged(int)),
            timeline, SLOT(setCurrentTime(int)));
}

void MainWindow::createDocks() {
    QScrollArea *area = new QScrollArea(this);
    area->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    area->setWidgetResizable(true);
    area->setWidget(situationWidget);
    situationDock->setWidget(area);
    situationDock->setFeatures(QDockWidget::NoDockWidgetFeatures);
}

void MainWindow::newTab() {
    SituationModel *situation = new SituationModel(this);
    SituationScene *scene = new SituationScene(situation);
    SituationView *view =new SituationView(scene);

    situationList.append(situation);
    sceneList.append(scene);
    viewList.append(view);
    tabWidget->addTab(view, "");

    tabWidget->setCurrentIndex(situationList.size()-1);
    view->setFocus();
    if (situationList.size() > 1) {
        removeTabAction->setEnabled(true);
        removeTabButton->setEnabled(true);
    }
}

void MainWindow::unsetTab() {
    if (situationList.size() == 1) {
        return;
    }

    SituationModel *situation = situationList.at(currentSituation);
    SituationScene *scene = sceneList.at(currentSituation);
    if (scene->state() == ANIMATE) {
        scene->setState(NO_STATE);
    }

    disconnect(situation->undoStack(), 0, 0, 0);
    disconnect(scene, 0, this, 0);

    disconnect(undoAction, 0, 0, 0);
    disconnect(redoAction, 0, 0, 0);

    disconnect(zoomInAction, 0, 0, 0);
    disconnect(zoomOutAction, 0, 0, 0);
    disconnect(zoomFitAction, 0, 0, 0);

    situationWidget->unSetSituation();
}

void MainWindow::setTab(int index) {
    unsetTab();
    currentSituation = index;
    SituationModel *situation = situationList.at(index);
    SituationScene *scene = sceneList.at(index);
    SituationView *view = viewList.at(index);

    connect(undoAction, SIGNAL(triggered()),
            situation->undoStack(), SLOT(undo()));
    connect(situation->undoStack(), SIGNAL(canUndoChanged(bool)),
            undoAction, SLOT(setEnabled(bool)));
    undoAction->setEnabled(situation->undoStack()->canUndo()),

    connect(redoAction, SIGNAL(triggered()),
            situation->undoStack(), SLOT(redo()));
    connect(situation->undoStack(), SIGNAL(canRedoChanged(bool)),
            redoAction, SLOT(setEnabled(bool)));
    redoAction->setEnabled(situation->undoStack()->canRedo());

    connect(situation->undoStack(), SIGNAL(cleanChanged(bool)),
            this, SLOT(cleanState(bool)));
    cleanState(situation->undoStack()->isClean());

    connect(zoomInAction, SIGNAL(triggered()),
            view, SLOT(zoomIn()));
    connect(zoomOutAction, SIGNAL(triggered()),
            view, SLOT(zoomOut()));
    connect(zoomFitAction, SIGNAL(triggered()),
            view, SLOT(zoomFit()));

    situationWidget->setSituation(situation);
    connect(scene, SIGNAL(stateChanged(SceneState)),
            this, SLOT(changeState(SceneState)));
    changeState(scene->state());
}

void MainWindow::removeTab() {
    int index = tabWidget->currentIndex();
    SituationModel *situation = situationList.at(index);
    SituationScene *scene = sceneList.at(index);
    SituationView *view = viewList.at(index);

    if (!maybeSave(situation)) {
        return;
    }

    if (index == situationList.size() - 1) {
        tabWidget->setCurrentIndex(index - 1);
    } else if (index != situationList.size() - 2) {
        tabWidget->setCurrentIndex(index + 1);
    }
    situationList.removeAt(index);
    sceneList.removeAt(index);
    viewList.removeAt(index);

    tabWidget->removeTab(index);
    view->deleteLater();
    scene->deleteLater();
    situation->deleteLater();
    if (situationList.size() == 1) {
        removeTabAction->setEnabled(false);
        removeTabButton->setEnabled(false);
    }
}

void MainWindow::writeSettings() {
    QSettings settings("Boats");

    settings.beginGroup("MainWindow");
    settings.setValue("size", size());
    settings.setValue("pos", pos());
    settings.setValue("AnimationBar", animationBar->isVisible());
    settings.setValue("AnimationBarArea", toolBarArea(animationBar));
    settings.setValue("ToolBar", toolbar->isVisible());
    settings.setValue("ToolBarArea", toolBarArea(toolbar));
    settings.setValue("ScenarioDock", situationDock->isVisible());
    QStringList fileList;
    foreach(SituationModel *situation, situationList) {
        if (!situation->fileName().isEmpty()) {
            fileList.append(situation->fileName());
        }
    }
    settings.setValue("fileList",fileList);
    settings.endGroup();
}

void MainWindow::readSettings() {
    QSettings settings("Boats");

    settings.beginGroup("MainWindow");
    QSize size = settings.value("size").toSize();
    QPoint pos = settings.value("pos").toPoint();
    if ( (size.isValid()) && (!pos.isNull()) ) {
        resize(size);
        move(pos);
    } else {
        showMaximized();
    }
    toggleAnimationToolbarAction->setChecked(settings.value("AnimationBar", true).toBool());
    addToolBar((Qt::ToolBarArea)(settings.value("AnimationBarArea").toInt()), animationBar);
    toggleMainToolbarAction->setChecked(settings.value("ToolBar", true).toBool());
    addToolBar((Qt::ToolBarArea)(settings.value("ToolBarArea").toInt()), toolbar);
    toggleScenarioDockAction->setChecked(settings.value("ScenarioDock", true).toBool());
    QStringList fileList = settings.value("fileList").toStringList();
    openFiles(fileList);
    settings.endGroup();
}

bool MainWindow::maybeSave(SituationModel *situation) {
    if (!situation->undoStack()->isClean()) {
        QString shownName = QFileInfo(situation->fileName()).fileName();
        QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(this, shownName,
                    tr("The document has been modified.\n"
                        "Do you want to save your changes?"),
                    QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        if (ret == QMessageBox::Save) {
            return saveSituation(situation, "");
        } else if (ret == QMessageBox::Cancel) {
            return false;
        }
    }
    return true;
}

void MainWindow::closeEvent(QCloseEvent *event) {
    SituationScene *scene = sceneList.at(tabWidget->currentIndex());

    bool animated = (scene->state() == ANIMATE);
    if (animated) {
        scene->setState(NO_STATE);
    }
    foreach(SituationModel *situation, situationList) {
        if (!maybeSave(situation)) {
            event->ignore();
            return;
        }
    }
    writeSettings();
    event->accept();
}

void MainWindow::newFile() {
    SituationModel *situation = situationList.at(tabWidget->currentIndex());
    SituationScene *scene = sceneList.at(tabWidget->currentIndex());
    SituationView *view = viewList.at(tabWidget->currentIndex());

    if (maybeSave(situation)) {
        scene->setState(NO_STATE);
        situation->undoStack()->setIndex(0);
        setCurrentFile(situation, "");
        situation->undoStack()->clear();
        situationWidget->unSetSituation();
        situationWidget->setSituation(situation);
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

    openFile(fileName);
}

void MainWindow::openFiles(QStringList fileList) {
    foreach(SituationModel *situation, situationList) {
        if (!maybeSave(situation)) {
            return;
        }
    }
    foreach (const QString fileName, fileList) {
        std::cout << "opening " << fileName.toStdString() << std::endl;
        openFile(fileName, fileList.first() != fileName);
    }
}

void MainWindow::openFile(const QString &fileName, bool inNewTab) {
    if (inNewTab) {
        // create new tab
        newTab();
        tabWidget->setCurrentIndex(situationList.size() - 1);
    } else {
        // delete situation;
        newFile();
    }

    SituationModel *situation = situationList.at(tabWidget->currentIndex());
    SituationScene *scene = sceneList.at(tabWidget->currentIndex());
    SituationView *view = viewList.at(tabWidget->currentIndex());

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
        return;
    }
    situationWidget->update();
    setCurrentFile(situation, fileName);
    view->centerOn(scene->itemsBoundingRect().center());
    statusbar->showMessage(tr("File loaded"), 2000);
}

bool MainWindow::saveSituation(SituationModel *situation, QString fileName) {
    QString name = fileName;
    if (name.isEmpty()) {
        QString defaultFile;
        if (situation->fileName().isEmpty()) {
            defaultFile = QDateTime::currentDateTime().toString("yyMMdd") + ".xbs";
        } else {
            defaultFile = situation->fileName();
        }
        name = QFileDialog::getSaveFileName(this, tr("Save Scenario"),
                                            defaultFile,
                                            tr("xmlscenario Files (*.xbs)"));
        if (name.isEmpty()) {
            return false;
        }
    }

    QFile file(name);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Save Scenario"),
                            tr("Cannot write file %1:\n%2.")
                            .arg(fileName)
                            .arg(file.errorString()));
        return false;
    }
    XmlSituationWriter writer(situation);
    writer.writeFile(&file);
    setCurrentFile(situation, name);
    statusbar->showMessage(tr("File saved"), 2000);
    return true;
}

bool MainWindow::saveFile() {
    SituationModel *situation = situationList.at(tabWidget->currentIndex());
    SituationScene *scene = sceneList.at(tabWidget->currentIndex());

    bool animated = (scene->state() == ANIMATE);
    if (animated) {
        scene->setState(NO_STATE);
    }
    bool saved = saveSituation(situation, situation->fileName());
    if (animated) {
        animate(true);
    }
    return saved;
}

bool MainWindow::saveAs() {
    SituationModel *situation = situationList.at(tabWidget->currentIndex());
    SituationScene *scene = sceneList.at(tabWidget->currentIndex());

    bool animated = (scene->state() == ANIMATE);
    if (animated) {
        scene->setState(NO_STATE);
    }
    bool saved = saveSituation(situation, "");
    if (animated) {
        animate(true);
    }
    return saved;
}

void MainWindow::print() {
    SituationModel *situation = situationList.at(tabWidget->currentIndex());
    SituationView *view = viewList.at(tabWidget->currentIndex());
    QPrinter printer(QPrinter::HighResolution);

    QPrintDialog *dialog = new QPrintDialog(&printer, this);
    dialog->setWindowTitle(tr("Print Document"));
    if (dialog->exec() != QDialog::Accepted) {
        return;
    }

    SituationPrint printSituation(situation, view);
    printSituation.render(printer.pageRect(QPrinter::Millimeter));
    printSituation.print(&printer);
}

void MainWindow::printPreview() {
    SituationModel *situation = situationList.at(tabWidget->currentIndex());
    SituationView *view = viewList.at(tabWidget->currentIndex());
    SituationPrint printSituation(situation, view);
    QPrinter printer(QPrinter::HighResolution);
    QPrintPreviewDialog dialog(&printer);
    printSituation.render(printer.pageRect(QPrinter::Millimeter));

    connect(&dialog, SIGNAL(paintRequested(QPrinter*)),
            &printSituation, SLOT(print(QPrinter*)));
    dialog.exec();
}

void MainWindow::exportPdf() {
    SituationModel *situation = situationList.at(tabWidget->currentIndex());
    SituationView *view = viewList.at(tabWidget->currentIndex());

    QString defaultName(situation->fileName());
    defaultName.chop(4);
    QString fileName = QFileDialog::getSaveFileName(this, "Export to PDF",
                                                    defaultName, "PDF Files (*.pdf)");
    if (fileName.isEmpty()) {
        return;
    }
    if (QFileInfo(fileName).suffix().isEmpty())
        fileName.append(".pdf");
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);
    SituationPrint printSituation(situation, view);
    printSituation.render(printer.pageRect(QPrinter::Millimeter));
    printSituation.print(&printer);
}

void MainWindow::exportImage() {
    SituationModel *situation = situationList.at(tabWidget->currentIndex());
    SituationView *view = viewList.at(tabWidget->currentIndex());

    QString defaultName(situation->fileName());
    defaultName.chop(4);
    QList<QByteArray> formatsList = QImageWriter::supportedImageFormats();
    QString formats;
    for (int i=0; i<formatsList.size(); i++)
        formats.append(formatsList[i].toUpper()).append(" Files (*.").append(formatsList[i]).append(");;");
    QString ext;
    QString fileName =
            QFileDialog::getSaveFileName(this, tr("Save Scenario"),
                                         defaultName,
                                         formats,
                                         &ext);
    if (fileName.isEmpty()) {
        return;
    }

    // if no provided extension or incorrect extension, use selected filter
    if (!formatsList.contains(QFileInfo(fileName).suffix().toAscii())) {
        fileName.append(".").append(ext.left(ext.indexOf(' ')).toLower());
    }

    QPixmap pixmap = view->screenShot();
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Export Image"),
                            tr("Cannot write file %1:\n%2.")
                            .arg(fileName)
                            .arg(file.errorString()));
        return;
    }
    pixmap.save(fileName);
}

void MainWindow::setCurrentFile(SituationModel *situation, const QString &fileName) {
    situation->setFileName(fileName);
    situation->undoStack()->setClean();
}

void MainWindow::addTrack() {
    SituationScene *scene = sceneList.at(tabWidget->currentIndex());

    if(scene->state() == CREATE_TRACK) {
        scene->setState(NO_STATE);
    } else {
        scene->setState(CREATE_TRACK);
    }
}

void MainWindow::deleteTrack() {
    SituationModel *situation = situationList.at(tabWidget->currentIndex());
    SituationScene *scene = sceneList.at(tabWidget->currentIndex());

    // TODO trick to delete first selected track
    if (!scene->selectedBoatModels().isEmpty()) {
        BoatModel *boat = scene->selectedBoatModels()[0];
        TrackModel * track = boat->track();
        situation->undoStack()->push(new DeleteTrackUndoCommand(situation, track));
    }
}

void MainWindow::addBoat() {
    SituationScene *scene = sceneList.at(tabWidget->currentIndex());

    if (scene->state() == CREATE_BOAT) {
        scene->setState(NO_STATE);
    } else {
        scene->setState(CREATE_BOAT);
    }
}

void MainWindow::deleteModels() {
    SituationModel *situation = situationList.at(tabWidget->currentIndex());
    SituationScene *scene = sceneList.at(tabWidget->currentIndex());

    foreach (BoatModel *boat, scene->selectedBoatModels()) {
        TrackModel* track = boat->track();
        if (track->size() > 1) {
            situation->undoStack()->push(new DeleteBoatUndoCommand(track, boat));
        } else {
            situation->undoStack()->push(new DeleteTrackUndoCommand(situation, track));
        }
    }
    foreach (MarkModel *mark, scene->selectedMarkModels()) {
        situation->undoStack()->push(new DeleteMarkUndoCommand(situation, mark));
    }
}

void MainWindow::addMark() {
    SituationScene *scene = sceneList.at(tabWidget->currentIndex());

    if (scene->state() == CREATE_MARK) {
        scene->setState(NO_STATE);
    } else {
        scene->setState(CREATE_MARK);
    }
}

void MainWindow::togglePortOverlap() {
    SituationModel *situation = situationList.at(tabWidget->currentIndex());
    SituationScene *scene = sceneList.at(tabWidget->currentIndex());

    QList<BoatModel *> boatList = scene->selectedBoatModels();
    if (! boatList.isEmpty()) {
        situation->undoStack()->push(new OverlapBoatUndoCommand(situation, boatList, Boats::port));
    }
}

void MainWindow::toggleStarboardOverlap() {
    SituationModel *situation = situationList.at(tabWidget->currentIndex());
    SituationScene *scene = sceneList.at(tabWidget->currentIndex());

    QList<BoatModel *> boatList = scene->selectedBoatModels();
    if (! boatList.isEmpty()) {
        situation->undoStack()->push(new OverlapBoatUndoCommand(situation, boatList, Boats::starboard));
    }
}

void MainWindow::toggleMarkZone() {
    SituationModel *situation = situationList.at(tabWidget->currentIndex());
    SituationScene *scene = sceneList.at(tabWidget->currentIndex());

    QList<MarkModel *> markList = scene->selectedMarkModels();
    if (! markList.isEmpty()) {
        situation->undoStack()->push(new ZoneMarkUndoCommand(situation, markList));
    } else {
        situation->undoStack()->push(new ZoneMarkUndoCommand(situation, situation->marks()));
    }
}

void MainWindow::animate(bool state) {
    SituationScene *scene = sceneList.at(tabWidget->currentIndex());

    if (state) {
        if (scene->state() != ANIMATE) {
            scene->setState(ANIMATE);
            scene->setAnimation(timeline);
            animationSlider->setRange(0,timeline->duration());
            animationSlider->setEnabled(true);
            timeline->setFrameRange(0,timeline->duration());
            timeline->setCurrentTime(timeline->duration());
            timeline->setCurrentTime(0);
            startAction->setEnabled(true);
            loopAction->setEnabled(true);
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
        loopAction->setEnabled(false);
    }
}

void MainWindow::play() {
    if (debugLevel & 1 << ANIMATION) std::cout << "playing" << std::endl;
    pauseAction->setChecked(false);
    timeline->start();
}

void MainWindow::pause(bool pause) {
    if (pause) {
        if (debugLevel & 1 << ANIMATION) std::cout << "pausing" << std::endl;
        timeline->setPaused(true);
    } else {
        if (debugLevel & 1 << ANIMATION) std::cout << "resuming" << std::endl;
        timeline->resume();
    }
}

void MainWindow::stop() {
    if (debugLevel & 1 << ANIMATION) std::cout << "stopping" << std::endl;
    pauseAction->setChecked(false);
    timeline->stop();
    timeline->setCurrentTime(0);
}

void MainWindow::loop(bool loop) {
    if (loop) {
        if (debugLevel & 1 << ANIMATION) std::cout << "loop play" << std::endl;
        timeline->setLoopCount(0);
    } else {
        if (debugLevel & 1 << ANIMATION) std::cout << "single play" << std::endl;
        timeline->setLoopCount(1);
    }
}

void MainWindow::changeAnimationState(QTimeLine::State newState) {
    switch(newState) {
        case QTimeLine::Running:
            if (debugLevel & 1 << ANIMATION) std::cout << "state running" << std::endl;
            startAction->setEnabled(false);
            pauseAction->setEnabled(true);
            stopAction->setEnabled(true);
            animationSlider->blockSignals(true);
            break;

        case QTimeLine::Paused:
            if (debugLevel & 1 << ANIMATION) std::cout << "state paused" << std::endl;
            startAction->setEnabled(true);
            pauseAction->setEnabled(true);
            stopAction->setEnabled(true);
            animationSlider->blockSignals(false);
            break;

        case QTimeLine::NotRunning:
            if (debugLevel & 1 << ANIMATION) std::cout << "state not running" << std::endl;
            startAction->setEnabled(true);
            pauseAction->setEnabled(false);
            stopAction->setEnabled(false);
            animationSlider->blockSignals(false);
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
