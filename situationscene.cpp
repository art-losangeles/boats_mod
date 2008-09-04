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

#include <QtGui>

#include "situationscene.h"

#include "undocommands.h"

#include "model/situationmodel.h"
#include "model/trackmodel.h"
#include "model/boatmodel.h"
#include "model/markmodel.h"

#include "boat.h"
#include "track.h"
#include "mark.h"

SituationScene::SituationScene(SituationModel *situation)
        : QGraphicsScene(situation),
        m_situation(situation),
        m_trackCreated(0),
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

    // react to model mark add/remove
    connect(situation, SIGNAL(markAdded(MarkModel*)),
            this, SLOT(addMarkItem(MarkModel*)));

    // react to layline angle
    connect(situation, SIGNAL(laylineChanged(const qreal)),
            this, SLOT(setLaylines(const qreal)));

    setLaylines(situation->laylineAngle());
}

void SituationScene::addTrack(TrackModel *track) {
    std::cout << "adding track graphics for model " << track << std::endl;
    TrackGraphicsItem *trackItem = new TrackGraphicsItem(track);
    addItem(trackItem);
}

void SituationScene::deleteTrack(TrackModel *track) {
    std::cout << "Treating deleteTrack " << track << std::endl;
}

void SituationScene::addBoatItem(BoatModel *boat) {
    std::cout << "adding boat graphics for model " << boat << std::endl;
    BoatGraphicsItem *boatItem = new BoatGraphicsItem(boat);
    addItem(boatItem);
}

void SituationScene::deleteBoatItem() {
    foreach(BoatModel *boat, m_selectedModels) {
        boat->track()->deleteBoat(boat);
    }
}

void SituationScene::addMarkItem(MarkModel *mark) {
    std::cout << "adding mark graphics for model " << mark << std::endl;
    MarkGraphicsItem *markItem = new MarkGraphicsItem(mark);
    addItem(markItem);
}

void SituationScene::deleteMarkItem() {
    foreach(MarkModel *mark, m_selectedMarkModels) {
        mark->situation()->deleteMark(mark);
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

    switch (m_state) {
        case NO_STATE:
            if (event->buttons() == Qt::LeftButton) {
                mouseMoveBoatEvent(event);
            }
            if (event->buttons() == Qt::RightButton) {
                mouseHeadingEvent(event);
            }
        break;
        default:
            break;
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
                if ((event->modifiers() & Qt::ControlModifier) != 0) {
                    mouseCreateTrackEvent(event);
                } else {
                    mouseCreateBoatEvent(event);
                }
            }
        case CREATE_MARK:
            if (event->button() == Qt::LeftButton) {
                mouseCreateMarkEvent(event);
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
        m_fromPosition = event->scenePos();
    }
}

void SituationScene::mouseHeadingEvent(QGraphicsSceneMouseEvent *event) {
    if (!m_movingModels.isEmpty()) {
        QPointF point = event->scenePos() - m_modelPressed->position();
        double theta = fmod((atan2 (point.x(), -point.y()) * 180 / M_PI) + 360.0, 360.0);
        qreal snap = m_situation->laylineAngle();
        if (fabs(theta)<=5) {
            theta = 0;
        } else if (fabs(theta-snap)<=5) {
            theta = snap;
        } else if (fabs(theta-(180-snap)) <=5) {
            theta = 180-snap;
        } else if (fabs(theta-180)<=5) {
            theta = 180;
        } else if (fabs(theta-(180+snap)) <=5) {
            theta = 180+snap;
        } else if (fabs(theta-(360-snap)) <=5) {
            theta = 360-snap;
        }
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
        BoatModel* lastBoat = m_trackCreated->m_boats.last();
        // calculate new heading:
        // from position of head of last boat to new position
        qreal theta0 = lastBoat->heading() * M_PI /180;
        QPointF point2 = point - (lastBoat->position() + QPointF(60*sin(theta0),-60*cos(theta0)));
        qreal heading = atan2 (point2.x(), -point2.y()) * 180 / M_PI;
        AddBoatUndoCommand *command = new AddBoatUndoCommand(m_trackCreated, point, heading);
        m_situation->undoStack()->push(command);
    }
}

void SituationScene::mouseCreateMarkEvent(QGraphicsSceneMouseEvent *event) {
    QPointF point = event->scenePos();
    AddMarkUndoCommand *command = new AddMarkUndoCommand(m_situation, point);
    m_situation->undoStack()->push(command);
}

void SituationScene::setSelectedModels() {
    m_selectedModels.clear();
    m_selectedMarkModels.clear();
    foreach(QGraphicsItem *item, selectedItems()) {
        switch(item->type()) {
            case BOAT_TYPE:
                m_selectedModels << (qgraphicsitem_cast<BoatGraphicsItem*>(item))->boat();
                break;
            case MARK_TYPE:
                m_selectedMarkModels << (qgraphicsitem_cast<MarkGraphicsItem*>(item))->mark();
                break;
        }
    }
    std::cout << "SelectedModels update " << m_selectedModels.size() << std::endl;
}


void SituationScene::setLaylines(const qreal angle) {
    std::cout << "creating layline Background for " << angle << std::endl;
    qreal theta = angle * M_PI /180;
    qreal x = 120*sin(theta);
    qreal y = 120*cos(theta);

    QPixmap pixmap(x,y);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.setPen(Qt::DashLine);
    painter.drawLine(0,0,x,y);
    painter.drawLine(0,y,x,0);

    QBrush brush;
    brush.setTexture(pixmap);
    setBackgroundBrush(brush);
}
