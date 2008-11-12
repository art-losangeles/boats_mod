//
// C++ Interface: BoatAnimation
//
// Description:
//
//
// Author: Thibaut GRIDEL <tgridel@free.fr>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef BOATANIMATION_H
#define BOATANIMATION_H

#include <QGraphicsItemAnimation>

class BoatGraphicsItem;
class TrackModel;
class BoatModel;

class BoatAnimation : public QGraphicsItemAnimation {
    public:
        BoatAnimation(TrackModel *track, BoatGraphicsItem *boat, int maxSize,  QGraphicsItemAnimation *parent = 0);
        ~BoatAnimation();
        BoatGraphicsItem *boat() const {return m_boat; };

        qreal headingAt(qreal step) const;

    protected:
        virtual void afterAnimationStep(qreal step);

    private:
        qreal linearHeadingForStep(qreal step, qreal defaultValue = 0) const;
        QList<QPair<qreal, qreal> > m_rotationList;

        TrackModel *m_track;
        QList<BoatModel*> m_boats;
        BoatGraphicsItem *m_boat;
        int m_maxSize;
        QTime m_time;
};

#endif
