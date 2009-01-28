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

/**
    \class SituationView

    \brief The View for a scenario

    The class represents the View for a Scenario, according to the
    Graphics View Framework.

    It inherits QGraphicsView and manages zoom and screenshot capture.

    \sa SituationScene, SituationModel
*/

class SituationView : public QGraphicsView {
    Q_OBJECT
    public:
        SituationView(QWidget *parent = 0);
        SituationView(QGraphicsScene *scene, QWidget *parent = 0);
        ~SituationView();
        QPixmap screenShot();

    protected:
        void wheelEvent(QWheelEvent *event);

    public slots:
        void zoomIn();
        void zoomOut();
        void zoomFit();

    private:
        void setScale(bool in);

        /// \a scaleValue holds the value for the viewing scale
        qreal scaleValue;
};

#endif
