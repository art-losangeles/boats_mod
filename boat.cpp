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

    setSailAngle();
    setPos(boat->position());
    setZValue(m_order);

    connect(boat, SIGNAL(headingChanged(qreal)),
            this, SLOT(setHeading(qreal)));
    connect(boat, SIGNAL(positionChanged(QPointF)),
            this, SLOT(setPosition(QPointF)));
    connect(boat, SIGNAL(orderChanged(int)),
            this, SLOT(setOrder(int)));
    connect(boat->track(), SIGNAL(colorChanged(QColor)),
            this, SLOT(setColor(QColor)));
    connect(boat->track()->situation(), SIGNAL(boatRemoved(BoatModel*)),
            this, SLOT(deleteItem(BoatModel*)));
}


BoatGraphicsItem::~BoatGraphicsItem() {}

void BoatGraphicsItem::setHeading(qreal value) {
    if (m_angle != value) {
        prepareGeometryChange();
        m_angle = value;
        setSailAngle();
        update();
    }
}

void BoatGraphicsItem::setSailAngle() {
    qreal layline = m_boat->track()->situation()->laylineAngle() -10;
    if (m_angle< layline || m_angle>360-layline) {
        m_sailAngle = -m_angle;
    } else if (m_angle<180) {
        m_sailAngle = -m_angle/1.86 + 6;
    } else {
        m_sailAngle = 180-m_angle/1.86 + 6;
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

void BoatGraphicsItem::setColor(QColor value) {
    if (m_color != value) {
        m_color = value;
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
    static_cast<SituationScene*>(scene())->setModelPressed(m_boat);
    bool multiSelect = (event->modifiers() & Qt::ControlModifier) != 0;
    if (!multiSelect) {
        scene()->clearSelection();
    }
    setSelected(true);
}

void BoatGraphicsItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    Q_UNUSED(event);
}

void BoatGraphicsItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    Q_UNUSED(event);
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
    Q_UNUSED(option);
    Q_UNUSED(widget);
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

    painter->translate(0,-10);
    QPainterPath sailPath(QPointF(0,0));
    qreal layline = m_boat->track()->situation()->laylineAngle() -10;
    if (m_angle< layline || m_angle>360-layline) {
        sailPath.lineTo(2,10);
        sailPath.lineTo(-2,20);
        sailPath.lineTo(2,30);
        sailPath.lineTo(-2,40);
        sailPath.lineTo(0,50);
    } else if (m_angle<180) {
        sailPath.cubicTo(5, 10, 5, 40, 0, 50);
    } else {
        sailPath.cubicTo(-5, 10, -5, 40, 0, 50);
    }
    painter->rotate(m_sailAngle);
    painter->strokePath(sailPath,painter->pen());
}

int BoatGraphicsItem::type() const {
    return BOAT_TYPE;
}
