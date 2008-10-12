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

#include "commontypes.h"
#include "undocommands.h"

#include "model/situationmodel.h"
#include "model/trackmodel.h"
#include "model/boatmodel.h"
#include "model/markmodel.h"

#include "boat.h"
#include "track.h"
#include "mark.h"
#include "boatanimation.h"

extern int debugLevel;

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
    connect(situation, SIGNAL(laylineChanged(const int)),
            this, SLOT(setLaylines(const int)));

    setLaylines(situation->laylineAngle());
}

void SituationScene::addTrack(TrackModel *track) {
    if (debugLevel & 1 << VIEW) std::cout << "adding track graphics for model " << track << std::endl;
    TrackGraphicsItem *trackItem = new TrackGraphicsItem(track);
    addItem(trackItem);
}

void SituationScene::deleteTrack(TrackModel *track) {
    if (debugLevel & 1 << VIEW) std::cout << "Treating deleteTrack " << track << std::endl;
}

void SituationScene::addBoatItem(BoatModel *boat) {
    if (debugLevel & 1 << VIEW) std::cout << "adding boat graphics for model " << boat << std::endl;
    BoatGraphicsItem *boatItem = new BoatGraphicsItem(boat);
    addItem(boatItem);
}

void SituationScene::deleteBoatItem() {
    foreach(BoatModel *boat, m_selectedBoatModels) {
        boat->track()->deleteBoat(boat);
    }
}

void SituationScene::addMarkItem(MarkModel *mark) {
    if (debugLevel & 1 << VIEW) std::cout << "adding mark graphics for model " << mark << std::endl;
    MarkGraphicsItem *markItem = new MarkGraphicsItem(mark);
    addItem(markItem);
}

void SituationScene::deleteMarkItem() {
    foreach(MarkModel *mark, m_selectedMarkModels) {
        mark->situation()->deleteMark(mark);
    }
}

void SituationScene::setAnimation(QTimeLine *timer) {
    if (debugLevel & 1 << VIEW) std::cout << "preparing for Animation" << std::endl;
    int maxSize = 0;
    foreach (TrackModel *track, m_situation->tracks()) {
        if (track->boats().size() > maxSize)
            maxSize = track->boats().size() - 1;
    }
    timer->setDuration(2000 * maxSize);

    foreach (TrackModel *track, m_situation->tracks()) {
        BoatGraphicsItem *boatItem = new BoatGraphicsItem(track->boats()[0]);
        addItem(boatItem);

        BoatAnimation *animation = new BoatAnimation(track, boatItem, maxSize);
        animation->setTimeLine(timer);
        m_animationItems.push_back(animation);
    }
}

void SituationScene::unSetAnimation() {
    if (debugLevel & 1 << VIEW) std::cout << "ending Animation" << std::endl;
    foreach(BoatAnimation *animation, m_animationItems) {
        removeItem(animation->boat());
        m_animationItems.removeOne(animation);
        delete animation->boat();
        delete animation;
    }
}

void SituationScene::keyPressEvent(QKeyEvent *event) {
    // propagate key event first for focus items
    QGraphicsScene::keyPressEvent(event);

    if (m_selectedModels.isEmpty()) {
        return;
    }
    if (event->key() == Qt::Key_Plus) {
        qreal theta = fmod(m_selectedBoatModels[0]->heading() + 5 + 360.0, 360.0);
        m_situation->undoStack()->push(new HeadingBoatUndoCommand(m_selectedBoatModels, theta));

    } else if (event->key() == Qt::Key_Minus) {
        qreal theta = fmod(m_selectedBoatModels[0]->heading() - 5 + 360.0, 360.0);
        m_situation->undoStack()->push(new HeadingBoatUndoCommand(m_selectedBoatModels, theta));

    } else if (event->key() == Qt::Key_Left) {
        QPointF pos(-5,0);
        m_situation->undoStack()->push(new MoveModelUndoCommand(m_selectedModels, pos));

    } else if (event->key() == Qt::Key_Right) {
        QPointF pos(5,0);
        m_situation->undoStack()->push(new MoveModelUndoCommand(m_selectedModels, pos));

    } else if (event->key() == Qt::Key_Up) {
        QPointF pos(0,-5);
        m_situation->undoStack()->push(new MoveModelUndoCommand(m_selectedModels, pos));
    } else if (event->key() == Qt::Key_Down) {
        QPointF pos(0,5);
        m_situation->undoStack()->push(new MoveModelUndoCommand(m_selectedModels, pos));
    }
}

void SituationScene::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    // propagate mouse event first for selected items
    QGraphicsScene::mousePressEvent(event);
    if (!m_selectedBoatModels.isEmpty())
        m_trackCreated = m_selectedBoatModels[0]->track();

    m_fromPosition = event->scenePos();
    if (debugLevel & 1 << VIEW) std::cout << "Mouse pressed with " << m_selectedModels.size()
    << " items selected" << std::endl;
}

void SituationScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    QGraphicsScene::mouseMoveEvent(event);

    switch (m_state) {
        case NO_STATE:
            if (event->buttons() == Qt::LeftButton) {
                mouseMoveModelEvent(event);
            }
            if (event->buttons() == Qt::RightButton) {
                mouseHeadingEvent(event);
            }
        break;
        case CREATE_BOAT:
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
                mouseMoveModelEvent(event);
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
            if (event->button() == Qt::RightButton) {
                mouseHeadingEvent(event);
            }
            break;
        case CREATE_MARK:
            if (event->button() == Qt::LeftButton) {
                mouseCreateMarkEvent(event);
            }
            break;
        default:
            break;
    }
}
// itemselected should connect trackwidget with item
//    headingbox->connect(headingbox,SIGNAL(valueChanged(int)),
//        boat,SLOT(setHeading(int)));

void SituationScene::mouseMoveModelEvent(QGraphicsSceneMouseEvent *event) {
    if (!m_selectedModels.isEmpty() && event->scenePos() != m_fromPosition) {
        m_situation->undoStack()->push(new MoveModelUndoCommand(m_selectedModels,(event->scenePos()-m_fromPosition)));
        m_fromPosition = event->scenePos();
    }
}

void SituationScene::mouseHeadingEvent(QGraphicsSceneMouseEvent *event) {
    if (!m_selectedBoatModels.isEmpty() && event->scenePos() != m_modelPressed->position()) {
        QPointF point = event->scenePos() - m_modelPressed->position();
        qreal theta = fmod((atan2 (point.x(), -point.y()) * 180 / M_PI) + 360.0, 360.0);
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
        m_situation->undoStack()->push(new HeadingBoatUndoCommand(m_selectedBoatModels, theta));
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
    setState(CREATE_BOAT);
}

void SituationScene::mouseCreateBoatEvent(QGraphicsSceneMouseEvent *event) {
    QPointF point = event->scenePos();
    if (m_trackCreated) {
        const BoatModel* lastBoat = m_trackCreated->boats().last();
        // calculate new heading:
        // from position of head of last boat to new position
        qreal theta0 = lastBoat->heading() * M_PI /180;
        QPointF point2 = point - (lastBoat->position() + QPointF(60*sin(theta0),-60*cos(theta0)));
        qreal heading = fmod(atan2 (point2.x(), -point2.y()) * 180 / M_PI + 360.0, 360.0);
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
    m_selectedBoatModels.clear();
    m_selectedMarkModels.clear();
    foreach(QGraphicsItem *item, selectedItems()) {
        switch(item->type()) {
            case BOAT_TYPE: {
                BoatModel *boat = (qgraphicsitem_cast<BoatGraphicsItem*>(item))->boat();
                m_selectedModels << boat;
                m_selectedBoatModels << boat;
                }
                break;
            case MARK_TYPE: {
                MarkModel *mark = (qgraphicsitem_cast<MarkGraphicsItem*>(item))->mark();
                m_selectedModels << mark;
                m_selectedMarkModels << mark;
                }
                break;
        }
    }
    if (debugLevel & 1 << VIEW) std::cout << "SelectedModels update " << m_selectedModels.size() << std::endl;
}


void SituationScene::setLaylines(const int angle) {
    if (debugLevel & 1 << VIEW) std::cout << "creating layline Background for " << angle << std::endl;
    qreal theta = angle * M_PI /180;
    int x = lround(120*sin(theta));
    int y = lround(120*cos(theta));

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
