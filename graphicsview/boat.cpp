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

#include "commontypes.h"
#include "situationscene.h"
#include "situationmodel.h"
#include "trackmodel.h"
#include "boatmodel.h"

extern int debugLevel;

BoatGraphicsItem::BoatGraphicsItem(BoatModel *boat, QGraphicsItem *parent)
        : QGraphicsItem(parent),
        m_boat(boat),
        m_angle(boat->heading()),
        m_trim(boat->trim()),
        m_color(boat->track()->color()),
        m_series(boat->track()->series()),
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
    connect(boat, SIGNAL(trimChanged(qreal)),
            this, SLOT(setTrim(qreal)));
    connect(boat->track(), SIGNAL(colorChanged(QColor)),
            this, SLOT(setColor(QColor)));
    connect(boat->track(), SIGNAL(seriesChanged(Boats::Series)),
            this, SLOT(setSeries(Boats::Series)));
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

/// calculate a sail incidence angle, corrected with user trimming
void BoatGraphicsItem::setSailAngle() {
    qreal layline = m_boat->track()->situation()->laylineAngle();

    // within 10° inside layline angle, the sail is headed
    if (m_angle < layline-10) {
        m_sailAngle =  m_angle + m_trim;
        return;
    } else if (m_angle > 360 - (layline-10)) {
        m_sailAngle =  m_angle - m_trim;
        return;
    }


    switch (m_series) {
    // tornado has fixed 20° incidence
    case Boats::tornado:
        if (m_angle<180) {
            m_sailAngle = 20 + m_trim;
        } else {
            m_sailAngle = - 20  + m_trim;
        }
        break;
    default:
        // linear incidence variation
        // incidence is 15° at layline angle and 90° downwind
        qreal a = (180 - layline) / 75;
        qreal b = layline / a - 15;
        if (m_angle<180) {
            m_sailAngle = m_angle/a - b + m_trim;
        } else {
            m_sailAngle = m_angle/a - b - 180 - m_trim;
        }
        break;
    }

    if (debugLevel & 1 << VIEW) std::cout
            << "angle = " << m_angle
            << " trim = " << m_trim
            << " sail = " << m_sailAngle
            << " the = "  << fmod(m_angle - m_sailAngle + 360, 360)
            << std::endl;
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
        setZValue(m_order);
        update();
    }
}

void BoatGraphicsItem::setTrim(qreal value) {
    if (m_trim != value) {
        m_trim = value;
        setSailAngle();
        update();
    }
}

void BoatGraphicsItem::setColor(QColor value) {
    if (m_color != value) {
        m_color = value;
        update();
    }
}

void BoatGraphicsItem::setSeries(Boats::Series value) {
    if (m_series != value) {
        prepareGeometryChange();
        m_series = value;
        setSailAngle();
        update();
    }
}

void BoatGraphicsItem::deleteItem(BoatModel *boat) {
    if (boat == m_boat) {
        if (debugLevel & 1 << VIEW) std::cout << "deleting boatGraphics for model" << m_boat << std::endl;
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
    update();
}

void BoatGraphicsItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    Q_UNUSED(event);
}

void BoatGraphicsItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    Q_UNUSED(event);
}

QRectF BoatGraphicsItem::boundingRect() const {
    switch (m_series) {
    case Boats::keelboat:
        return QRectF(-67, -54, 133, 108);
    case Boats::laser:
        return QRectF(-27, -22, 54, 44);
    case Boats::optimist:
        return QRectF(-15, -12, 31, 25);
    case Boats::tornado:
        return QRectF(-40, -33, 81, 66);
    case Boats::startboat:
        return QRectF(-55, -55, 110, 110);
    default:
        return QRectF(-50, -50, 100, 100);
    }
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

    QPainterPath path;
    int numberSize = 0;
    qreal posY = 0;

    painter->rotate(m_angle);

    if (isSelected())
        painter->setPen(Qt::red);
    else
        painter->setPen(Qt::black);
    painter->setBrush(m_color);

    switch (m_series) {
    case Boats::keelboat:
        path.moveTo(0,-50);
        path.cubicTo(20, 0, 18, 13, 10, 48);
        path.lineTo(-10, 48);
        path.cubicTo(-18, 13, -20, 0, 0, -50);
        numberSize = 12;
        posY = 25;
        break;
    case Boats::laser:
        path.moveTo(0,-20);
        path.cubicTo(0.3, -19.7, 0.3, -20.0, 0.7, -19.7);
        path.cubicTo(3.3, -14.3, 6.7, -3.3, 6.7, 4.7);
        path.cubicTo(6.7, 11.0, 6.7, 14.3, 5.0, 20.0);
        path.lineTo(-5.0, 20.0);
        path.cubicTo(-6.7, 14.3, -6.7, 11.0, -6.7, 4.7);
        path.cubicTo(-6.7, -3.3, -3.3, -14.3, -0.7, -19.7);
        path.cubicTo(-0.3, -20.0, -0.3, -19.7, 0, -20);
        numberSize = 7;
        posY = 10;
        break;
    case Boats::optimist:
        path.moveTo(0,-11.5);
        path.cubicTo(1.5, -11.5, 1.7, -11.3, 2.9, -11.1);
        path.cubicTo(3.6, -9.4, 5.6, -4.0, 5.6, 1.5);
        path.cubicTo(5.6, 5.4, 5.0, 9.0, 4.6, 11.5);
        path.lineTo(-4.6, 11.5);
        path.cubicTo(-5.0, 9.0, -5.6, 5.4, -5.6, 1.5);
        path.cubicTo(-5.6, -4.0, -3.6, -9.4, -2.9, -11.1);
        path.cubicTo(-1.7, -11.3, -1.5, -11.5, 0, -11.5);
        numberSize = 6;
        posY = 5;
        break;
    case Boats::tornado:
        path.moveTo(0,0);
        path.lineTo(10.7, 0);
        path.cubicTo(11.2, -11.7, 12.2, -19.8, 13.2, -30.5);
        path.cubicTo(14.7, -20.3, 15.3, -6.1, 15.3, 6.1);
        path.cubicTo(15.3, 13.7, 14.7, 20.8, 14.7, 30.0);
        path.cubicTo(13.7, 30.5, 13.7, 30.5, 12.7, 30.5);
        path.cubicTo(12.2, 30.5, 12.2, 30.5, 10.7, 30.0);
        path.lineTo(10.7, 23.9);

        path.lineTo(-10.7, 23.9);
        path.lineTo(-10.7, 30.0);
        path.cubicTo(-12.2, 30.5, -12.2, 30.5, -12.7, 30.5);
        path.cubicTo(-13.7, 30.5, -13.7, 30.5, -14.7, 30);
        path.cubicTo(-14.7, 20.8, -15.3, 13.7, -15.3, 6.1);
        path.cubicTo(-15.3, -6.1, -14.7, -20.3, -13.2, -30.5);
        path.cubicTo(-12.2, -19.8, -11.2, -11.7, -10.7, 0);
        path.lineTo(0, 0);
        numberSize = 10;
        posY = 17;
        break;
    case Boats::startboat:
        path.moveTo(0,-50);
        path.cubicTo(30, -20, 20, 30, 17, 50);
        path.lineTo(-17, 50);
        path.cubicTo(-20, 30, -30, -20, 0, -50);
        path.addEllipse(-1, -10, 2, 2);
        break;
    default:
        break;
    }

    painter->drawPath(path);

    if (numberSize && m_order) {
        paintNumber(painter, numberSize, posY);
    }

    switch (m_series) {
    case Boats::keelboat:
        paintSail(painter, 41.5, QPointF(0,-8));
        break;
    case Boats::laser:
        paintSail(painter, 28.5, QPointF(0, -8.7));
        break;
    case Boats::optimist:
        paintSail(painter, 16.5, QPointF(0,-6.9));
        break;
    case Boats::tornado:
        paintSail(painter, 25.5, QPointF(0,0));
        break;
    default:
        break;
    }

}

void BoatGraphicsItem::paintNumber(QPainter *painter, int numberSize, qreal posY) {
    QPainterPath fpath;
    QFont f(painter->font());
    QString number = QString::number(m_order);
    f.setPointSize(numberSize);
    QFontMetrics fm(f);
    fpath.addText(-fm.width(number)/2.0, posY ,f,number);
    painter->fillPath(fpath, QBrush(Qt::black));
}

void BoatGraphicsItem::paintSail(QPainter *painter, qreal sailSize, QPointF attach) {
    painter->save();
    painter->translate(attach);
    QPainterPath sailPath;
    qreal angle = fmod(m_angle - m_sailAngle +360, 360);
    if (angle < 10 || angle > 350 || (angle > 170 && angle < 190)) {
        sailPath.cubicTo(.1 * sailSize, .2 * sailSize, .1 * sailSize, .2 * sailSize, 0, .3 * sailSize);
        sailPath.cubicTo(-.1 * sailSize, .4 * sailSize, -.1 * sailSize, .4 * sailSize, 0, .5 * sailSize);
        sailPath.cubicTo(.1 * sailSize, .6 * sailSize, .1 * sailSize, .6 * sailSize, 0, .7 * sailSize);
        sailPath.cubicTo(-.1 * sailSize, .8 * sailSize, -.1 * sailSize, .8 * sailSize, 0, sailSize);
        sailPath.lineTo(0, 0);
    } else if (angle<180) {
        sailPath.cubicTo(.1 * sailSize, .4 * sailSize, .1 * sailSize, .6 * sailSize, 0, sailSize);
        sailPath.lineTo(0, 0);
    } else {
        sailPath.cubicTo(-.1 * sailSize, .4 * sailSize, -.1 * sailSize, .6 * sailSize, 0, sailSize);
        sailPath.lineTo(0, 0);
    }
    painter->rotate(- m_sailAngle);
    painter->fillPath(sailPath, QBrush(Qt::white));
    painter->strokePath(sailPath, painter->pen());

    painter->restore();
}

int BoatGraphicsItem::type() const {
    return BOAT_TYPE;
}
