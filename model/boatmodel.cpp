//
// C++ Implementation: boatmodel
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

#include "model/boatmodel.h"

BoatModel::BoatModel(TrackModel* track, QObject *parent)
        : QObject(parent),
        m_track(track),
        m_position(),
        m_heading(0) {
    std::cout << "new Boat " << this << std::endl;
}

BoatModel::~BoatModel() {
    std::cout << "delete Boat " << this << std::endl;
}

void BoatModel::setHeading(const qreal& theValue, bool update) {
    if (theValue != m_heading) {
        m_heading = theValue;
        if (update)
            emit headingChanged(m_heading);
    }
}

void BoatModel::setPosition(const QPointF& theValue, bool update) {
    if (theValue != m_position) {
        std::cout << "Boat " << this
        << " position " << theValue.x()
        << ", " << theValue.y() << std::endl;
        m_position = theValue;
        if (update)
            emit positionChanged(m_position);
    }
}
