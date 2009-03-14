//
// C++ Implementation: BoatAnimation
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

#include "boatanimation.h"
#include "situationmodel.h"
#include "trackmodel.h"
#include "boatmodel.h"
#include "boat.h"

/**
    Populates the internal lists of Positions and Headings function
    of time

    This method parses the path of the \a track, does some stalling
    analysis and determines position and heading as a function of time.

    It also gives the pale color to \a track boats, while keeping
    original color for animation \a boat.
*/

BoatAnimation::BoatAnimation(TrackModel *track, BoatGraphicsItem *boat, int maxSize, QGraphicsItemAnimation *parent)
    : QGraphicsItemAnimation(parent),
    m_track(track),
    m_boat(boat),
    m_maxSize(maxSize),
    m_time(QTime::currentTime()) {

    QPainterPath path = m_track->path();
    qreal size = m_track->boats().size() - 1;

    QPointF point = path.elementAt(0);
    setPosAt(0,point);
    BoatModel *model = m_track->boats()[0];
    qreal layline = m_track->situation()->laylineAngle();
    Boats::Series series = m_track->series();
    setRotationAt(0,model->heading());
    setsailAt(0, model->getSailAngle(layline, model->heading(), series, model->trim()));

    for (int i=0; i< size; i++) {
        qreal index;
        QPointF c1 = path.elementAt(i*3+1);
        QPointF c2 = path.elementAt(i*3+2);
        QPointF end = path.elementAt(i*3+3);
        bool stalled = ((point == c1) || (c2 == end));
        QPainterPath curve(point);
        curve.cubicTo(c1,c2,end);
        qreal length = curve.length();
        for (int j=1; j<=4; j++) {
            qreal percent = curve.percentAtLength(length*j/4.0);
            index = (i*4.0+j)/(maxSize*4.0);
            setPosAt(index, curve.pointAtPercent(percent));
            if (!stalled) {
                setRotationAt(index, fmod(360+90-curve.angleAtPercent(percent),360.0));
            }
        }
        model = m_track->boats()[i+1];
        if (stalled) {
            setRotationAt(index, fmod(model->heading(),360.0));
        }
        setsailAt(index, model->getSailAngle(layline, model->heading(), series, model->trim()));
        point = end;
    }

    QColor color = m_track->color();
    color.setAlpha(64);
    m_track->setColor(color);
    color.setAlpha(255);
    m_boat->setColor(color);
    m_boat->setOrder(0);

    if (m_rotationList.isEmpty())
        m_rotationList = rotationList();
}

/**
    Resets the color and List of BoatModels in the track
*/

BoatAnimation::~BoatAnimation() {
    QColor color = m_track->color();
    color.setAlpha(255);
    m_track->setColor(color);
    while (!m_boats.isEmpty()) {
        m_track->addBoat(m_boats.last());
        m_boats.pop_back();
    }
}

/**
    Calculates an intermediate heading value that makes the least
    angle change between 2 values, unlike \m linearRotationForStep()
*/

qreal BoatAnimation::linearAngleForStep(PairList pairList, qreal step, qreal defaultValue) const {
    const PairList *source = &pairList;
    step = qMin<qreal>(qMax<qreal>(step, 0), 1);

    if (step == 1)
        return source->last().second;

    qreal stepBefore = 0;
    qreal stepAfter = 1;
    qreal valueBefore = source->first().first == 0 ? source->first().second : defaultValue;
    qreal valueAfter = source->last().second;

    // Find the closest step and value before the given step.
    for (int i = 0; i < source->size() && step >= source->at(i).first; ++i) {
        stepBefore = source->at(i).first;
        valueBefore = source->at(i).second;
    }

    // Find the closest step and value after the given step.
    for (int j = source->size() - 1; j >= 0 && step < source->at(j).first; --j) {
        stepAfter = source->at(j).first;
        valueAfter = source->at(j).second;
    }

    qreal minValue = qMin<qreal>(valueBefore,valueAfter);
    qreal maxValue = qMax<qreal>(valueBefore,valueAfter);

    if (maxValue - minValue <= 360 + minValue - maxValue) {
        // Do a simple linear interpolation.
        return valueBefore + (valueAfter - valueBefore) * ((step - stepBefore) / (stepAfter - stepBefore));
    } else {
        // Do a reverse linear interpolation.
        if (valueBefore > valueAfter)
            return valueBefore + (360 + valueAfter - valueBefore) * ((step - stepBefore) / (stepAfter - stepBefore));
        else
            return valueBefore + (valueAfter - valueBefore - 360) * ((step - stepBefore) / (stepAfter - stepBefore));

    }
}

/**
    Returns the interpolated value of heading at \a step
*/

qreal BoatAnimation::headingAt(qreal step) const {
    if (step < 0.0 || step > 1.0)
        qWarning("BoatAnimation::headingAt: invalid step = %f", step);
    return linearAngleForStep(m_rotationList, step);
}


qreal BoatAnimation::sailAt(qreal step) const {
    if (step < 0.0 || step > 1.0)
        qWarning("BoatAnimation::sailAt: invalid step = %f", step);
    return linearAngleForStep(m_sailList, step);
}

/**
    Updates TrackModel and animation boat position and heading
    at \a step time, with a rate limit of one refresh every 40ms
*/

void BoatAnimation::afterAnimationStep(qreal step) {
    // limit update rate to 40ms
    if ((m_time.elapsed() < 40) && (step != 0) && (step != 1)) {
        return;
    }

    m_boat->setPosition(posAt(step));
    qreal heading = headingAt(step);
    m_boat->setHeading(heading);
    qreal sailAngle = BoatModel::getSailAngle(m_track->situation()->laylineAngle(), heading, m_track->series(), 0);
    m_boat->setTrim(sailAt(step)- sailAngle);

    int index = (int)(step * m_maxSize);
    for (int i=m_track->size()-1; i > index; i--) {
        BoatModel *boat = m_track->boats()[i];
        m_boats.push_back(boat);
        m_track->deleteBoat(boat);
    }
    if (!m_boats.isEmpty()) {
        for (int i = m_track->size()-1; i < index; i++) {
            m_track->addBoat(m_boats.last());
            m_boats.pop_back();
        }
    }

    // trigger next update rate calculation
    m_time.start();
}
