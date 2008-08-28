//
// C++ Implementation: markmodel
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

#include "model/markmodel.h"
#include "model/situationmodel.h"

MarkModel::MarkModel(SituationModel* situation, QObject *parent)
        : QObject(parent),
        m_situation(situation),
        m_position(),
        m_order(situation->markSize()) {
    std::cout << "new Mark " << this << std::endl;
}

MarkModel::~MarkModel() {
    std::cout << "delete Mark " << this << std::endl;
}

void MarkModel::setPosition(const QPointF& theValue, bool update) {
    if (theValue != m_position) {
        std::cout << "Mark " << this
        << " position " << theValue.x()
        << ", " << theValue.y() << std::endl;
        m_position = theValue;
        if (update) {
            emit positionChanged(m_position);
        }
    }
}

void MarkModel::setOrder(const int theValue, bool update) {
    if (theValue != m_order) {
        std::cout << "Mark " << this
        << " order " << theValue << std::endl;
        m_order = theValue;
        if (update)
            emit orderChanged(m_order);
    }
}

void MarkModel::appendDiscardedXml(const QString& theValue) {
    if (!m_discardedXml.contains(theValue)) {
        m_discardedXml.append(theValue);
    }
}
