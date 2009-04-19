//
// C++ Implementation: boatmodel
//
// Description:
//
//
// Author: Thibaut GRIDEL <tgridel@free.fr>
//
// Copyright (c) 2008-2009 Thibaut GRIDEL
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
//
#include <iostream>
#include <cmath>

#include "boatmodel.h"

#include "commontypes.h"
#include "situationmodel.h"
#include "trackmodel.h"

extern int debugLevel;

BoatModel::BoatModel(TrackModel* track, QObject *parent)
        : PositionModel(parent),
        m_heading(0),
        m_trim(0),
        m_track(track) {
    if (debugLevel & 1 << MODEL) std::cout << "new Boat " << this << std::endl;
    setOrder(track->size()+1);
}

BoatModel::~BoatModel() {
    if (debugLevel & 1 << MODEL) std::cout << "delete Boat " << this << std::endl;
}

void BoatModel::setHeading(const qreal& theValue) {
    if (theValue != m_heading) {
        m_heading = fmod(theValue+360.0,360.0);
        emit headingChanged(m_heading);
        m_track->changingTrack(m_track);
    }
}

void BoatModel::setPosition(const QPointF& theValue) {
    PositionModel::setPosition(theValue);
    m_track->changingTrack(m_track);
}

void BoatModel::setTrim(const qreal& theValue) {
    qreal layline = m_track->situation()->laylineAngle();
    qreal sailAngle = getSailAngle(layline, m_heading, m_track->series(), 0);
    qreal newAngle = sailAngle + theValue;
    if (theValue != m_trim
        && newAngle < 180
        && newAngle > -180) {
        m_trim = theValue;
        emit trimChanged(m_trim);
    }
}

void BoatModel::setOverlap(const  Boats::Overlaps theValue) {
    if (theValue != m_overlap) {
        if (debugLevel & 1 << MODEL) std::cout
                << "overlap = " << theValue  << std::endl;
        m_overlap = theValue;
        emit overlapChanged(m_overlap);
    }
}

qreal BoatModel::getSailAngle(qreal layline, qreal heading, Boats::Series series, qreal trim) {
    qreal sailAngle;

    // within 10° inside layline angle, the sail is headed
    if (heading < layline-10) {
        sailAngle =  heading + trim;
    } else if (heading > 360 - (layline-10)) {
        sailAngle =  heading - 360 + trim;
    } else {

        switch (series) {
            // tornado has fixed 20° incidence
        case Boats::tornado:
            if (heading<180) {
                sailAngle = 20 + trim;
            } else {
                sailAngle = - 20  + trim;
            }
            break;
        default:
            // linear incidence variation
            // incidence is 15° at layline angle and 90° downwind
            qreal a = (180 - layline) / 75;
            qreal b = layline / a - 15;
            if (heading<180) {
                sailAngle = heading/a - b + trim;
            } else {
                sailAngle = heading/a - b - 180 + trim;
            }
            break;
        }
    }

    if (debugLevel & 1 << MODEL) std::cout
            << "heading = " << heading
            << " trim = " << trim
            << " sail = " << sailAngle
            << std::endl;
    return sailAngle;
}

