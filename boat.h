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

class BoatModel;

class BoatGraphicsItem : public QObject, public QGraphicsItem {
        Q_OBJECT
    public:
        BoatGraphicsItem(BoatModel *boat = 0, QGraphicsItem *parent = 0);
        ~BoatGraphicsItem();

        BoatModel* boat() const { return m_boat; }

        QRectF boundingRect() const;
        QPainterPath shape() const;
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                   QWidget *widget);

    public slots:
        void setHeading(qreal value);
        void setPosition(QPointF position);
        void setOrder(int value);
        void deleteItem(BoatModel *boat);

    protected:
        void mousePressEvent(QGraphicsSceneMouseEvent *event);

    private:
        BoatModel *m_boat;
        qreal m_angle;
        QColor m_color;
        bool m_selected;
        int m_order;
};

#endif
