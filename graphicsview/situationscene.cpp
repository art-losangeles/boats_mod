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
#include <cmath>

#include <QtGui>

#include "situationscene.h"

#include "commontypes.h"
#include "undocommands.h"

#include "situationmodel.h"
#include "trackmodel.h"
#include "boatmodel.h"
#include "markmodel.h"

#include "boat.h"
#include "track.h"
#include "mark.h"
#include "boatanimation.h"

extern int debugLevel;

SituationScene::SituationScene(SituationModel *situation)
        : QGraphicsScene(situation),
        m_situation(situation),
        m_trackCreated(0),
        m_state(NO_STATE),
        m_time(QTime::currentTime()),
        m_clickTime(QTime::currentTime()),
        m_actionMenu(0) {

    // try to set a minimum scene rect
    QGraphicsItem *e1 = addEllipse(QRectF(-1000,-1000, 1, 1));
    QGraphicsItem *e2 = addEllipse(QRectF(1000, 1000, 1, 1));
    sceneRect();
    delete e1;
    delete e2;

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
    foreach (BoatModel *boat, m_selectedBoatModels) {
        boat->track()->deleteBoat(boat);
    }
}

void SituationScene::addMarkItem(MarkModel *mark) {
    if (debugLevel & 1 << VIEW) std::cout << "adding mark graphics for model " << mark << std::endl;
    MarkGraphicsItem *markItem = new MarkGraphicsItem(mark);
    addItem(markItem);
}

void SituationScene::deleteMarkItem() {
    foreach (MarkModel *mark, m_selectedMarkModels) {
        mark->situation()->deleteMark(mark);
    }
}

/**
    Prepares the Scene for animation mode.
    This method finds the maximum size of track, and sets the timer length
    accordingly.
    It then creates a BoatGraphicsItem for animation purpose, and associates
    an BoatAnimation to move it along the \a timer values
*/

void SituationScene::setAnimation(QTimeLine *timer) {
    if (debugLevel & 1 << VIEW) std::cout << "preparing for Animation" << std::endl;
    int maxSize = 0;
    foreach (const TrackModel *track, m_situation->tracks()) {
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

/**
    Restores the Scene out of animation mode.
    This method brings the scene back to the normal drawing mode.
    For this it removes all BoatAnimation objects created in setAnimation().
*/

void SituationScene::unSetAnimation() {
    if (debugLevel & 1 << VIEW) std::cout << "ending Animation" << std::endl;
    foreach (BoatAnimation *animation, m_animationItems) {
        removeItem(animation->boat());
        m_animationItems.removeOne(animation);
        delete animation->boat();
        delete animation;
    }
}

/**
    Reacts to user keyboard actions in the Scene
    This method modifies the associated SituationModel through the
    concerned Undo Framework class. Handled keys are for
    - heading of selected boats (+,-)
    - position of selected objects (H (left), L (right), J (down), K (up))
*/

void SituationScene::keyPressEvent(QKeyEvent *event) {

    if (!m_selectedBoatModels.isEmpty()) {
        if (event->key() == Qt::Key_Plus) {
            qreal theta = fmod(m_selectedBoatModels[0]->heading() + 5 + 360.0, 360.0);
            m_situation->undoStack()->push(new HeadingBoatUndoCommand(m_selectedBoatModels, theta));

        } else if (event->key() == Qt::Key_Minus) {
            qreal theta = fmod(m_selectedBoatModels[0]->heading() - 5 + 360.0, 360.0);
            m_situation->undoStack()->push(new HeadingBoatUndoCommand(m_selectedBoatModels, theta));

        } else if (event->key() == Qt::Key_Less) {
            m_situation->undoStack()->push(new TrimBoatUndoCommand(m_selectedBoatModels, m_selectedBoatModels[0]->trim() - 5));

        } else if (event->key() == Qt::Key_Greater) {
            m_situation->undoStack()->push(new TrimBoatUndoCommand(m_selectedBoatModels, m_selectedBoatModels[0]->trim() + 5));

        } else if (event->key() == Qt::Key_Equal) {
            m_situation->undoStack()->push(new TrimBoatUndoCommand(m_selectedBoatModels, 0));
        }
    }

    if (!m_selectedModels.isEmpty()) {
        if (event->key() == Qt::Key_Left) {
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
        } else {
            QGraphicsScene::keyPressEvent(event);
        }
    } else {
        QGraphicsScene::keyPressEvent(event);
    }
}

/**
    Sets selection depending on modal status.
    Selection happens with left button in NO_STATE, and right button in CREATE_BOAT
    and CREATE_MARK modes.
*/

void SituationScene::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    switch (m_state) {
        case NO_STATE:
            mouseSelectEvent(event);
            if (!m_selectedBoatModels.isEmpty()) {
                m_trackCreated = m_selectedBoatModels[0]->track();
            }
            break;
        case CREATE_BOAT:
        case CREATE_MARK:
            if (event->buttons() == Qt::RightButton
                || (event->buttons() == Qt::LeftButton
                    && ((event->modifiers() & Qt::MetaModifier) != 0))) {
                mouseSelectEvent(event);
            }
            break;
        default:
            break;
    }

    m_clickTime.start();
}

/**
    Handles a timer to limit user interaction
    This method limits the move processing to one event handled per 40ms.
    This is very noticeable on Windows platform, and the
    culprit is the drawing, not the model setting.
*/

void SituationScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    // limit update rate to 40ms
    if ((m_time.elapsed() <40) ) {
        return;
    }

    QGraphicsScene::mouseMoveEvent(event);

    switch (m_state) {
        case NO_STATE:
            if (event->buttons() == Qt::LeftButton
                && (event->modifiers() & Qt::MetaModifier) == 0) {
                mouseMoveModelEvent(event);
            }
            if (event->buttons() == Qt::RightButton
                || (event->buttons() == Qt::LeftButton
                    && ((event->modifiers() & Qt::MetaModifier) != 0))) {
                mouseHeadingEvent(event);
            }
        break;
        case CREATE_BOAT:
            if (event->buttons() == Qt::RightButton
                || (event->buttons() == Qt::LeftButton
                    && ((event->modifiers() & Qt::MetaModifier) != 0))) {
                mouseHeadingEvent(event);
            }
        break;
        default:
            break;
    }

    // trigger next update rate calculation
    m_time.start();
}

/**
    Performs action depending of button and mode
    This method will trigger performing of actions like:
    - setting position of selected objects
    - setting heading of selected BoatGraphicsItem
    - creating new TrackModel, BoatModel or MarkModel according to mode.
*/

void SituationScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    QGraphicsScene::mouseReleaseEvent(event);

    bool click = (m_clickTime.elapsed() < 250);
    if (click && (event->button() == Qt::RightButton
                || (event->button() == Qt::LeftButton
                    && ((event->modifiers() & Qt::MetaModifier) != 0)))) {
        m_actionMenu->popup(event->screenPos());
        return;
    }

    switch (m_state) {
        case NO_STATE:
            if (event->button() == Qt::LeftButton
                && (event->modifiers() & Qt::MetaModifier) == 0) {
                mouseMoveModelEvent(event);
            }
            if (event->button() == Qt::RightButton
                || (event->button() == Qt::LeftButton
                    && ((event->modifiers() & Qt::MetaModifier) != 0))) {
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
                } else if ((event->modifiers() & Qt::MetaModifier) == 0) {
                    mouseCreateBoatEvent(event);
                }
            }
            if (event->button() == Qt::RightButton
                || (event->button() == Qt::LeftButton
                    && ((event->modifiers() & Qt::MetaModifier) != 0))) {
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

void SituationScene::mouseSelectEvent(QGraphicsSceneMouseEvent *event) {
    // propagate mouse event first for selected items
    QGraphicsScene::mousePressEvent(event);

    m_fromPosition = event->scenePos();
    if (debugLevel & 1 << VIEW) std::cout << "Mouse pressed with " << m_selectedModels.size()
    << " items selected" << std::endl;
}

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
        qreal length = m_trackCreated->length() / 2.0;
        qreal theta0 = lastBoat->heading() * M_PI /180;
        QPointF point2 = point - (lastBoat->position() + QPointF(length*sin(theta0),-length*cos(theta0)));
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
    foreach (QGraphicsItem *item, selectedItems()) {
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
    emit selectedModelsChanged();
}


void SituationScene::setLaylines(const int angle) {
    if (!m_situation->showLayline()) {
        if (debugLevel & 1 << VIEW) std::cout << "reseting empty Background" << std::endl;
        setBackgroundBrush(Qt::NoBrush);
        return;
    }
    if (debugLevel & 1 << VIEW) std::cout << "creating layline Background for " << angle << std::endl;
    qreal theta = angle * M_PI /180;
    int length = m_situation->sizeForSeries(m_situation->situationSeries());

    // draw 4 times as big, then use transform to scale back the brush
    // gives better precision grid
    qreal x = 2*length*sin(theta) * 4;
    qreal y = 2*length*cos(theta) * 4;

    QPixmap pixmap(x,y);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    QPen pen;
    pen.setWidth(2);
    pen.setStyle(Qt::DashLine);
    painter.setPen(pen);
    painter.setRenderHints(QPainter::Antialiasing);
    painter.drawLine(QLineF(0,0,x,y));
    painter.drawLine(QLineF(0,y,x,0));

    QBrush brush;
    QTransform transform;
    transform.scale(0.25, 0.25);
    brush.setTransform(transform);
    brush.setTexture(pixmap);
    setBackgroundBrush(brush);
}
