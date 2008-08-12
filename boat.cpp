//
// C++ Implementation: BoatGraphicsItem
//
// Description:
//
//
// Author: Thibaut GRIDEL <tgridel@free.fr>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include <iostream>

#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>

#include "boat.h"

#include "model/situationmodel.h"
#include "model/trackmodel.h"
#include "model/boatmodel.h"

BoatGraphicsItem::BoatGraphicsItem(BoatModel *boat)
        : m_boat(boat),
        m_angle(boat->heading()),
        m_color(boat->track()->color()),
        m_selected(false) {
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);

    setPos(boat->position());

    connect(boat, SIGNAL(headingChanged(int)),
            this, SLOT(setHeading(int)));
    connect(boat, SIGNAL(positionChanged(QPointF)),
            this, SLOT(setPosition(QPointF)));
    connect(boat->track()->situation(), SIGNAL(boatRemoved(BoatModel*)),
            this, SLOT(deleteItem(BoatModel*)));
}


BoatGraphicsItem::~BoatGraphicsItem() {}

void BoatGraphicsItem::setHeading(int value) {
    if (m_angle != value) {
        m_angle = value;
        update();
    }
}

void BoatGraphicsItem::setPosition(QPointF position) {
    if (pos() != position) {
        setPos(position);
        update();
    }
}

void BoatGraphicsItem::deleteItem(BoatModel *boat) {
    if (boat == m_boat) {
        std::cout << "deleting boatGraphics for model" << m_boat << std::endl;
        scene()->removeItem(this);
        delete this;
    }
}

QRectF BoatGraphicsItem::boundingRect() const {
    return QRectF(-20, -20, 40, 40);
}

QPainterPath BoatGraphicsItem::shape() const {

    QPainterPath path(QPointF(0, -20));
    path.cubicTo(8, 0, 7, 5, 4, 20);
    path.lineTo(-4, 20);
    path.cubicTo(-7, 5, -8, 0, 0, -20);
    return path;
}

void BoatGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,

                             QWidget *widget) {
    if (isSelected())
        painter->setPen(Qt::red);
    else
        painter->setPen(Qt::black);

    painter->setBrush(m_color);
    painter->rotate(m_angle);

    //painter->setBrush(Qt::NoBrush);
    QPainterPath path(QPointF(0, -20));
    path.cubicTo(8, 0, 7, 5, 4, 20);
    path.lineTo(-4, 20);
    path.cubicTo(-7, 5, -8, 0, 0, -20);
    painter->drawPath(path);

    //previous try
    //painter->setPen(Qt::blue);
    //QPainterPath oldpath(QPointF(0, -20));
    //oldpath.cubicTo(7, 0, 7, 5, 4, 20);
    //oldpath.lineTo(-4, 20);
    //oldpath.cubicTo(-7, 5, -7, 0, 0, -20);
    //painter->drawPath(oldpath);

}
