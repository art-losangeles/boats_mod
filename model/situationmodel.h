//
// C++ Interface: situationmodel
//
// Description:
//
//
// Author: Thibaut GRIDEL <tgridel@free.fr>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef SITUATIONMODEL_H
#define SITUATIONMODEL_H

#include <QObject>

class TrackModel;

class SituationModel : public QObject {
        Q_OBJECT
    public:
        SituationModel(QObject *parent = 0);
        ~SituationModel() {}

        int size() const { return m_tracks.size();};

    signals:
        void trackAdded(TrackModel *track);
        void trackRemoved(TrackModel *track);

    public slots:
        TrackModel * addTrack();
        void deleteTrack(TrackModel *track);

    private:

        QList<TrackModel*> m_tracks;
};

#endif
