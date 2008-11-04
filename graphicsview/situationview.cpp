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

void SituationView::wheelEvent(QWheelEvent *event) {
    if (debugLevel & 1 << VIEW) std::cout << "wheel event " << event->delta() << std::endl;
    setScale(event->delta() > 0);
}

void SituationView::setScale(bool in) {
    if (in) {
        if (scaleValue < 3.0) {
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