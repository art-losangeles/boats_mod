//
// C++ Implementation: situationview
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

#include "commontypes.h"

#include "situationview.h"

extern int debugLevel;

SituationView::SituationView(QWidget *parent)
    :QGraphicsView(parent) {
}

SituationView::SituationView(QGraphicsScene *scene, QWidget *parent)
    : QGraphicsView(scene, parent),
    scaleValue(1) {
}

SituationView::~SituationView() {
}

QPixmap SituationView::screenShot() {
    QPixmap pixmap(size());
    pixmap.fill(Qt::white);
    QPainter painter(&pixmap);
    painter.setRenderHints( QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    render(&painter);
    return pixmap;
}

void SituationView::wheelEvent(QWheelEvent *event) {
    if (debugLevel & 1 << VIEW) std::cout << "wheel event " << event->delta() << std::endl;
    setScale(event->delta() > 0);
}

void SituationView::zoomIn() {
    setScale(true);
}

void SituationView::zoomOut() {
    setScale(false);
}

void SituationView::zoomFit() {
    QMatrix old = matrix();
    fitInView(scene()->itemsBoundingRect(),Qt::KeepAspectRatio);
    qreal s = matrix().m11();
    if (s < 10.0 && s > 0.1) {
        // adopt scaling
        scaleValue = (round(s * 20)) / 20.0;
    }
    // apply calculated or default scale
    QMatrix m(scaleValue, old.m21(), old.m21(), scaleValue, old.dx(), old.dy());
    setMatrix(m);
}

void SituationView::setScale(bool in) {
    if (in) {
        if (scaleValue < 10.0) {
            scaleValue += .05;
        }
    } else if (scaleValue > 0.1) {
        scaleValue -= .05;
    }
    QMatrix old = matrix();
    QMatrix m(scaleValue, old.m21(), old.m21(), scaleValue, old.dx(), old.dy());
    setMatrix(m);
    if (debugLevel & 1 << VIEW) std::cout << "new scale values "
        << " horizontal: " << m.m11()
        << " vertical: " << m.m22() << std::endl;
}
