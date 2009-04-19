//
// C++ Interface: positionmodel
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
#ifndef POSITIONMODEL_H
#define POSITIONMODEL_H

#include <QtGui>

/**
    \class PositionModel

    \brief The Model for a Position

    The class represents the Model for a Position, according to an
    Observer Pattern.

    PositionModel contains data which represents an object with a position,
    and a stacking order. It is inherited by BoatModel and TrackModel.

    \sa SituationModel, BoatModel, TrackModel

*/

class PositionModel : public QObject {
        Q_OBJECT
    public:
        PositionModel(QObject *parent = 0);
        virtual ~PositionModel();

        // Setters and Getters for Model Data
        QPointF position() const { return m_position; }
        virtual void setPosition(const QPointF& theValue);

        int order() const { return m_order; }
        void setOrder(const int theValue);

        // Setters and Getters for Non model Data
        QStringList discardedXml() const { return m_discardedXml; }
        void appendDiscardedXml(const QString& theValue);

    signals:
        void positionChanged(QPointF position);
        void orderChanged(int order);

    private:
        // Model Data
        /// \a m_position holds the position of the object
        QPointF m_position;

        /// \a m_order holds the stacking order of the object
        int m_order;

        // Non model Data
        /// \a m_discardedXml keeps all unparsed xml tags
        QStringList m_discardedXml;
};

#endif
