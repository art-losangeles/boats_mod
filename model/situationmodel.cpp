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

#include "commontypes.h"
#include "model/trackmodel.h"
#include "model/boatmodel.h"
#include "model/markmodel.h"

extern int debugLevel;

SituationModel::SituationModel(QObject *parent)
        : QObject(parent),
        m_undoStack(new QUndoStack(this)),
        m_laylineAngle(40),
        m_situationSeries(KEELBOAT) {
    if (debugLevel & 1 << MODEL) std::cout << "new situation " << this << std::endl;
    m_seriesNames << "keelboat" << "optimist" << "tornado";
}

SituationModel::~SituationModel() {
    if (debugLevel & 1 << MODEL) std::cout << "end situation " << this << std::endl;
}

void SituationModel::setLaylineAngle(const int theValue, bool update) {
    if (theValue != m_laylineAngle) {
        if (debugLevel & 1 << MODEL) std::cout << "Situation " << this
        << " Layline Angle " << theValue << std::endl;
        m_laylineAngle = theValue;
        if (update)
            emit laylineChanged(m_laylineAngle);
    }
}

void SituationModel::setSituationSeries(const int theValue, bool update) {
    if (theValue != m_situationSeries) {
        if (debugLevel & 1 << MODEL) std::cout << "Situation " << this
        << " Series " << m_seriesNames[theValue].toStdString() << std::endl;
        m_situationSeries = (Series) theValue;
        foreach(TrackModel *track, m_tracks) {
            track->setSeries(m_situationSeries, true);
        }
        if (update)
            emit seriesChanged(m_situationSeries);
    }
}

void SituationModel::appendDiscardedXml(const QString& theValue) {
    if (!m_discardedXml.contains(theValue)) {
        m_discardedXml.append(theValue);
    }
}

void SituationModel::addTrack(TrackModel *track) {
    m_tracks.push_back(track);
    if (debugLevel & 1 << MODEL) std::cout << "Adding Track " << m_tracks.size() << std::endl;
    track->displayBoats();
    emit trackAdded(track);
}

void SituationModel::deleteTrack(TrackModel *track) {
    int index = m_tracks.indexOf(track);
    if (debugLevel & 1 << MODEL) std::cout << "Removing Track " << index
    << " with " << track->size() << std::endl;
    track->hideBoats();
    m_tracks.removeOne(track);
    emit trackRemoved(track);
}

void SituationModel::addMark(MarkModel *mark, int order) {
    if (order == 0) {
        order = m_marks.size();
    }
    m_marks.insert(order, mark);
    if (debugLevel & 1 << MODEL) std::cout << "Adding Mark " << order+1 << std::endl;
    for (int i=order+1; i<m_marks.size(); i++) {
        m_marks[i]->setOrder(i+1, true);
    }
    emit markAdded(mark);
}

int SituationModel::deleteMark(MarkModel *mark) {
    int index = m_marks.indexOf(mark);
    if (debugLevel & 1 << MODEL) std::cout << "Removing Mark " << index+1 << std::endl;
    m_marks.removeOne(mark);
    emit markRemoved(mark);
    return index;
}
