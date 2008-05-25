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

TrackModel::~TrackModel() {}

BoatModel * TrackModel::addBoat() {
    BoatModel *boat = new BoatModel(this, this);
    std::cout << "Adding Boat " << m_boats.size() << std::endl;
    m_boats.push_back(boat);
    emit boatAdded(boat);
    return boat;
}

void TrackModel::deleteBoat(BoatModel *boat) {
    std::cout << "Removing Boat " << m_boats.indexOf(boat) << std::endl;
    m_boats.removeOne(boat);
    emit boatRemoved(boat);
    delete boat;
    if (size() == 0)
        m_situation->deleteTrack(this);
}
