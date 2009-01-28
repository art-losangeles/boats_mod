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
        m_laylineAngle(40),
        m_situationSeries(Boats::keelboat),
        m_situationLength(3),
        m_undoStack(new QUndoStack(this)) {
    if (debugLevel & 1 << MODEL) std::cout << "new situation " << this << std::endl;
}

SituationModel::~SituationModel() {
    if (debugLevel & 1 << MODEL) std::cout << "end situation " << this << std::endl;
}

int SituationModel::sizeForSeries(const Boats::Series series) {
    int size;
    switch (series) {
        case Boats::keelboat:
            size = 100;
            break;
        case Boats::laser:
            size = 40;
            break;
        case Boats::optimist:
            size = 23;
            break;
        case Boats::tornado:
            size = 61;
            break;
        default:
            size = 100;
            break;
    }
    return size;
}

void SituationModel::setTitle(const QString theValue, bool update) {
    if (theValue != m_title) {
        if (debugLevel & 1 << MODEL) std::cout << "Setting Title " << theValue.toStdString() << std::endl;
        m_title = theValue;
        if (update)
            emit titleChanged(m_title);
    }
}

void SituationModel::setRules(const QString theValue, bool update) {
    if (theValue != m_rules) {
        if (debugLevel & 1 << MODEL) std::cout << "Setting Rules " << theValue.toStdString() << std::endl;
       m_rules = theValue;
        if (update)
            emit rulesChanged(m_rules);
    }
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
        << " Series " << ENUM_NAME(Boats, Series, theValue) << std::endl;
        m_situationSeries = (Boats::Series) theValue;
        foreach(TrackModel *track, m_tracks) {
            track->setSeries(m_situationSeries, true);
        }
        if (update) {
            emit seriesChanged(m_situationSeries);
            emit laylineChanged(m_laylineAngle);
        }
    }
}

void SituationModel::setSituationLength(const int theValue, bool update) {
    if (theValue != m_situationLength) {
        if (debugLevel & 1 << MODEL) std::cout << "Situation " << this
        << " Length " << theValue << std::endl;
        m_situationLength = theValue;
        foreach(MarkModel *mark, m_marks) {
            mark->setLength(m_situationLength, true);
        }
        if (update)
            emit lengthChanged(m_situationLength);
    }
}

void SituationModel::setAbstract(const QString theValue, bool update) {
    if (theValue != m_abstract) {
        if (debugLevel & 1 << MODEL) std::cout << "Setting Abstract " << theValue.toStdString() << std::endl;
        m_abstract = theValue;
        if (update)
            emit abstractChanged(m_abstract);
    }
}

void SituationModel::setDescription(const QString theValue, bool update) {
    if (theValue != m_description) {
        if (debugLevel & 1 << MODEL) std::cout << "Setting Description " << theValue.toStdString() << std::endl;
        m_description = theValue;
        if (update)
            emit descriptionChanged(m_description);
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
    for (int i=index; i<m_marks.size(); i++) {
        m_marks[i]->setOrder(i+1, true);
    }
    emit markRemoved(mark);
    return index;
}
