//
// C++ Implementation: situationwidget
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

#include "situationwidget.h"
#include "trackdelegate.h"

#include "model/situationmodel.h"

#include "undocommands.h"

extern int debugLevel;

SituationWidget::SituationWidget(QWidget *parent)
    : QWidget(parent),
    m_situation(0) {

    // Scenario layout
    scenarioGroup = new QGroupBox(tr("Scenario"),this);
    scenarioGroup->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Maximum);

    scenarioGrid = new QGridLayout(scenarioGroup);
    scenarioForm = new QFormLayout();
    scenarioGrid->addLayout(scenarioForm,0,0);

    titleEdit = new QLineEdit(scenarioGroup);
    scenarioForm->addRow(new QLabel(tr("Title"),scenarioGroup),titleEdit);

    rulesEdit = new QLineEdit(scenarioGroup);
    scenarioForm->addRow(new QLabel(tr("Rules"),scenarioGroup),rulesEdit);

    seriesCombo = new QComboBox(scenarioGroup);
    scenarioForm->addRow(new QLabel(tr("Series"),scenarioGroup),seriesCombo);

    laylineCheck = new QCheckBox(scenarioGroup);
    scenarioForm->addRow(new QLabel(tr("Show Laylines"),scenarioGroup),laylineCheck);

    laylineSpin = new QSpinBox(scenarioGroup);
    scenarioForm->addRow(new QLabel(tr("Laylines"),scenarioGroup),laylineSpin);

    lengthSpin = new QSpinBox(scenarioGroup);
    scenarioForm->addRow(new QLabel(tr("Zone Length"),scenarioGroup),lengthSpin);

    QLabel *abstractLabel = new QLabel(tr("Abstract"),scenarioGroup);
    scenarioGrid->addWidget(abstractLabel,1,0);

    abstractEdit = new QPlainTextEdit(scenarioGroup);
    abstractEdit->setUndoRedoEnabled(false);
    abstractEdit->setContextMenuPolicy(Qt::NoContextMenu);
    scenarioGrid->addWidget(abstractEdit,2,0);

    QLabel *descriptionLabel = new QLabel(tr("Description"),scenarioGroup);
    scenarioGrid->addWidget(descriptionLabel,3,0);

    descriptionEdit = new QPlainTextEdit(scenarioGroup);
    descriptionEdit->setUndoRedoEnabled(false);
    descriptionEdit->setContextMenuPolicy(Qt::NoContextMenu);
    scenarioGrid->addWidget(descriptionEdit,4,0);

    // Track layout
    trackGroup = new QGroupBox(tr("Tracks"),this);
    trackLayout = new QGridLayout(trackGroup);
    trackTableModel = new TrackTableModel(m_situation);
    trackTableView = new QTableView(trackGroup);
    trackTableView->setItemDelegate(new TrackDelegate);
    trackTableView->verticalHeader()->hide();
    trackTableView->horizontalHeader()->setResizeMode(QHeaderView::Fixed);
    trackTableView->horizontalHeader()->setDefaultSectionSize(60);
    trackTableView->horizontalHeader()->setStretchLastSection(true);
    trackTableView->horizontalHeader()->setClickable(false);
    trackLayout->addWidget(trackTableView);

    // last bricks
    situationLayout = new QVBoxLayout(this);
    situationLayout->addWidget(scenarioGroup);
    situationLayout->addWidget(trackGroup);
    this->setLayout(situationLayout);
}

void SituationWidget::update() {
    if (m_situation) {
        titleEdit->setText(m_situation->title());
        rulesEdit->setText(m_situation->rules());
        seriesCombo->setCurrentIndex(m_situation->situationSeries());
        laylineCheck->setChecked(m_situation->showLayline());
        laylineSpin->setValue(m_situation->laylineAngle());
        lengthSpin->setValue(m_situation->situationLength());
        abstractEdit->setPlainText(m_situation->abstract());
        descriptionEdit->setPlainText(m_situation->description());
    }
}

void SituationWidget::setSituation(SituationModel *situation) {
    if (situation != m_situation) {
        m_situation = situation;

        update();

        // Scenario Group
        connect (titleEdit, SIGNAL(textEdited(QString)),
                this, SLOT(setTitle(QString)));
        connect (situation, SIGNAL(titleChanged(QString)),
                titleEdit, SLOT(setText(QString)));

        connect (rulesEdit, SIGNAL(textEdited(QString)),
                this, SLOT(setRules(QString)));
        connect (situation, SIGNAL(rulesChanged(QString)),
                rulesEdit, SLOT(setText(QString)));

        seriesCombo->addItems(Boats::seriesList());
        connect (seriesCombo, SIGNAL(currentIndexChanged(int)),
                this, SLOT(setSeries(int)));
        connect (situation, SIGNAL(seriesChanged(int)),
                seriesCombo, SLOT(setCurrentIndex(int)));

        connect(laylineCheck, SIGNAL(toggled(bool)),
                this, SLOT(setShowLayline(bool)));
        connect(situation, SIGNAL(showLaylineChanged(bool)),
                laylineCheck, SLOT(setChecked(bool)));

        laylineSpin->setRange(0, 359);
        laylineSpin->setWrapping(true);
        connect (laylineSpin, SIGNAL(valueChanged(int)),
                this, SLOT(setLayline(int)));
        connect (situation, SIGNAL(laylineChanged(const int)),
                laylineSpin, SLOT(setValue(int)));

        lengthSpin->setRange(1,5);
        connect (lengthSpin, SIGNAL(valueChanged(int)),
                this, SLOT(setLength(int)));
        connect (situation, SIGNAL(lengthChanged(const int)),
                lengthSpin, SLOT(setValue(int)));

        connect(abstractEdit->document(), SIGNAL(contentsChanged()),
                this, SLOT(setAbstract()));
        connect(situation, SIGNAL(abstractChanged(const QString)),
                this, SLOT(updateAbstract(const QString)));

        connect(descriptionEdit->document(), SIGNAL(contentsChanged()),
                this, SLOT(setDescription()));
        connect(situation, SIGNAL(descriptionChanged(const QString)),
                this, SLOT(updateDescription(const QString)));

        // Track group
        trackTableModel->setSituation(m_situation);
        connect(situation, SIGNAL(trackAdded(TrackModel*)),
                trackTableModel, SLOT(addTrack(TrackModel*)));
        connect(situation, SIGNAL(trackRemoved(TrackModel*)),
                trackTableModel, SLOT(deleteTrack(TrackModel*)));
        trackTableView->setModel(trackTableModel);

    }
}

void SituationWidget::unSetSituation() {

    // Scenario Group
    disconnect(titleEdit, 0, 0, 0);
    titleEdit->clear();

    disconnect(rulesEdit, 0, 0, 0);
    rulesEdit->clear();

    disconnect(seriesCombo, 0, 0, 0);
    disconnect(m_situation, 0, seriesCombo, 0);
    seriesCombo->clear();
    seriesCombo->setCurrentIndex(0);

    disconnect(laylineCheck, 0, 0, 0);
    disconnect(m_situation, 0, laylineCheck, 0);
    laylineCheck->setChecked(true);

    disconnect(laylineSpin,  0, 0, 0);
    disconnect(m_situation, 0, laylineSpin, 0);
    laylineSpin->setValue(40);

    disconnect(lengthSpin, 0, 0, 0);
    disconnect(m_situation, 0, lengthSpin, 0);
    lengthSpin->setValue(3);

    disconnect(m_situation, 0, this, 0);
    disconnect(abstractEdit->document(), 0, 0, 0);
    abstractEdit->clear();
    disconnect(descriptionEdit->document(), 0, 0, 0);
    descriptionEdit->clear();

    // Track Group
    disconnect(m_situation, 0, trackTableModel, 0);

    m_situation = 0;
}

void SituationWidget::setTitle(QString title) {
    if (m_situation) {
        if (title != m_situation->title()) {
            m_situation->undoStack()->push(new SetTitleUndoCommand(m_situation, title));
        }
    }
}

void SituationWidget::setRules(QString rules) {
    if (m_situation) {
        if (rules != m_situation->rules()) {
            m_situation->undoStack()->push(new SetRulesUndoCommand(m_situation, rules));
        }
    }
}

void SituationWidget::setShowLayline(bool show) {
    if (m_situation) {
        if (show != m_situation->showLayline())
            m_situation->undoStack()->push(new SetShowLaylineUndoCommand(m_situation));
    }
}

void SituationWidget::setLayline(int angle) {
    if (m_situation) {
        if (angle != m_situation->laylineAngle()) {
            m_situation->undoStack()->push(new SetLaylineUndoCommand(m_situation, angle));
        }
    }
}

void SituationWidget::setLength(int length) {
    if (m_situation) {
        if (length != m_situation->situationLength()) {
            m_situation->undoStack()->push(new LengthMarkUndoCommand(m_situation, length));
        }
    }
}


void SituationWidget::setSeries(int series) {
    if (m_situation) {
        if (series != m_situation->situationSeries()) {
            m_situation->undoStack()->push(new SetSituationSeriesUndoCommand(m_situation, series));
        }
    }
}

void SituationWidget::setAbstract() {
    if (m_situation) {
        if (abstractEdit->document()->toPlainText() != m_situation->abstract()) {
            m_situation->undoStack()->push(new SetAbstractUndoCommand(m_situation, abstractEdit->document()->toPlainText()));
        }
    }
}

void SituationWidget::setDescription() {
    if (m_situation) {
        if (descriptionEdit->document()->toPlainText() != m_situation->description()) {
            m_situation->undoStack()->push(new SetDescriptionUndoCommand(m_situation, descriptionEdit->document()->toPlainText()));
        }
    }
}

void SituationWidget::updateAbstract(const QString abstract) {
    if (m_situation) {
        if (abstractEdit->document()->toPlainText() != abstract) {
            disconnect(abstractEdit->document(), 0, 0, 0);
            abstractEdit->document()->setPlainText(abstract);
            connect(abstractEdit->document(), SIGNAL(contentsChanged()),
                    this, SLOT(setAbstract()));
        }
    }
}

void SituationWidget::updateDescription(const QString description) {
    if (m_situation) {
        if (descriptionEdit->document()->toPlainText() != description) {
            disconnect(descriptionEdit->document(), 0, 0, 0);
            descriptionEdit->document()->setPlainText(description);
            connect(descriptionEdit->document(), SIGNAL(contentsChanged()),
                    this, SLOT(setDescription()));
        }
    }
}

