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

#include <QGraphicsSceneMouseEvent>

#include "situationscene.h"

#include "model/situationmodel.h"
#include "model/trackmodel.h"
#include "model/boatmodel.h"

#include "boat.h"

SituationScene::SituationScene(SituationModel *situation)
        : QGraphicsScene(situation),
        m_situation(situation),
        m_state(NO_STATE) {
    // react to self change of selection
    connect(this, SIGNAL(selectionChanged()),
            this, SLOT(setSelectedModels()));

    // react to model track add
    connect(situation, SIGNAL(trackAdded(TrackModel*)),
            this, SLOT(addTrack(TrackModel*)));
}

void SituationScene::addTrack(TrackModel *track) {
    std::cout << "Treating addTrack" << std::endl;
    // react to model boat add
    connect(track, SIGNAL(boatAdded(BoatModel*)),
            this, SLOT(addBoatItem(BoatModel*)));
}

void SituationScene::deleteTrack(TrackModel *track) {
    std::cout << "Treating deleteTrack" << std::endl;
    disconnect(track, SIGNAL(boatAdded(BoatModel*)),
               this, SLOT(addBoatItem(BoatModel*)));
}

void SituationScene::addBoatItem(BoatModel *boat) {
    std::cout << "Treating addBoatItem" << std::endl;
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

    if (event->button() == Qt::LeftButton) {
        if (!selectedItems().isEmpty() && m_movingModels.isEmpty()) {
            m_movingModels = m_selectedModels;
            m_fromPosition = event->scenePos();
            std::cout << "Mouse pressed with " << m_movingModels.size()
            << " items selected" << std::endl;
        }
    }
}

void SituationScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    QGraphicsScene::mouseReleaseEvent(event);
    if (event->button() == Qt::LeftButton) {
        std::cout << "Mouse released with " << m_movingModels.size() << " items selected" << std::endl;
        if (!m_movingModels.isEmpty() && event->scenePos() != m_fromPosition) {
            std::cout << "Detected move for " << m_movingModels.size() << "events" << std::endl;
        }
        m_movingModels.clear();
    }
}
// itemselected should connect trackwidget with item
//    headingbox->connect(headingbox,SIGNAL(valueChanged(int)),
//        boat,SLOT(setHeading(int)));


void SituationScene::setSelectedModels() {
    m_selectedModels.clear();
    if (!selectedItems().isEmpty()) {
        foreach(QGraphicsItem *item, selectedItems()) {
            m_selectedModels << (static_cast<BoatGraphicsItem*>(item))->boat();
        }
    }
    std::cout << "SelectedModels update " << m_selectedModels.size() << std::endl;
}
