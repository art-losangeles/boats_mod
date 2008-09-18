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

#include "model/positionmodel.h"

PositionModel::PositionModel(QObject *parent)
        : QObject(parent),
        m_position() {
    std::cout << "new Position " << this << std::endl;
}

PositionModel::~PositionModel() {
    std::cout << "delete Position " << this << std::endl;
}

void PositionModel::setPosition(const QPointF& theValue, bool update) {
    if (theValue != m_position) {
        std::cout << "Position " << this
        << " position " << theValue.x()
        << ", " << theValue.y() << std::endl;
        m_position = theValue;
        if (update) {
            emit positionChanged(m_position);
        }
    }
}

void PositionModel::setOrder(const int theValue, bool update) {
    if (theValue != m_order) {
        std::cout << "Position " << this
        << " order " << theValue << std::endl;
        m_order = theValue;
        if (update)
            emit orderChanged(m_order);
    }
}

void PositionModel::appendDiscardedXml(const QString& theValue) {
    if (!m_discardedXml.contains(theValue)) {
        m_discardedXml.append(theValue);
    }
}
