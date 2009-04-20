//
// C++ Implementation: situationmodel
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

#include "situationmodel.h"

#include "commontypes.h"
#include "trackmodel.h"
#include "markmodel.h"

extern int debugLevel;

SituationModel::SituationModel(QObject *parent)
        : QObject(parent),
        m_showLayline(true),
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
	case Boats::rib:
	    size = 60;
	    break;
        default:
            size = 100;
            break;
    }
    return size;
}

void SituationModel::setTitle(const QString theValue) {
    if (theValue != m_title) {
        if (debugLevel & 1 << MODEL) std::cout << "Setting Title " << theValue.toStdString() << std::endl;
        m_title = theValue;
        emit titleChanged(m_title);
    }
}

void SituationModel::setRules(const QString theValue) {
    if (theValue != m_rules) {
        if (debugLevel & 1 << MODEL) std::cout << "Setting Rules " << theValue.toStdString() << std::endl;
       m_rules = theValue;
       emit rulesChanged(m_rules);
    }
}

void SituationModel::setShowLayline(const bool theValue) {
    if (theValue != m_showLayline) {
        if (debugLevel & 1 << MODEL) std::cout << "Show Layline " << theValue << std::endl;
        m_showLayline = theValue;
        emit showLaylineChanged(m_showLayline);
        emit laylineChanged(m_laylineAngle);
    }
}

void SituationModel::setLaylineAngle(const int theValue) {
    if (theValue != m_laylineAngle) {
        if (debugLevel & 1 << MODEL) std::cout << "Situation " << this
        << " Layline Angle " << theValue << std::endl;
        m_laylineAngle = theValue;
        emit laylineChanged(m_laylineAngle);
    }
}

void SituationModel::setSituationSeries(const int theValue) {
    if (theValue != m_situationSeries) {
        if (debugLevel & 1 << MODEL) std::cout << "Situation " << this
        << " Series " << ENUM_NAME(Boats, Series, theValue) << std::endl;
        m_situationSeries = (Boats::Series) theValue;
        emit seriesChanged(m_situationSeries);
        emit laylineChanged(m_laylineAngle);
    }
}

void SituationModel::setSituationLength(const int theValue) {
    if (theValue != m_situationLength) {
        if (debugLevel & 1 << MODEL) std::cout << "Situation " << this
        << " Length " << theValue << std::endl;
        m_situationLength = theValue;
        foreach (MarkModel *mark, m_marks) {
            mark->setLength(m_situationLength);
        }
        emit lengthChanged(m_situationLength);
    }
}

void SituationModel::setAbstract(const QString theValue) {
    if (theValue != m_abstract) {
        if (debugLevel & 1 << MODEL) std::cout << "Setting Abstract " << theValue.toStdString() << std::endl;
        m_abstract = theValue;
        emit abstractChanged(m_abstract);
    }
}

void SituationModel::setDescription(const QString theValue) {
    if (theValue != m_description) {
        if (debugLevel & 1 << MODEL) std::cout << "Setting Description " << theValue.toStdString() << std::endl;
        m_description = theValue;
        emit descriptionChanged(m_description);
    }
}

void SituationModel::appendDiscardedXml(const QString& theValue) {
    if (!m_discardedXml.contains(theValue)) {
        m_discardedXml.append(theValue);
    }
}

void SituationModel::addTrack(TrackModel *track, int order) {
    if (order == 0) {
        order = m_tracks.size();
    }
    m_tracks.insert(order,track);
    if (debugLevel & 1 << MODEL) std::cout << "Adding Track " << order+1 <<  std::endl;
    for (int i=order+1; i<m_tracks.size(); i++) {
        m_tracks[i]->setOrder(i+1);
    }
    track->displayBoats();
    emit trackAdded(track);
}

void SituationModel::deleteTrack(TrackModel *track) {
    int index = m_tracks.indexOf(track);
    if (debugLevel & 1 << MODEL) std::cout << "Removing Track " << index+1
    << " with " << track->size() << std::endl;
    track->hideBoats();
    m_tracks.removeOne(track);
    for (int i=index; i<m_tracks.size(); i++) {
        m_tracks[i]->setOrder(i+1);
    }
    emit trackRemoved(track);
}

void SituationModel::addMark(MarkModel *mark, int order) {
    if (order == 0) {
        order = m_marks.size();
    }
    m_marks.insert(order, mark);
    if (debugLevel & 1 << MODEL) std::cout << "Adding Mark " << order+1 << std::endl;
    for (int i=order+1; i<m_marks.size(); i++) {
        m_marks[i]->setOrder(i+1);
    }
    emit markAdded(mark);
}

int SituationModel::deleteMark(MarkModel *mark) {
    int index = m_marks.indexOf(mark);
    if (debugLevel & 1 << MODEL) std::cout << "Removing Mark " << index+1 << std::endl;
    m_marks.removeOne(mark);
    for (int i=index; i<m_marks.size(); i++) {
        m_marks[i]->setOrder(i+1);
    }
    emit markRemoved(mark);
    return index;
}
