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

        QPointF pos0(m_track->m_boats[0]->position());
        qreal heading0 = m_track->m_boats[0]->heading()  * M_PI /180;

        path.moveTo(pos0);
        QListIterator<BoatModel*> boatI(m_track->m_boats);
        boatI.next();
        while (boatI.hasNext()) {
            BoatModel *boat = boatI.next();
            QPointF pos1(boat->position());
            qreal heading1 = (boat->heading()) * M_PI /180;
            path.cubicTo(pos0 + QPointF(60*sin(heading0),-60*cos(heading0)),
                         pos1 - QPointF(60*sin(heading1),-60*cos(heading1)),
                         pos1);
            pos0 = pos1;
            heading0 = heading1;
        }
        setPath(path);

        update();
}

void TrackGraphicsItem::deleteItem(TrackModel *track) {
    if (track == m_track) {
        std::cout << "deleting trackGraphics for model" << m_track << std::endl;
        scene()->removeItem(this);
        delete this;
    }
}
