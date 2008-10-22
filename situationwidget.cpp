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

    title = new QLineEdit(scenarioGroup);
    scenarioForm->addRow(new QLabel(tr("Title"),scenarioGroup),title);

    rules = new QLineEdit(scenarioGroup);
    scenarioForm->addRow(new QLabel(tr("Rules"),scenarioGroup),rules);

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

    abstract = new QTextEdit(descriptionGroup);
    abstract->setAcceptRichText(false);
    descriptionLayout->addWidget(abstract,1,0);

    QLabel *descriptionLabel = new QLabel(tr("Description"),descriptionGroup);
    descriptionLayout->addWidget(descriptionLabel,2,0);

    description = new QTextEdit(descriptionGroup);
    description->setAcceptRichText(false);
    descriptionLayout->addWidget(description,3,0);

    // last bricks
    situationLayout = new QVBoxLayout(this);
    situationLayout->addWidget(scenarioGroup);
    situationLayout->addWidget(descriptionGroup);
    this->setLayout(situationLayout);
}

void SituationWidget::update() {
    if (m_situation) {
        title->setText(m_situation->title());
        rules->setText(m_situation->rules());
        seriesCombo->setCurrentIndex(m_situation->situationSeries());
        laylineSpin->setValue(m_situation->laylineAngle());
        abstract->setPlainText(m_situation->abstract());
        description->setPlainText(m_situation->description());
    }
}

void SituationWidget::setSituation(SituationModel *situation) {
    if (situation != m_situation) {
        m_situation = situation;

        update();

        // Scenario Group
        connect (title, SIGNAL(textEdited(QString)),
                situation, SLOT(setTitle(QString)));

        connect (rules, SIGNAL(textEdited(QString)),
                situation, SLOT(setRules(QString)));

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
        connect(abstract->document(), SIGNAL(contentsChanged()),
                this, SLOT(setAbstract()));

        connect(description->document(), SIGNAL(contentsChanged()),
                this, SLOT(setDescription()));
    }
}

void SituationWidget::unSetSituation() {

    // Scenario Group
    disconnect(title, 0, 0, 0);
    title->clear();

    disconnect(rules, 0, 0, 0);
    rules->clear();

    disconnect(seriesCombo, 0, 0, 0);
    disconnect(m_situation, 0, seriesCombo, 0);
    seriesCombo->clear();
    seriesCombo->setCurrentIndex(0);

    disconnect(laylineSpin,  0, 0, 0);
    disconnect(m_situation, 0, laylineSpin, 0);
    laylineSpin->setValue(40);

    // Description Group
    disconnect(abstract, 0, 0, 0);
    abstract->clear();
    disconnect(description, 0, 0, 0);
    description->clear();

    m_situation = 0;
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
        if (debugLevel & 1 << COMMAND) std::cout << "setting Abstract" << std::endl;
        m_situation->setAbstract(abstract->document()->toPlainText());
    }
}

void SituationWidget::setDescription() {
    if (m_situation) {
        if (debugLevel & 1 << COMMAND) std::cout << "setting Description" << std::endl;
        m_situation->setDescription(description->document()->toPlainText());
    }
}
