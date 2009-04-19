//
// C++ Interface: boatmodel
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
#ifndef BOATMODEL_H
#define BOATMODEL_H

#include <QtGui>

#include "boats.h"
#include "positionmodel.h"

class TrackModel;

/**
    \class BoatModel

    \brief The Model for a Boat Position

    The class represents  the Model for a Boat, according to an
    Observer Pattern.

    BoatModel inherits PositionModel and contains data which represents
    one boat at a given time, like its position and heading.

    It shall not be mistaken with TrackModel, which holds the list of all
    positions where a Boat has navigated.
    More exactly, a TrackModel will hold a List of BoatModels.

    \sa SituationModel, TrackModel

*/

class BoatModel : public PositionModel {
        Q_OBJECT
    public:
        BoatModel(TrackModel *track, QObject *parent = 0);
        ~BoatModel();

        // Setters and Getters for Model Data
        virtual void setPosition(const QPointF& theValue);

        qreal heading() const { return m_heading; }
        void setHeading(const qreal& theValue);

        qreal trim() const { return m_trim; }
        void setTrim(const qreal& theValue);

        Boats::Overlaps overlap() const {return m_overlap; }
        void setOverlap(const Boats::Overlaps theValue);

        // Setters and Getters for Non model Data
        TrackModel* track() const { return m_track; }

        static qreal getSailAngle(qreal layline, qreal heading, Boats::Series series, qreal trim);

    signals:
        void headingChanged(qreal heading);
        void trimChanged(qreal trim);
        void overlapChanged(Boats::Overlaps overlap);

    private:
        // Model Data
        /// \a m_heading holds the heading of a Boat
        qreal  m_heading;

        /// \a m_trim holds the sailing trim of a Boat
        qreal m_trim;

        /// \a m_overlap holds whether an overlap line should be displayed
        Boats::Overlaps m_overlap;

        // Non model Data
        /// \a m_track keeps a pointer to the TrackModel to which
        /// it belongs
        TrackModel *m_track;
};

#endif
