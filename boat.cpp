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

#include "situationscene.h"
#include "model/situationmodel.h"
#include "model/trackmodel.h"
#include "model/boatmodel.h"

BoatGraphicsItem::BoatGraphicsItem(BoatModel *boat, QGraphicsItem *parent)
        : QGraphicsItem(parent),
        m_boat(boat),
        m_angle(boat->heading()),
        m_color(boat->track()->color()),
        m_selected(false),
        m_order(boat->order()) {
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);

    setBoundingRegionGranularity(1);

    setPos(boat->position());
    setZValue(m_order);

    connect(boat, SIGNAL(headingChanged(qreal)),
            this, SLOT(setHeading(qreal)));
    connect(boat, SIGNAL(positionChanged(QPointF)),
            this, SLOT(setPosition(QPointF)));
    connect(boat, SIGNAL(orderChanged(int)),
            this, SLOT(setOrder(int)));
    connect(boat->track()->situation(), SIGNAL(boatRemoved(BoatModel*)),
            this, SLOT(deleteItem(BoatModel*)));
}


BoatGraphicsItem::~BoatGraphicsItem() {}

void BoatGraphicsItem::setHeading(qreal value) {
    if (m_angle != value) {
        prepareGeometryChange();
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

void BoatGraphicsItem::setOrder(int value) {
    if (m_order != value) {
        m_order = value;
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

void BoatGraphicsItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    // propagate mouse event first for selected items
    QGraphicsItem::mousePressEvent(event);

    static_cast<SituationScene*>(scene())->setModelPressed(m_boat);
    bool multiSelect = (event->modifiers() & Qt::ControlModifier) != 0;
    if (!multiSelect) {
        scene()->clearSelection();
    }
    setSelected(true);
}

QRectF BoatGraphicsItem::boundingRect() const {
    return QRectF(-60, -60, 120, 120);
}

QPainterPath BoatGraphicsItem::shape() const {
    QPainterPath path;
    path.addRegion(boundingRegion(QTransform()));
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
    QPainterPath path(QPointF(0, -60));
    path.cubicTo(24, 0, 21, 15, 12, 60);
    path.lineTo(-12, 60);
    path.cubicTo(-21, 15, -24, 0, 0, -60);
    path.addText(-5,25,painter->font(),QString::number(m_order));
    painter->drawPath(path);

    //previous try
    //painter->setPen(Qt::blue);
    //QPainterPath oldpath(QPointF(0, -20));
    //oldpath.cubicTo(7, 0, 7, 5, 4, 20);
    //oldpath.lineTo(-4, 20);
    //oldpath.cubicTo(-7, 5, -7, 0, 0, -20);
    //painter->drawPath(oldpath);

}
