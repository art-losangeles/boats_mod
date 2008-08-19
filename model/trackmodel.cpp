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

#include "model/situationmodel.h"
#include "model/trackmodel.h"
#include "model/boatmodel.h"

TrackModel::TrackModel(SituationModel *situation, QObject *parent)
        : QObject(parent),
        m_situation(situation),
        m_color() {
    static int track_id = 0;
    std::cout << "new track " << this << std::endl;

    switch (track_id % 6) {
        case 0: m_color = QColor(Qt::yellow); break;
        case 1: m_color = QColor(Qt::blue); break;
        case 2: m_color = QColor(Qt::green); break;
        case 3: m_color = QColor(Qt::red); break;
        case 4: m_color = QColor(Qt::cyan); break;
        case 5: m_color = QColor(Qt::magenta); break;
    }
    track_id++;
}

TrackModel::~TrackModel() {
    std::cout << "end track " << this << std::endl;
}

void TrackModel::setColor(const QColor& theValue, bool update) {
    if (theValue != m_color) {
        m_color = theValue;
        if (update)
            emit colorChanged(m_color);
    }
}


BoatModel * TrackModel::addBoat(BoatModel *boat, int order) {
    if (order == 0) {
        order = m_boats.size();
    }
    m_boats.insert(order, boat);
    std::cout << "Adding Boat " << order << std::endl;
    for (int i=order+1; i<m_boats.size(); i++) {
        m_boats[i]->setOrder(i, true);
    }
    m_situation->addingBoat(boat);
    emit changingTrack(this);
    return boat;
}

int TrackModel::deleteBoat(BoatModel *boat) {
    int order = m_boats.indexOf(boat);
    m_boats.removeOne(boat);
    std::cout << "Removing Boat " << order+1 << std::endl;
    for (int i=order; i<m_boats.size(); i++) {
        m_boats[i]->setOrder(i, true);
    }
    m_situation->removingBoat(boat);
    emit changingTrack(this);
    return order;
}

void TrackModel::displayBoats() {
    std::cout << "Displaying boats" << std::endl;
    foreach (BoatModel* boat, m_boats) {
        m_situation->addingBoat(boat);
    }
}

void TrackModel::hideBoats() {
    std::cout << "Hiding boats" << std::endl;
    foreach (BoatModel* boat, m_boats) {
        m_situation->removingBoat(boat);
    }
}
