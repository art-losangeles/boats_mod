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
class BoatModel;
class MarkModel;

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

class MoveBoatUndoCommand : public QUndoCommand {

    public:
        enum { Id = 1 };
        MoveBoatUndoCommand(QList<BoatModel*> &boatList, const QPointF &deltaPosition, const qreal &heading, QUndoCommand *parent = 0);
        ~MoveBoatUndoCommand();
        void undo();
        void redo();
        bool mergeWith(const QUndoCommand *command);
        int id() const { return Id; }
    private:
        QList<BoatModel*> m_boatList;
        QList<qreal> m_headingList;
        QPointF m_deltaPosition;
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
