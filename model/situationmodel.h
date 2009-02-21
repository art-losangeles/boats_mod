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

#include "boats.h"

class TrackModel;
class BoatModel;
class MarkModel;

/**
    \class SituationModel

    \brief The main Model for a scenario

    This Class represents the Model for a Scenario, according to an
    Observer Pattern.

    It holds the actual data for the scenario and signals whoever is
    interested in changes to the model.

    There are mainly:
    - description attributes and technical data which define the scenario
    - a List of TrackModel, which in turn contains a List of BoatModel
    - a List of MarkModel

    \sa SituationModel, TrackModel, BoatModel, MarkModel
*/

class SituationModel : public QObject {
        Q_OBJECT
    public:
        SituationModel(QObject *parent = 0);
        ~SituationModel();

        int sizeForSeries(const Boats::Series series);

        // Setters and Getters for Model Data
        QString title() const { return m_title; }
        void setTitle(const QString theValue);

        QString rules() const { return m_rules; }
        void setRules(const QString theValue);

        bool showLayline() const { return m_showLayline; }
        void setShowLayline(const bool theValue);

        int laylineAngle() const { return m_laylineAngle; }
        void setLaylineAngle(const int theValue);

        Boats::Series situationSeries() const { return m_situationSeries; }
        void setSituationSeries(const int theValue);

        int situationLength() const { return m_situationLength; }
        void setSituationLength(const int theValue);

        QString abstract() const { return m_abstract; }
        void setAbstract(const QString theValue);

        QString description() const { return m_description; }
        void setDescription(const QString theValue);

        int size() const { return m_tracks.size();}
        const QList<TrackModel*> tracks() const { return m_tracks; }

        int markSize() const { return m_marks.size();}
        const QList<MarkModel*> marks() const { return m_marks; }

        // Setters and Getters for Non model Data
        QUndoStack * undoStack() const { return m_undoStack;}

        QStringList discardedXml() const { return m_discardedXml; }
        void appendDiscardedXml(const QString& theValue);

        QString fileName() const { return m_fileName; }
        void setFileName(const QString theValue) {m_fileName = theValue; }

        // Helper to remotely trigger boat signals from elsewhere
        void addingBoat(BoatModel *boat) {emit boatAdded(boat);}
        void removingBoat(BoatModel *boat) {emit boatRemoved(boat);}

    signals:
        // Signals for Track
        void trackAdded(TrackModel *track);
        void trackRemoved(TrackModel *track);

        // Signals for Boat
        void boatAdded(BoatModel *boat);
        void boatRemoved(BoatModel *boat);

        // Signals for Scenario Parameters
        void titleChanged(const QString title);
        void rulesChanged(const QString rules);
        void showLaylineChanged(const bool show);
        void laylineChanged(const int angle);
        void seriesChanged(const int series);
        void lengthChanged(const int length);
        void abstractChanged(const QString abstract);
        void descriptionChanged(const QString description);

        // Signals for Marks
        void markAdded(MarkModel *mark);
        void markRemoved(MarkModel *mark);

    public slots:
        // Slots for Tracks
        void addTrack(TrackModel *track, int order = 0);
        void deleteTrack(TrackModel *track);

        // Slots for Marks
        void addMark(MarkModel *mark, int order = 0);
        int deleteMark(MarkModel *mark);

    private:
        // Model Data
        /// \a m_title holds the Title of the Scenario
        QString m_title;

        /// \a m_rules holds the Rules of the Scenario
        QString m_rules;

        /// \a m_abstract holds the Abstract of the Scenario
        QString m_abstract;

        /// \a m_description holds the Description of the Scenario
        QString m_description;

        /// \a m_showLayline holds whether the Laylines will be displayed
        bool m_showLayline;

        /// \a m_laylineAngle holds the Layline Angle of the Scenario
        int m_laylineAngle;

        /// \a m_situationSeries holds the Main Series of the Scenario
        Boats::Series m_situationSeries;

        /// \a m_situationLength holds the size of the Zone at Marks of
        /// the Scenario
        int m_situationLength;

        /// \a m_tracks holds the List of Tracks of the Scenario
        QList<TrackModel*> m_tracks;

        /// \a m_marks holds the List of Marks of the Scenario
        QList<MarkModel*> m_marks;


        // Non model Data
        /// \a m_undoStack maintains the Undo Stack for the Scenario
        QUndoStack *m_undoStack;

        /// \a m_discardedXml keeps all unparsed xml tags
        QStringList m_discardedXml;

        /// \a m_fileName holds the name of the file on disk
        QString m_fileName;
};

#endif
