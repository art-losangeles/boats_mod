//
// C++ Implementation: situationmodel
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

SituationModel::SituationModel(QObject *parent)
        : QObject(parent) {}


TrackModel * SituationModel::addTrack() {
    TrackModel *track = new TrackModel(this, this);
    m_tracks.push_back(track);
    std::cout << "Adding Track " << m_tracks.size() << std::endl;
    emit trackAdded(track);
    track->addBoat();
    return track;
}

void SituationModel::deleteTrack(TrackModel *track) {
    int index = m_tracks.indexOf(track);
    std::cout << "Removing Track " << index
    << " with " << m_tracks[index]->size() << std::endl;
    m_tracks.removeOne(track);
    emit trackRemoved(track);
    delete track;
}
