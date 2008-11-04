//
// C++ Interface: situationview
//
// Description:
//
//
// Author: Thibaut GRIDEL <tgridel@free.fr>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef SITUATIONVIEW_H
#define SITUATIONVIEW_H

#include <QtGui>

class SituationView : public QGraphicsView {

    public:
        SituationView(QWidget *parent = 0);
        SituationView(QGraphicsScene *scene, QWidget *parent = 0);
        ~SituationView();

    protected:
        void wheelEvent(QWheelEvent *event);

    private:
        void setScale(bool in);
        qreal scaleValue;
};

#endif
