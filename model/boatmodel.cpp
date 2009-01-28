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
#include <cmath>

#include "model/boatmodel.h"

#include "commontypes.h"
#include "model/trackmodel.h"

extern int debugLevel;

BoatModel::BoatModel(TrackModel* track, QObject *parent)
        : PositionModel(parent),
        m_heading(0),
        m_track(track) {
    if (debugLevel & 1 << MODEL) std::cout << "new Boat " << this << std::endl;
    setOrder(track->size()+1);
}

BoatModel::~BoatModel() {
    if (debugLevel & 1 << MODEL) std::cout << "delete Boat " << this << std::endl;
}

void BoatModel::setHeading(const qreal& theValue, bool update) {
    if (theValue != m_heading) {
        m_heading = fmod(theValue+360.0,360.0);
        if (update) {
            emit headingChanged(m_heading);
            m_track->changingTrack(m_track);
        }
    }
}

void BoatModel::setPosition(const QPointF& theValue, bool update) {
    PositionModel::setPosition(theValue, update);
    if (update) {
        m_track->changingTrack(m_track);
    }
}
