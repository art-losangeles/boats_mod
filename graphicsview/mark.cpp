//
// C++ Implementation: MarkGraphicsItem
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
#include <QGraphicsScene>

#include "mark.h"

#include "commontypes.h"
#include "situationscene.h"
#include "model/situationmodel.h"
#include "model/markmodel.h"

extern int debugLevel;

MarkGraphicsItem::MarkGraphicsItem(MarkModel *mark, QGraphicsItem *parent)
        : QGraphicsItem(parent),
        m_mark(mark),
        m_color(mark->color()),
        m_zone(mark->zone()),
        m_selected(false),
        m_order(mark->order()) {
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);

    setPos(mark->position());
    setZValue(m_order);

    connect(mark, SIGNAL(positionChanged(QPointF)),
            this, SLOT(setPosition(QPointF)));
    connect(mark, SIGNAL(orderChanged(int)),
            this, SLOT(setOrder(int)));
    connect(mark, SIGNAL(colorChanged(QColor)),
            this, SLOT(setColor(QColor)));
    connect(mark, SIGNAL(zoneChanged(bool)),
            this, SLOT(setZone(bool)));
    connect(mark->situation(), SIGNAL(markRemoved(MarkModel*)),
            this, SLOT(deleteItem(MarkModel*)));
}


MarkGraphicsItem::~MarkGraphicsItem() {}

void MarkGraphicsItem::setPosition(QPointF position) {
    if (pos() != position) {
        setPos(position);
        update();
    }
}

void MarkGraphicsItem::setOrder(int value) {
    if (m_order != value) {
        m_order = value;
        update();
    }
}

void MarkGraphicsItem::setColor(QColor value) {
    if (m_color != value) {
        m_color = value;
        update();
    }
}

void MarkGraphicsItem::setZone(bool value) {
    if (m_zone != value) {
        m_zone = value;
        update();
    }
}

void MarkGraphicsItem::deleteItem(MarkModel *mark) {
    if (mark == m_mark) {
        if (debugLevel & 1 << VIEW) std::cout << "deleting markGraphics for model" << m_mark << std::endl;
        scene()->removeItem(this);
        delete this;
    }
}

QRectF MarkGraphicsItem::boundingRect() const {
    return QRectF(-240, -240, 480, 480);
}

QPainterPath MarkGraphicsItem::shape() const {
    QPainterPath path;
    path.addEllipse(QPointF(0,0),10,10);
    return path;
}

void MarkGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,

                             QWidget *widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);
    if (isSelected())
        painter->setPen(Qt::red);
    else
        painter->setPen(Qt::black);

    painter->setBrush(m_color);
    QPointF point(0, 0);
    painter->drawEllipse(point,10,10);
    painter->drawText(QRectF(-10,-10,20,20),Qt::AlignCenter,QString::number(m_order));
    if (m_zone) {
        painter->setBrush(Qt::NoBrush);
        painter->setPen(Qt::DashLine);
        painter->drawEllipse(point,240,240);
    }
}

int MarkGraphicsItem::type() const {
    return MARK_TYPE;
}
