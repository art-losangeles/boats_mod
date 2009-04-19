//
// C++ Implementation: positionmodel
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

#include "positionmodel.h"
#include "commontypes.h"

extern int debugLevel;

PositionModel::PositionModel(QObject *parent)
        : QObject(parent),
        m_position() {
    if (debugLevel & 1 << MODEL) std::cout << "new Position " << this << std::endl;
}

PositionModel::~PositionModel() {
    if (debugLevel & 1 << MODEL) std::cout << "delete Position " << this << std::endl;
}

void PositionModel::setPosition(const QPointF& theValue) {
    if (theValue != m_position) {
        if (debugLevel & 1 << MODEL) std::cout << "Position " << this
        << " position " << theValue.x()
        << ", " << theValue.y() << std::endl;
        m_position = theValue;
        emit positionChanged(m_position);
    }
}

void PositionModel::setOrder(const int theValue) {
    if (theValue != m_order) {
        if (debugLevel & 1 << MODEL) std::cout << "Position " << this
        << " order " << theValue << std::endl;
        m_order = theValue;
        emit orderChanged(m_order);
    }
}

void PositionModel::appendDiscardedXml(const QString& theValue) {
    if (!m_discardedXml.contains(theValue)) {
        m_discardedXml.append(theValue);
    }
}
