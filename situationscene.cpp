//
// C++ Implementation: situationscene
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
#include <math.h>

#include <QGraphicsSceneMouseEvent>

#include "situationscene.h"

#include "undocommands.h"

#include "model/situationmodel.h"
#include "model/trackmodel.h"
#include "model/boatmodel.h"

#include "boat.h"
#include "track.h"

SituationScene::SituationScene(SituationModel *situation)
        : QGraphicsScene(situation),
        m_situation(situation),
        m_state(NO_STATE) {
    // react to self change of selection
    connect(this, SIGNAL(selectionChanged()),
            this, SLOT(setSelectedModels()));

    // react to model track add/remove
    connect(situation, SIGNAL(trackAdded(TrackModel*)),
            this, SLOT(addTrack(TrackModel*)));
    connect(situation, SIGNAL(trackRemoved(TrackModel*)),
            this, SLOT(deleteTrack(TrackModel*)));

    // react to model boat add/remove
    connect(situation, SIGNAL(boatAdded(BoatModel*)),
            this, SLOT(addBoatItem(BoatModel*)));
}

void SituationScene::addTrack(TrackModel *track) {
    std::cout << "adding track graphics for model " << track << std::endl;
    TrackGraphicsItem *trackItem = new TrackGraphicsItem(track);
    addItem(trackItem);
}

void SituationScene::deleteTrack(TrackModel *track) {
    std::cout << "Treating deleteTrack " << std::endl;
}

void SituationScene::addBoatItem(BoatModel *boat) {
    std::cout << "adding boat graphics for model " << boat << std::endl;
    BoatGraphicsItem *boatItem = new BoatGraphicsItem(boat);
    addItem(boatItem);
}

void SituationScene::deleteBoatItem() {
    if (selectedItems().isEmpty())
        return;
    foreach(BoatModel *boat, selectedModels()) {
        boat->track()->deleteBoat(boat);
    }
}

void SituationScene::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    // propagate mouse event first for selected items
    QGraphicsScene::mousePressEvent(event);

    m_fromPosition = event->scenePos();
    if (!selectedItems().isEmpty() && m_movingModels.isEmpty()) {
        m_movingModels = m_selectedModels;
    }
    std::cout << "Mouse pressed with " << m_movingModels.size()
    << " items selected" << std::endl;
}

void SituationScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    QGraphicsScene::mouseMoveEvent(event);
    if (event->buttons() == Qt::RightButton) {
        mouseHeadingEvent(event);
    }
}

void SituationScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    QGraphicsScene::mouseReleaseEvent(event);
    if (event->button() == Qt::LeftButton) {
        std::cout << "Left Mouse released with " << m_movingModels.size() << " items selected" << std::endl;
        if (!m_movingModels.isEmpty() && event->scenePos() != m_fromPosition) {
            std::cout << "Detected move for " << m_movingModels.size() << " items" << std::endl;
            m_situation->undoStack()->push(new MoveBoatUndoCommand(m_movingModels,(event->scenePos()-m_fromPosition)));
        }
    }
    if (event->button() == Qt::RightButton) {
        std::cout << "Right-Mouse released " << std::endl;
        mouseHeadingEvent(event);
    }
    m_movingModels.clear();
}
// itemselected should connect trackwidget with item
//    headingbox->connect(headingbox,SIGNAL(valueChanged(int)),
//        boat,SLOT(setHeading(int)));

void SituationScene::mouseHeadingEvent(QGraphicsSceneMouseEvent *event) {
    if (!m_movingModels.isEmpty()) {
        QPointF point = event->scenePos() - m_modelPressed->position();
        double theta = atan2 (point.x(), -point.y()) * 180 / M_PI;
        foreach(BoatModel* boat, m_movingModels) {
            boat->setHeading(theta, true);
        }
    }
}

void SituationScene::setSelectedModels() {
    m_selectedModels.clear();
    if (!selectedItems().isEmpty()) {
        foreach(QGraphicsItem *item, selectedItems()) {
            m_selectedModels << (static_cast<BoatGraphicsItem*>(item))->boat();
        }
    }
    std::cout << "SelectedModels update " << m_selectedModels.size() << std::endl;
}
