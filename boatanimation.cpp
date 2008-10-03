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

#include <QtGui>

#include "boatanimation.h"
#include "model/trackmodel.h"
#include "model/boatmodel.h"
#include "boat.h"

BoatAnimation::BoatAnimation(TrackModel *track, BoatGraphicsItem *boat, int maxSize, QGraphicsItemAnimation *parent)
    : QGraphicsItemAnimation(parent),
    m_track(track),
    m_boat(boat),
    m_maxSize(maxSize) {

    QPainterPath path = m_track->path();
    qreal size = m_track->boats().size() - 1;

    QPointF point = path.elementAt(0);
    setPosAt(0,point);
    setRotationAt(0,m_track->boats()[0]->heading());

    for (int i=0; i< size; i++) {
        QPointF c1 = path.elementAt(i*3+1);
        QPointF c2 = path.elementAt(i*3+2);
        QPointF end = path.elementAt(i*3+3);
        QPainterPath curve(point);
        curve.cubicTo(c1,c2,end);
        qreal length = curve.length();
        for (int j=1; j<=4; j++) {
            qreal percent = curve.percentAtLength(length*j/4.0);
            qreal index = (i*4.0+j)/(maxSize*4.0);
            setPosAt(index, curve.pointAtPercent(percent));
            setRotationAt(index, fmod(360+90-curve.angleAtPercent(percent),360.0));
        }
        point = end;
    }

    QColor color = m_track->color();
    color.setAlpha(128);
    m_track->setColor(color, true);
    color.setAlpha(255);
    m_boat->setColor(color);

    if (m_rotationList.isEmpty())
        m_rotationList = rotationList();
}

BoatAnimation::~BoatAnimation() {
    QColor color = m_track->color();
    color.setAlpha(255);
    m_track->setColor(color, true);
    while (!m_boats.isEmpty()) {
        m_track->addBoat(m_boats.last());
        m_boats.pop_back();
    }
}

qreal BoatAnimation::linearHeadingForStep(qreal step, qreal defaultValue) const {
    const QList<QPair<qreal, qreal> > *source = &m_rotationList;
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

qreal BoatAnimation::headingAt(qreal step) const {
    if (step < 0.0 || step > 1.0)
        qWarning("BoatAnimation::headingAt: invalid step = %f", step);
    return linearHeadingForStep(step);
}

void BoatAnimation::afterAnimationStep(qreal step) {
    m_boat->setPosition(posAt(step));
    m_boat->setHeading(headingAt(step));

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
}
