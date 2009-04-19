//
// C++ Interface: situationview
//
// Description:
//
//
// Author: Thibaut GRIDEL <tgridel@free.fr>
//
// Copyright (c) 2008-2009 Thibaut GRIDEL
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
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
