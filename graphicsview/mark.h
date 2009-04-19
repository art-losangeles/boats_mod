//
// C++ Interface: MarkGraphicsItem
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
#ifndef MARK_H
#define MARK_H

#include <QGraphicsItem>
#include <QObject>
#include <QColor>

#include "commontypes.h"

class MarkModel;

/**
    \class MarkGraphicsItem

    \brief the QGraphicsItem for a mark

    The class represents the Item according to the Graphics View Framework.

    It inherits QGraphicsItem for Item framework and QObject for slot and
    signal framework. It displays the representation of a MarkModel on a
    SituationView.

    \sa SituationView, SituationScene, SituationModel, MarkModel

*/

class MarkGraphicsItem : public QObject, public QGraphicsItem {
        Q_OBJECT
    public:
        MarkGraphicsItem(MarkModel *mark = 0, QGraphicsItem *parent = 0);
        ~MarkGraphicsItem();

        MarkModel* mark() const { return m_mark; }

        QRectF boundingRect() const;
        QPainterPath shape() const;
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                   QWidget *widget);
        int type() const;

    public slots:
        void setPosition(QPointF position);
        void setOrder(int value);
        void setColor(QColor value);
        void setZone(bool zone);
        void setLength(int length);
        void setSeries(int value);
        void deleteItem(MarkModel *mark);

    private:
        /// \a m_mark holds the MarkModel being represented
        MarkModel *m_mark;

        /// \a m_color holds the color of the mark
        QColor m_color;

        /// \a m_zone holds wether zone is displayed
        bool m_zone;

        /// \a m_length holds the number of boat lengths for zone
        int m_length;

        /// \a m_boatLength holds the size in scene coordinates for main series
        int m_boatLength;

        /// \a m_selected holds selection information
        bool m_selected;

        /// \a m_order holds the stacking order in the mark list
        int m_order;
};

#endif
