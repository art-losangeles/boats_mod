//
// C++ Implementation: positionmodel
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
