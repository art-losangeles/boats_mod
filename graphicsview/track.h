//
// C++ Interface: BoatGraphicsItem
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
#ifndef TRACK_H
#define TRACK_H

#include <QGraphicsItem>
#include <QObject>
#include <QColor>

class TrackModel;

/**
    \class TrackGraphicsItem

    \brief the QGraphicsItem for a track

    The class represents the Item according to the Graphics View Framework.

    It inherits QGraphicsItem for Item framework and QObject for slot and
    signal framework. It displays the representation of a TrackModel on a
    SituationView.

    \sa SituationView, SituationScene, SituationModel, TrackModel

*/

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

        /// \a m_track holds the TrackModel being represented
        TrackModel *m_track;
};

#endif
