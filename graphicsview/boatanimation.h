//
// C++ Interface: BoatAnimation
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
#ifndef BOATANIMATION_H
#define BOATANIMATION_H

#include <QGraphicsItemAnimation>

class BoatGraphicsItem;
class TrackModel;
class BoatModel;

/**
    \class BoatAnimation

    \brief The animation Item for a scenario

    The class is the animation helper for a Scenario, according to the
    Graphics View Framework.

    It inherits QGraphicsItemAnimation and provides BoatGraphicsItem
    specific animation features, like position and heading given a
    time step.

    It also does a rate limiting update for drawing as Windows platforms
    suffer from that.

    \sa SituationScene, SituationModel
*/

typedef QPair<qreal, qreal> Pair;
typedef QList<Pair> PairList;


class BoatAnimation : public QGraphicsItemAnimation {
    public:
        BoatAnimation(TrackModel *track, BoatGraphicsItem *boat, int maxSize,  QGraphicsItemAnimation *parent = 0);
        ~BoatAnimation();
        BoatGraphicsItem *boat() const {return m_boat; }

        qreal headingAt(qreal step) const;
        qreal sailAt(qreal step) const;
        void setsailAt(qreal step, qreal sail) { m_sailList.append(Pair(step,sail)); }

    protected:
        virtual void afterAnimationStep(qreal step);

    private:

        qreal linearAngleForStep (PairList pairList, qreal step, qreal defaultValue = 0) const;

        /// \a m_rotationList holds the list of (step,heading) pair values
        PairList m_rotationList;

        /// \a m_sailList holds the list of (step,sail) pair values
        PairList m_sailList;

        /// \a m_track holds the reference to the track
        TrackModel *m_track;

        /// \a m_boats holds the List of boats hidden during animation
        QList<BoatModel*> m_boats;

        /// \a m_boat holds the reference to the animation BoatGraphicsItem
        BoatGraphicsItem *m_boat;

        /// \a m_maxSize holds the maximum track size of the scenario
        int m_maxSize;

        /// \a m_time holds the timer used for movement filtering
        QTime m_time;
};

#endif
