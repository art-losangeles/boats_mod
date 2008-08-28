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

#include <QtGui>

class TrackModel;
class BoatModel;
class MarkModel;

class SituationModel : public QObject {
        Q_OBJECT
    friend class XmlSituationReader;
    friend class XmlSituationWriter;
    public:
        SituationModel(QObject *parent = 0);
        ~SituationModel();

        QUndoStack * undoStack() const { return m_undoStack;};
        int size() const { return m_tracks.size();};
        int markSize() const { return m_marks.size();};
        qreal laylineAngle() const { return m_laylineAngle; };
        void setLaylineAngle(const qreal, bool update = false);
        QStringList discardedXml() const { return m_discardedXml; };
        void appendDiscardedXml(const QString& theValue);
        void addingBoat(BoatModel *boat) {emit boatAdded(boat);};
        void removingBoat(BoatModel *boat) {emit boatRemoved(boat);};

    signals:
        void trackAdded(TrackModel *track);
        void trackRemoved(TrackModel *track);
        void boatAdded(BoatModel *boat);
        void boatRemoved(BoatModel *boat);
        void laylineChanged(const qreal angle);
        void markAdded(MarkModel *mark);
        void markRemoved(MarkModel *mark);

    public slots:
        void addTrack(TrackModel *track);
        void deleteTrack(TrackModel *track);
        void addMark(MarkModel *mark);
        void deleteMark(MarkModel *mark);

    protected:
        QList<TrackModel*> m_tracks;

    private:
        QUndoStack *m_undoStack;
        qreal m_laylineAngle;
        QList<MarkModel*> m_marks;
        QStringList m_discardedXml;

};

#endif
