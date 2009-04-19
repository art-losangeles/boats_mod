//
// C++ Interface: markmodel
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
#ifndef MARKMODEL_H
#define MARKMODEL_H

#include <QtGui>

#include "positionmodel.h"

class SituationModel;

/**
    \class MarkModel

    \brief The Model for a Mark

    The class represents the Model for a Mark, according to an
    Observer Pattern.

    MarkModel inherits PositionModel and contains data which represents
    a mark, like its color, and whether the zone is displayed.

    \sa SituationModel

*/

class MarkModel : public PositionModel {
        Q_OBJECT
    public:
        MarkModel(SituationModel *situation, QObject *parent = 0);
        ~MarkModel();

        // Setters and Getters for Model Data
        QColor color() const { return m_color; }
        void setColor(const QColor& theValue);

        bool zone() const { return m_zone; }
        void setZone(const bool theValue);

        // Setters and Getters for Non model Data
        SituationModel* situation() const { return m_situation; }

        int length() const { return m_length; }
        void setLength(const int theValue);

    signals:
        void colorChanged(QColor color);
        void zoneChanged(bool zone);
        void lengthChanged(int length);

    private:
        // Model Data
        /// \a m_color holds the color of the Mark
        QColor m_color;

        /// \a m_zone holds whether the zone of the Mark is displayed
        bool m_zone;

        // Non model Data
        /// \a m_situation keeps a pointer to the SituationModel to which
        /// it belongs
        SituationModel *m_situation;

        /// \a m_length keeps the length of Main Series of the Scenario
        int m_length;
};

#endif
