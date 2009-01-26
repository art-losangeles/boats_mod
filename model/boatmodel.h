//
// C++ Interface: boatmodel
//
// Description:
//
//
// Author: Thibaut GRIDEL <tgridel@free.fr>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef BOATMODEL_H
#define BOATMODEL_H

#include <QtGui>

#include "model/positionmodel.h"

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
        void setPosition(const QPointF& theValue, bool update = false);

        qreal heading() const { return m_heading; }
        void setHeading(const qreal& theValue, bool update = false);

        // Setters and Getters for Non model Data
        TrackModel* track() const { return m_track; }

    signals:
        void headingChanged(qreal heading);

    private:
        // Model Data
        /// \a m_heading holds the heading of a Boat
        qreal  m_heading;

        // Non model Data
        /// \a m_track keeps a pointer to the TrackModel to which
        /// it belongs
        TrackModel *m_track;
};

#endif
