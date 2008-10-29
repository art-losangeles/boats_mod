//
// C++ Interface: BoatGraphicsItem
//
// Description:
//
//
// Author: Thibaut GRIDEL <tgridel@free.fr>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef TRACK_H
#define TRACK_H

#include <QGraphicsItem>
#include <QObject>
#include <QColor>

class TrackModel;

class TrackGraphicsItem : public QObject, public QGraphicsPathItem {
        Q_OBJECT
    public:
        TrackGraphicsItem(TrackModel *track = 0, QGraphicsItem *parent = 0);
        ~TrackGraphicsItem();

        TrackModel* track() const { return m_track; }

    public slots:
        void setTrack();
        void deleteItem(TrackModel *track);

    private:
        TrackModel *m_track;
};

#endif
