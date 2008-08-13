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

#include <QObject>
#include <QPointF>

class TrackModel;

class BoatModel : public QObject {
        Q_OBJECT
    public:
        BoatModel(TrackModel *track, QObject *parent = 0);
        ~BoatModel();

        TrackModel* track() const { return m_track; }
        QPointF position() const { return m_position; }
        qreal heading() const { return m_heading; }
        int order() const { return m_order; }

        void setPosition(const QPointF& theValue, bool update = false);
        void setHeading(const qreal& theValue, bool update = false);
        void setOrder(const int theValue, bool update = false);

    signals:
        void headingChanged(qreal heading);
        void positionChanged(QPointF position);
        void orderChanged(int order);

    private:
        TrackModel *m_track;
        QPointF m_position;
        qreal  m_heading;
        int m_order;

};

#endif
