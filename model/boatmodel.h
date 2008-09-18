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

class BoatModel : public PositionModel {
        Q_OBJECT
    public:
        BoatModel(TrackModel *track, QObject *parent = 0);
        ~BoatModel();

        TrackModel* track() const { return m_track; }
        qreal heading() const { return m_heading; }
        void setPosition(const QPointF& theValue, bool update = false);
        void setHeading(const qreal& theValue, bool update = false);

    signals:
        void headingChanged(qreal heading);

    private:
        TrackModel *m_track;
        qreal  m_heading;
};

#endif
