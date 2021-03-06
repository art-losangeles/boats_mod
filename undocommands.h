//
// C++ Interface: undocommands
//
// Description:
//
//
// Author: Thibaut GRIDEL <tgridel@free.fr>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef UNDOCOMMANDS_H
#define UNDOCOMMANDS_H

#include <QUndoCommand>
#include <QPointF>

class SituationModel;
class TrackModel;
class PositionModel;
class BoatModel;
class MarkModel;

enum {
    SET_TITLE,
    SET_RULES,
    SET_LAYLINE,
    SET_SERIES,
    SET_ABSTRACT,
    SET_DESCRIPTION,
    MOVE_MODEL,
    HEADING_BOAT,
    ZONE_MARK
};

class SetTitleUndoCommand : public QUndoCommand {

    public:
        SetTitleUndoCommand(SituationModel* situation, QString title, QUndoCommand *parent = 0);
        ~SetTitleUndoCommand();
        void undo();
        void redo();
        bool mergeWith(const QUndoCommand *command);
        int id() const { return SET_TITLE; }

    private:
        SituationModel *m_situation;
        QString m_oldTitle;
        QString m_newTitle;
};

class SetRulesUndoCommand : public QUndoCommand {

    public:
        SetRulesUndoCommand(SituationModel* situation, QString rules, QUndoCommand *parent = 0);
        ~SetRulesUndoCommand();
        void undo();
        void redo();
        bool mergeWith(const QUndoCommand *command);
        int id() const { return SET_RULES; }

    private:
        SituationModel *m_situation;
        QString m_oldRules;
        QString m_newRules;
};

class SetLaylineUndoCommand : public QUndoCommand {

    public:
        SetLaylineUndoCommand(SituationModel* situation, const int angle, QUndoCommand *parent = 0);
        ~SetLaylineUndoCommand();
        void undo();
        void redo();
        bool mergeWith(const QUndoCommand *command);
        int id() const { return SET_LAYLINE; }

    private:
        SituationModel *m_situation;
        int m_oldAngle;
        int m_newAngle;
};

class SetSeriesUndoCommand : public QUndoCommand {

    public:
        SetSeriesUndoCommand(SituationModel* situation, const int series, QUndoCommand *parent = 0);
        ~SetSeriesUndoCommand();
        void undo();
        void redo();
        bool mergeWith(const QUndoCommand *command);
        int id() const { return SET_SERIES; }

    private:
        SituationModel *m_situation;
        int m_oldSeries;
        int m_newSeries;
};

class SetAbstractUndoCommand : public QUndoCommand {

    public:
        SetAbstractUndoCommand(SituationModel* situation, QString abstract, QUndoCommand *parent = 0);
        ~SetAbstractUndoCommand();
        void undo();
        void redo();
        bool mergeWith(const QUndoCommand *command);
        int id() const { return SET_ABSTRACT; }

    private:
        SituationModel *m_situation;
        QString m_oldAbstract;
        QString m_newAbstract;
};

class SetDescriptionUndoCommand : public QUndoCommand {

    public:
        SetDescriptionUndoCommand(SituationModel* situation, QString description, QUndoCommand *parent = 0);
        ~SetDescriptionUndoCommand();
        void undo();
        void redo();
        bool mergeWith(const QUndoCommand *command);
        int id() const { return SET_DESCRIPTION; }

    private:
        SituationModel *m_situation;
        QString m_oldDescription;
        QString m_newDescription;
};

class AddTrackUndoCommand : public QUndoCommand {

    public:
        AddTrackUndoCommand(SituationModel* situation, QUndoCommand *parent = 0);
        ~AddTrackUndoCommand();
        void undo();
        void redo();

        TrackModel *track() {return m_track; }

    private:
        SituationModel *m_situation;
        TrackModel *m_track;
};

class DeleteTrackUndoCommand : public QUndoCommand {

    public:
        DeleteTrackUndoCommand(SituationModel* situation, TrackModel* track, QUndoCommand *parent = 0);
        ~DeleteTrackUndoCommand();
        void undo();
        void redo();
    private:
        SituationModel *m_situation;
        TrackModel *m_track;
};

class MoveModelUndoCommand : public QUndoCommand {

    public:
        MoveModelUndoCommand(QList<PositionModel*> &modelList, const QPointF &deltaPosition, QUndoCommand *parent = 0);
        ~MoveModelUndoCommand();
        void undo();
        void redo();
        bool mergeWith(const QUndoCommand *command);
        int id() const { return MOVE_MODEL; }
    private:
        QList<PositionModel*> m_modelList;
        QPointF m_deltaPosition;
};

class AddBoatUndoCommand : public QUndoCommand {

    public:
        AddBoatUndoCommand(TrackModel* track, QPointF& position, qreal heading, QUndoCommand *parent = 0);
        ~AddBoatUndoCommand();
        void undo();
        void redo();

        BoatModel *boat() {return m_boat; }

    private:
        TrackModel *m_track;
        BoatModel *m_boat;
};

class HeadingBoatUndoCommand : public QUndoCommand {

    public:
        HeadingBoatUndoCommand(QList<BoatModel*> &boatList, const qreal &heading, QUndoCommand *parent = 0);
        ~HeadingBoatUndoCommand();
        void undo();
        void redo();
        bool mergeWith(const QUndoCommand *command);
        int id() const { return HEADING_BOAT; }
    private:
        QList<BoatModel*> m_boatList;
        QList<qreal> m_headingList;
        qreal m_heading;
};

class DeleteBoatUndoCommand : public QUndoCommand {

    public:
        DeleteBoatUndoCommand(TrackModel* track, BoatModel* boat, QUndoCommand *parent = 0);
        ~DeleteBoatUndoCommand();
        void undo();
        void redo();
    private:
        TrackModel *m_track;
        BoatModel *m_boat;
        int m_order;
};

class AddMarkUndoCommand : public QUndoCommand {

    public:
        AddMarkUndoCommand(SituationModel* situation, QPointF& position, QUndoCommand *parent = 0);
        ~AddMarkUndoCommand();
        void undo();
        void redo();

        MarkModel *mark() {return m_mark; }

    private:
        SituationModel *m_situation;
        MarkModel *m_mark;
};

class ZoneMarkUndoCommand : public QUndoCommand {

    public:
        ZoneMarkUndoCommand(SituationModel* situation, QList<MarkModel*> &markList, QUndoCommand *parent = 0);
        ~ZoneMarkUndoCommand();
        void undo();
        void redo();
        bool mergeWith(const QUndoCommand *command);
        int id() const { return ZONE_MARK; }

    private:
        SituationModel *m_situation;
        QList<MarkModel*> m_markList;
};

class DeleteMarkUndoCommand : public QUndoCommand {

    public:
        DeleteMarkUndoCommand(SituationModel* situation, MarkModel* mark, QUndoCommand *parent = 0);
        ~DeleteMarkUndoCommand();
        void undo();
        void redo();
    private:
        SituationModel *m_situation;
        MarkModel *m_mark;
        int m_order;
};

#endif
