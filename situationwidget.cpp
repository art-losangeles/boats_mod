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

#include <QtGui>

#include "situationwidget.h"

#include "model/situationmodel.h"

#include "undocommands.h"

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
    descriptionLayout->addWidget(abstract,1,0);

    QLabel *descriptionLabel = new QLabel(tr("Description"),descriptionGroup);
    descriptionLayout->addWidget(descriptionLabel,2,0);

    description = new QTextEdit(descriptionGroup);
    descriptionLayout->addWidget(description,3,0);

    // last bricks
    situationLayout = new QVBoxLayout(this);
    situationLayout->addWidget(scenarioGroup);
    situationLayout->addWidget(descriptionGroup);
    this->setLayout(situationLayout);
}

void SituationWidget::setSituation(SituationModel *situation) {
    if (situation != m_situation) {
        m_situation = situation;

        // Scenario Group
        title->setText(situation->title());
        connect (title, SIGNAL(textEdited(QString)),
                situation, SLOT(setTitle(QString)));

        rules->setText(situation->rules());
        connect (rules, SIGNAL(textEdited(QString)),
                situation, SLOT(setRules(QString)));

        seriesCombo->addItems(situation->seriesNames());
        seriesCombo->setCurrentIndex(situation->situationSeries());
        connect (seriesCombo, SIGNAL(currentIndexChanged(int)),
                this, SLOT(setSeries(int)));
        connect (situation, SIGNAL(seriesChanged(int)),
                seriesCombo, SLOT(setCurrentIndex(int)));

        laylineSpin->setRange(0, 359);
        laylineSpin->setWrapping(true);
        laylineSpin->setValue(situation->laylineAngle());
        connect (laylineSpin, SIGNAL(valueChanged(int)),
                this, SLOT(setLayline(int)));
        connect (situation, SIGNAL(laylineChanged(const int)),
                laylineSpin, SLOT(setValue(int)));


        // Description Group
        abstract->setText(situation->abstract());
        description->setText(situation->description());
    }
}

void SituationWidget::unSetSituation() {
    m_situation = 0;

    // Scenario Group
    title->clear();
    disconnect (title, 0, 0, 0);

    rules->clear();
    disconnect (rules, 0, 0, 0);

    seriesCombo->setCurrentIndex(0);
    disconnect (seriesCombo, 0, 0, 0);

    laylineSpin->setValue(40);
    disconnect (laylineSpin,  0, 0, 0);

    // Description Group
    abstract->clear();
    description->clear();
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

