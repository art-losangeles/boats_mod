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
        void setColor(QColor value);
        void setSeries(Boats::Series value);
        void deleteItem(BoatModel *boat);

    protected:
        void mousePressEvent(QGraphicsSceneMouseEvent *event);
        void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
        void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

    private:
        void setSailAngle();

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
