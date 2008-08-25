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
#include "model/boatmodel.h"

SituationModel::SituationModel(QObject *parent)
        : QObject(parent),
        m_undoStack(new QUndoStack(this)),
        m_laylineAngle(40) {
    std::cout << "new situation " << this << std::endl;
}

SituationModel::~SituationModel() {
    std::cout << "end situation " << this << std::endl;
}

void SituationModel::setLaylineAngle(const qreal theValue) {
    if (theValue != m_laylineAngle) {
        std::cout << "Situation " << this
        << " Layline Angle " << theValue << std::endl;
        m_laylineAngle = theValue;
    }
}

void SituationModel::addTrack(TrackModel *track) {
    m_tracks.push_back(track);
    std::cout << "Adding Track " << m_tracks.size() << std::endl;
    track->displayBoats();
    emit trackAdded(track);
}

void SituationModel::deleteTrack(TrackModel *track) {
    int index = m_tracks.indexOf(track);
    std::cout << "Removing Track " << index
    << " with " << track->size() << std::endl;
    track->hideBoats();
    m_tracks.removeOne(track);
    emit trackRemoved(track);
}
