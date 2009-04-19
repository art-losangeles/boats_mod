//
// C++ Implementation: TrackGraphicsItem
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
#include <cmath>

#include <QPainter>
#include <QGraphicsScene>


#include "track.h"

#include "commontypes.h"
#include "situationmodel.h"
#include "trackmodel.h"

extern int debugLevel;

TrackGraphicsItem::TrackGraphicsItem(TrackModel *track, QGraphicsItem *parent)
        : QGraphicsPathItem(parent),
        m_track(track) {
    setZValue(0);
    setTrack();

    connect(track, SIGNAL(trackChanged(TrackModel*)),
            this, SLOT(setTrack()));
    connect(track->situation(), SIGNAL(trackRemoved(TrackModel*)),
            this, SLOT(deleteItem(TrackModel*)));
}


TrackGraphicsItem::~TrackGraphicsItem() {}

void TrackGraphicsItem::setTrack() {
        QPainterPath path;

        prepareGeometryChange();
        setPath(m_track->path());
        update();
}

void TrackGraphicsItem::deleteItem(TrackModel *track) {
    if (track == m_track) {
        if (debugLevel & 1 << VIEW) std::cout << "deleting trackGraphics for model" << m_track << std::endl;
        scene()->removeItem(this);
        delete this;
    }
}
