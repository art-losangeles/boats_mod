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
        BoatGraphicsItem(BoatModel *boat = NULL);
        ~BoatGraphicsItem();

        BoatModel* boat() const { return m_boat; }

        QRectF boundingRect() const;
        QPainterPath shape() const;
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                   QWidget *widget);

    public slots:
        void setHeading(int value);
        void setPosition(QPointF position);
        void deleteItem();

    private:
        BoatModel *m_boat;
        qreal m_angle;
        QColor m_color;
        bool m_selected;
};

#endif
