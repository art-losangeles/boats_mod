//
// C++ Implementation: markmodel
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

#include "markmodel.h"

#include "commontypes.h"
#include "situationmodel.h"

extern int debugLevel;

MarkModel::MarkModel(SituationModel* situation, QObject *parent)
        : PositionModel(parent),
        m_color(Qt::gray),
        m_zone(false),
        m_situation(situation),
        m_length(situation->situationLength()) {
    if (debugLevel & 1 << MODEL) std::cout << "new Mark " << this << std::endl;
    setOrder(situation->markSize()+1);
}

MarkModel::~MarkModel() {
    if (debugLevel & 1 << MODEL) std::cout << "delete Mark " << this << std::endl;
}

void MarkModel::setColor(const QColor& theValue) {
    if (theValue != m_color) {
        m_color = theValue;
        emit colorChanged(m_color);
    }
}

void MarkModel::setZone(const bool theValue) {
    if (theValue != m_zone) {
        if (debugLevel & 1 << MODEL) std::cout << "Mark " << this
        << " zone " << theValue << std::endl;
        m_zone = theValue;
        emit zoneChanged(m_zone);
    }
}

void MarkModel::setLength(const int theValue) {
    if (theValue != m_length) {
        if (debugLevel & 1 << MODEL) std::cout << "Mark " << this
        << " length " << theValue << std::endl;
        m_length = theValue;
        emit lengthChanged(m_length);
    }
}
