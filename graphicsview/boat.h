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
#ifndef BOAT_H
#define BOAT_H

#include <QGraphicsItem>
#include <QObject>
#include <QColor>

#include "boats.h"

class BoatModel;

/**
    \class BoatGraphicsItem

    \brief the QGraphicsItem for a boat

    The class represents the Item according to the Graphics View Framework.

    It inherits QGraphicsItem for Item framework and QObject for slot and
    signal framework. It displays the representation of a BoatModel on a
    SituationView.

    \sa SituationView, SituationScene, SituationModel, BoatModel

*/

class BoatGraphicsItem : public QObject, public QGraphicsItem {
        Q_OBJECT
    public:
        BoatGraphicsItem(BoatModel *boat, QGraphicsItem *parent = 0);
        ~BoatGraphicsItem();

        BoatModel* boat() const { return m_boat; }

        QRectF boundingRect() const;
        QPainterPath shape() const;
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                   QWidget *widget);
        int type() const;

    public slots:
        void setHeading(qreal value);
        void setPosition(QPointF position);
        void setOrder(int value);
        void setTrim(qreal value);
        void setOverlap(Boats::Overlaps value);
        void setColor(QColor value);
        void setSeries(Boats::Series value);
        void deleteItem(BoatModel *boat);

    protected:
        void mousePressEvent(QGraphicsSceneMouseEvent *event);
        void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
        void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

    private:
        void setSailAngle();
        void setOverlapLine();

        /// draw a number with font size numberSize at posY alongside boat
        void paintNumber(QPainter *painter, int numberSize, qreal posY);

        /// draw a sail with size sailScale from point attach
        void paintSail(QPainter *painter, qreal sailSize, QPointF attach);

        /// \a m_boat holds the BoatModel being represented
        BoatModel *m_boat;

        /// \a m_angle holds the heading of the boat
        qreal m_angle;

        /// \a m_sailAngle holds the ideal sail trimming angle
        qreal m_sailAngle;

        /// \a m_trim holds the manual trimming angle override
        qreal m_trim;

        /// \a m_overlap holds whether an overlap line should be displayed
        Boats::Overlaps m_overlap;
        QGraphicsLineItem *m_overlapLine;

        /// \a m_color holds the color of the TrackModel
        QColor m_color;

        /// \a m_series holds the series of the TrackModel
        Boats::Series m_series;

        /// \a m_selected holds selection information
        bool m_selected;

        /// \a m_order holds the stacking order in the TrackModel
        int m_order;
};

#endif
