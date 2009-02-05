//
// C++ Interface: tracktablemodel
//
// Description:
//
//
// Author: Thibaut GRIDEL <tgridel@free.fr>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef TRACKTABLEMODEL_H
#define TRACKTABLEMODEL_H

#include <QtGui>

#include "commontypes.h"

#include "model/situationmodel.h"

class TrackTableModel : public QAbstractTableModel {
        Q_OBJECT
    public:
        TrackTableModel(SituationModel* situation = 0, QObject *parent = 0);
        ~TrackTableModel();

        void setSituation(SituationModel *situation) { m_situation = situation; };
        int rowCount(const QModelIndex &parent) const { Q_UNUSED(parent); return m_situation->size(); };
        int columnCount(const QModelIndex &parent) const { Q_UNUSED(parent); return 2;};
        QVariant data(const QModelIndex &index, int role) const;
        QVariant headerData(int section, Qt::Orientation orientation, int role) const;
        Qt::ItemFlags flags(const QModelIndex &index) const;
        bool setData(const QModelIndex &index, const QVariant &value, int role=Qt::EditRole);

    public slots:
        void addTrack(TrackModel *track);
        void updateTrack();
        void deleteTrack(TrackModel *track);

    private:
        SituationModel *m_situation;
};

#endif
