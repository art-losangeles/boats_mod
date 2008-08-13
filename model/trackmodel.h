//
// C++ Interface: trackmodel
//
// Description:
//
//
// Author: Thibaut GRIDEL <tgridel@free.fr>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef TRACKMODEL_H
#define TRACKMODEL_H

#include <QObject>
#include <QColor>

class BoatModel;
class SituationModel;

class TrackModel : public QObject {
        Q_OBJECT
    public:
        TrackModel(SituationModel* situation = 0, QObject *parent = 0);
        ~TrackModel();

        BoatModel * addBoat(BoatModel *boat, int order = 0);
        int deleteBoat(BoatModel *boat);

        void displayBoats();
        void hideBoats();

        SituationModel* situation() const { return m_situation; }
        QColor color() const { return m_color;};
        int size() const { return m_boats.size();};

    private:
        SituationModel *m_situation;
        QList<BoatModel*> m_boats;
        QColor m_color;
};

#endif
