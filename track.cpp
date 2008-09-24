//
// C++ Implementation: TrackGraphicsItem
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
#include <math.h>

#include <QPainter>
#include <QGraphicsScene>


#include "track.h"

#include "model/situationmodel.h"
#include "model/trackmodel.h"
#include "model/boatmodel.h"

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
        std::cout << "deleting trackGraphics for model" << m_track << std::endl;
        scene()->removeItem(this);
        delete this;
    }
}
