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

#include "commontypes.h"
#include "model/situationmodel.h"

extern int debugLevel;

MarkModel::MarkModel(SituationModel* situation, QObject *parent)
        : PositionModel(parent),
        m_situation(situation),
        m_color(Qt::gray),
        m_zone(false) {
    if (debugLevel & 1 << MODEL) std::cout << "new Mark " << this << std::endl;
    setOrder(situation->markSize()+1);
}

MarkModel::~MarkModel() {
    if (debugLevel & 1 << MODEL) std::cout << "delete Mark " << this << std::endl;
}

void MarkModel::setColor(const QColor& theValue, bool update) {
    if (theValue != m_color) {
        m_color = theValue;
        if (update)
            emit colorChanged(m_color);
    }
}

void MarkModel::setZone(const bool theValue, bool update) {
    if (theValue != m_zone) {
        if (debugLevel & 1 << MODEL) std::cout << "Mark " << this
        << " zone " << theValue << std::endl;
        m_zone = theValue;
        if (update)
            emit zoneChanged(m_zone);
    }
}
