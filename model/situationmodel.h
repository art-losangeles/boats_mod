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
#include <QUndoStack>

class TrackModel;
class BoatModel;

class SituationModel : public QObject {
        Q_OBJECT
    public:
        SituationModel(QObject *parent = 0);
        ~SituationModel();

        QUndoStack * undoStack() const { return m_undoStack;};
        int size() const { return m_tracks.size();};
        void addingBoat(BoatModel *boat) {emit boatAdded(boat);};
        void removingBoat(BoatModel *boat) {emit boatRemoved(boat);};

    signals:
        void trackAdded(TrackModel *track);
        void trackRemoved(TrackModel *track);
        void boatAdded(BoatModel *boat);
        void boatRemoved(BoatModel *boat);

    public slots:
        void addTrack(TrackModel *track);
        void deleteTrack(TrackModel *track);

    private:
        QUndoStack *m_undoStack;
        QList<TrackModel*> m_tracks;
};

#endif
