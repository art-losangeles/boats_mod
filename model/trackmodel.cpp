//
// C++ Implementation: trackmodel
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

#include "model/trackmodel.h"

#include "commontypes.h"
#include "model/situationmodel.h"
#include "model/boatmodel.h"

extern int debugLevel;

TrackModel::TrackModel(SituationModel *situation, QObject *parent)
        : QObject(parent),
        m_color(),
        m_series(Boats::unknown),
        m_situation(situation),
        m_length(0) {
    m_order = situation->size();
    if (debugLevel & 1 << MODEL) std::cout << "new track " << this << std::endl;
    switch (situation->size() % 6) {
        case 0: m_color = QColor(Qt::yellow); break;
        case 1: m_color = QColor(Qt::blue); break;
        case 2: m_color = QColor(Qt::green); break;
        case 3: m_color = QColor(Qt::red); break;
        case 4: m_color = QColor(Qt::cyan); break;
        case 5: m_color = QColor(Qt::magenta); break;
    }
    setSeries(situation->situationSeries());
}

TrackModel::~TrackModel() {
    if (debugLevel & 1 << MODEL) std::cout << "end track " << this << std::endl;
}

void TrackModel::setOrder(const int theValue) {
    if (theValue != m_order) {
        m_order = theValue;
        emit orderChanged(m_order);
    }
}

void TrackModel::setColor(const QColor& theValue) {
    if (theValue != m_color) {
        m_color = theValue;
        emit colorChanged(m_color);
    }
}

void TrackModel::setSeries(const Boats::Series theValue) {
    if (theValue != m_series) {
        m_series = theValue;
        m_length = m_situation->sizeForSeries(m_series);
        emit seriesChanged(m_series);
    }
}

void TrackModel::appendDiscardedXml(const QString& theValue) {
    if (!m_discardedXml.contains(theValue)) {
        m_discardedXml.append(theValue);
    }
}


BoatModel * TrackModel::addBoat(BoatModel *boat, int order) {
    if (order == 0) {
        order = m_boats.size();
    }
    m_boats.insert(order, boat);
    if (debugLevel & 1 << MODEL) std::cout << "Adding Boat " << order+1 << std::endl;
    for (int i=order+1; i<m_boats.size(); i++) {
        m_boats[i]->setOrder(i+1);
    }
    m_situation->addingBoat(boat);
    emit changingTrack(this);
    return boat;
}

int TrackModel::deleteBoat(BoatModel *boat) {
    int order = m_boats.indexOf(boat);
    m_boats.removeOne(boat);
    if (debugLevel & 1 << MODEL) std::cout << "Removing Boat " << order+1 << std::endl;
    for (int i=order; i<m_boats.size(); i++) {
        m_boats[i]->setOrder(i+1);
    }
    m_situation->removingBoat(boat);
    emit changingTrack(this);
    return order;
}

void TrackModel::displayBoats() {
    if (debugLevel & 1 << MODEL) std::cout << "Displaying boats" << std::endl;
    foreach (BoatModel* boat, m_boats) {
        m_situation->addingBoat(boat);
    }
}

void TrackModel::hideBoats() {
    if (debugLevel & 1 << MODEL) std::cout << "Hiding boats" << std::endl;
    foreach (BoatModel* boat, m_boats) {
        m_situation->removingBoat(boat);
    }
}

void TrackModel::changingTrack(TrackModel *track) {
    QPainterPath path;

    if (m_boats.size() < 1) {
        m_path = path;
        return;
    }

    QPointF pos0(m_boats[0]->position());
    qreal heading0 = m_boats[0]->heading();

    path.moveTo(pos0);
    QListIterator<BoatModel*> boatI(m_boats);
    boatI.next();
    while (boatI.hasNext()) {
        BoatModel *boat = boatI.next();
        QPointF pos1(boat->position());
        qreal heading1 = boat->heading();

        // distance and angle between positions
        QPointF delta = pos1-pos0;
        qreal dist = sqrt(pow(delta.x(),2) + pow(delta.y(),2));
        qreal theta = fmod(atan2 (delta.x(), -delta.y()) *180/M_PI +360, 360);

        // empirical factor for control point distance
        qreal factor = dist*2/5;

        QPointF c1(pos0);
        // stalled condition when next boat in the back
        qreal angle0 = fmod(theta - heading0 +360, 360);
        bool stalled0 = ((angle0 >= 90) && (angle0 <= 270));
        if (!stalled0) {
            c1 += QPointF(factor*sin(heading0 * M_PI /180), -factor*cos(heading0 * M_PI /180));
        }

        QPointF c2(pos1);
        // stalled condition when previous boat in the back
        qreal angle1 = fmod(theta- heading1 +360, 360);
        bool stalled1 = ((angle1 >= 90) && (angle1 <= 270)) ;
        if (!stalled1) {
            c2 -= QPointF(factor*sin(heading1 * M_PI /180), -factor*cos(heading1 * M_PI /180));
        }

        path.cubicTo(c1, c2, pos1);
        pos0 = pos1;
        heading0 = heading1;
    }
    m_path = path;
    emit trackChanged(track);
}
