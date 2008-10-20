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

#include "commontypes.h"

class TrackModel;
class BoatModel;
class MarkModel;

class SituationModel : public QObject {
        Q_OBJECT
    public:
        SituationModel(QObject *parent = 0);
        ~SituationModel();

        QUndoStack * undoStack() const { return m_undoStack;};
        QList<QString> seriesNames() {return m_seriesNames; };

        Series situationSeries() const { return m_situationSeries; };
        void setSituationSeries(const int theValue, bool update = false);

        QString title() const { return m_title; };

        QString rules() const { return m_rules; };

        QString abstract() const { return m_abstract; };

        QString description() const { return m_description; };

        int size() const { return m_tracks.size();};
        const QList<TrackModel*> tracks() const { return m_tracks; };

        int markSize() const { return m_marks.size();};
        const QList<MarkModel*> marks() const { return m_marks; };

        qreal laylineAngle() const { return m_laylineAngle; };
        void setLaylineAngle(const int theValue, bool update = false);

        QStringList discardedXml() const { return m_discardedXml; };
        void appendDiscardedXml(const QString& theValue);

        QString fileName() const { return m_fileName; };
        void setFileName(const QString theValue) {m_fileName = theValue; };

        void addingBoat(BoatModel *boat) {emit boatAdded(boat);};
        void removingBoat(BoatModel *boat) {emit boatRemoved(boat);};

    signals:
        void trackAdded(TrackModel *track);
        void trackRemoved(TrackModel *track);
        void boatAdded(BoatModel *boat);
        void boatRemoved(BoatModel *boat);
        void laylineChanged(const int angle);
        void seriesChanged(const int series);
        void markAdded(MarkModel *mark);
        void markRemoved(MarkModel *mark);

    public slots:
        void setTitle(const QString theValue) { m_title = theValue; };
        void setRules(const QString theValue) { m_rules = theValue; };
        void setAbstract(const QString theValue) { m_abstract = theValue; };
        void setDescription(const QString theValue) { m_description = theValue; };
        void addTrack(TrackModel *track);
        void deleteTrack(TrackModel *track);
        void addMark(MarkModel *mark, int order = 0);
        int deleteMark(MarkModel *mark);

    private:
        QUndoStack *m_undoStack;
        QList<QString> m_seriesNames;
        QString m_fileName;
        QString m_title;
        QString m_rules;
        QString m_abstract;
        QString m_description;
        int m_laylineAngle;
        Series m_situationSeries;
        QList<TrackModel*> m_tracks;
        QList<MarkModel*> m_marks;
        QStringList m_discardedXml;

};

#endif
