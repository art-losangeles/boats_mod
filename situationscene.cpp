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
    switch (m_state) {
        case NO_STATE:
            if (event->button() == Qt::LeftButton) {
                mouseMoveBoatEvent(event);
            }
            if (event->button() == Qt::RightButton) {
                mouseHeadingEvent(event);
            }
            break;
        case CREATE_TRACK:
            if (event->button() == Qt::LeftButton) {
                mouseCreateTrackEvent(event);
            }
            break;
        case CREATE_BOAT:
            if (event->button() == Qt::LeftButton) {
                mouseCreateBoatEvent(event);
            }
            break;
        default:
            break;
    }
    m_movingModels.clear();
}
// itemselected should connect trackwidget with item
//    headingbox->connect(headingbox,SIGNAL(valueChanged(int)),
//        boat,SLOT(setHeading(int)));

void SituationScene::mouseMoveBoatEvent(QGraphicsSceneMouseEvent *event) {
    if (!m_movingModels.isEmpty() && event->scenePos() != m_fromPosition) {
        m_situation->undoStack()->push(new MoveBoatUndoCommand(m_movingModels,(event->scenePos()-m_fromPosition)));
    }
}

void SituationScene::mouseHeadingEvent(QGraphicsSceneMouseEvent *event) {
    if (!m_movingModels.isEmpty()) {
        QPointF point = event->scenePos() - m_modelPressed->position();
        double theta = atan2 (point.x(), -point.y()) * 180 / M_PI;
        foreach(BoatModel* boat, m_movingModels) {
            boat->setHeading(theta, true);
        }
    }
}

void SituationScene::mouseCreateTrackEvent(QGraphicsSceneMouseEvent *event) {
    QPointF point = event->scenePos();
    AddTrackUndoCommand *command = new AddTrackUndoCommand(m_situation);
    TrackModel *track = command->track();
    m_situation->undoStack()->push(command);
    BoatModel *boat = new BoatModel(track, track);
    boat->setPosition(point);
    track->addBoat(boat);
    m_trackCreated = track;
    m_state = CREATE_BOAT;
}

void SituationScene::mouseCreateBoatEvent(QGraphicsSceneMouseEvent *event) {
    QPointF point = event->scenePos();
    if (m_trackCreated) {
        m_situation->undoStack()->push(new AddBoatUndoCommand(m_trackCreated, point));
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
