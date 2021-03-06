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

#include "model/situationmodel.h"

#include "undocommands.h"

extern int debugLevel;

SituationWidget::SituationWidget(QWidget *parent)
    : QWidget(parent),
    m_situation(0) {

    // Scenario layout
    scenarioGroup = new QGroupBox(tr("Scenario"),this);
    scenarioForm = new QFormLayout(scenarioGroup);

    titleEdit = new QLineEdit(scenarioGroup);
    scenarioForm->addRow(new QLabel(tr("Title"),scenarioGroup),titleEdit);

    rulesEdit = new QLineEdit(scenarioGroup);
    scenarioForm->addRow(new QLabel(tr("Rules"),scenarioGroup),rulesEdit);

    seriesCombo = new QComboBox(scenarioGroup);
    scenarioForm->addRow(new QLabel(tr("Series"),scenarioGroup),seriesCombo);

    laylineSpin = new QSpinBox(scenarioGroup);
    scenarioForm->addRow(new QLabel(tr("Laylines"),scenarioGroup),laylineSpin);
    scenarioGroup->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Maximum);

    // Description layout
    descriptionGroup = new QGroupBox(tr("Description"),this);
    descriptionLayout = new QGridLayout(descriptionGroup);

    QLabel *abstractLabel = new QLabel(tr("Abstract"),descriptionGroup);
    descriptionLayout->addWidget(abstractLabel,0,0);

    abstractEdit = new QPlainTextEdit(descriptionGroup);
    abstractEdit->setUndoRedoEnabled(false);
    abstractEdit->setContextMenuPolicy(Qt::NoContextMenu);
    descriptionLayout->addWidget(abstractEdit,1,0);

    QLabel *descriptionLabel = new QLabel(tr("Description"),descriptionGroup);
    descriptionLayout->addWidget(descriptionLabel,2,0);

    descriptionEdit = new QPlainTextEdit(descriptionGroup);
    descriptionEdit->setUndoRedoEnabled(false);
    descriptionEdit->setContextMenuPolicy(Qt::NoContextMenu);
    descriptionLayout->addWidget(descriptionEdit,3,0);

    // last bricks
    situationLayout = new QVBoxLayout(this);
    situationLayout->addWidget(scenarioGroup);
    situationLayout->addWidget(descriptionGroup);
    this->setLayout(situationLayout);
}

void SituationWidget::update() {
    if (m_situation) {
        titleEdit->setText(m_situation->title());
        rulesEdit->setText(m_situation->rules());
        seriesCombo->setCurrentIndex(m_situation->situationSeries());
        laylineSpin->setValue(m_situation->laylineAngle());
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

        seriesCombo->addItems(situation->seriesNames());
        connect (seriesCombo, SIGNAL(currentIndexChanged(int)),
                this, SLOT(setSeries(int)));
        connect (situation, SIGNAL(seriesChanged(int)),
                seriesCombo, SLOT(setCurrentIndex(int)));

        laylineSpin->setRange(0, 359);
        laylineSpin->setWrapping(true);
        connect (laylineSpin, SIGNAL(valueChanged(int)),
                this, SLOT(setLayline(int)));
        connect (situation, SIGNAL(laylineChanged(const int)),
                laylineSpin, SLOT(setValue(int)));


        // Description Group
        connect(abstractEdit->document(), SIGNAL(contentsChanged()),
                this, SLOT(setAbstract()));
        connect(situation, SIGNAL(abstractChanged(const QString)),
                this, SLOT(updateAbstract(const QString)));

        connect(descriptionEdit->document(), SIGNAL(contentsChanged()),
                this, SLOT(setDescription()));
        connect(situation, SIGNAL(descriptionChanged(const QString)),
                this, SLOT(updateDescription(const QString)));
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

    disconnect(laylineSpin,  0, 0, 0);
    disconnect(m_situation, 0, laylineSpin, 0);
    laylineSpin->setValue(40);

    // Description Group
    disconnect(m_situation, 0, this, 0);
    disconnect(abstractEdit->document(), 0, 0, 0);
    abstractEdit->clear();
    disconnect(descriptionEdit->document(), 0, 0, 0);
    descriptionEdit->clear();

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

void SituationWidget::setLayline(int angle) {
    if (m_situation) {
        if (angle != m_situation->laylineAngle()) {
            m_situation->undoStack()->push(new SetLaylineUndoCommand(m_situation, angle));
        }
    }
}

void SituationWidget::setSeries(int series) {
    if (m_situation) {
        if (series != m_situation->situationSeries()) {
            m_situation->undoStack()->push(new SetSeriesUndoCommand(m_situation, series));
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

