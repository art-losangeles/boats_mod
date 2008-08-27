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
#include "model/trackmodel.h"

BoatModel::BoatModel(TrackModel* track, QObject *parent)
        : QObject(parent),
        m_track(track),
        m_position(),
        m_heading(0),
        m_order(track->size()) {
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
            m_track->changingTrack(m_track);
    }
}

void BoatModel::setPosition(const QPointF& theValue, bool update) {
    if (theValue != m_position) {
        std::cout << "Boat " << this
        << " position " << theValue.x()
        << ", " << theValue.y() << std::endl;
        m_position = theValue;
        if (update) {
            emit positionChanged(m_position);
            m_track->changingTrack(m_track);
        }
    }
}

void BoatModel::setOrder(const int theValue, bool update) {
    if (theValue != m_order) {
        std::cout << "Boat " << this
        << " order " << theValue << std::endl;
        m_order = theValue;
        if (update)
            emit orderChanged(m_order);
    }
}

void BoatModel::appendDiscardedXml(const QString& theValue) {
    if (!m_discardedXml.contains(theValue)) {
        m_discardedXml.append(theValue);
    }
}
