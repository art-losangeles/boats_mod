//
// C++ Interface: trackmodel
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
#ifndef TRACKMODEL_H
#define TRACKMODEL_H

#include <QtGui>

#include "boats.h"

class SituationModel;
class BoatModel;

/**
    \class TrackModel

    \brief The Model for the Track of a Boat

    The class represents the Model for a Track, according to an
    Observer Pattern.

    TrackModel contains data which describe one boat, like the series of
    the boat, the color it is drawn and the List of Positions where the boat
    navigates.

    It shall not be mistaken with BoatModel, which holds one position at
    a given time. More exactly, a TrackModel will hold a List of BoatModels.

    \sa SituationModel, BoatModel

*/

class TrackModel : public QObject {
        Q_OBJECT
    public:
        TrackModel(SituationModel* situation = 0, QObject *parent = 0);
        ~TrackModel();

        BoatModel * addBoat(BoatModel *boat, int order = 0);
        int deleteBoat(BoatModel *boat);

        void displayBoats();
        void hideBoats();

        // Setters and Getters for Model Data
        int order() const { return m_order; }
        void setOrder(const int theValue);

        QColor color() const { return m_color;}
        void setColor(const QColor& theValue);

        Boats::Series series() const { return m_series;}
        void setSeries(const Boats::Series theValue);

        int size() const { return m_boats.size();}
        const QList<BoatModel*> boats() const { return m_boats; }

        const QPainterPath path() const { return m_path; }

        // Setters and Getters for Non model Data
        SituationModel* situation() const { return m_situation; }

        int length() const { return m_length; }

        QStringList discardedXml() const { return m_discardedXml; }
        void appendDiscardedXml(const QString& theValue);

        void changingTrack(TrackModel *track);

    signals:
        // Signals for TrackModel parameters
        void orderChanged(int order);
        void colorChanged(QColor color);
        void seriesChanged(Boats::Series series);
        void trackChanged(TrackModel *track);

    private:
        // Model Data
        /// \a m_order holds the stacking order of the Track
        int m_order;

        /// \a m_color holds the color of the Track
        QColor m_color;

        /// \a m_series holds the series of the Track
        Boats::Series m_series;

        /// \a m_boats holds the List of Boat Positions of the Track
        QList<BoatModel*> m_boats;

        // Non model Data
        /// \a m_situation keeps a pointer to the SituationModel to which
        /// it belongs
        SituationModel *m_situation;

        /// \a m_length holds the size of the Boat in World Coordinates
        int m_length;

        /// \a m_path holds the QPainterPath of the Track
        QPainterPath m_path;

        /// \a m_discardedXml keeps all unparsed xml tags
        QStringList m_discardedXml;
};

#endif
