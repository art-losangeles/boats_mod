//
// C++ Interface: situationscene
//
// Description:
//
//
// Author: Thibaut GRIDEL <tgridel@free.fr>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef SITUATIONSCENE_H
#define SITUATIONSCENE_H

#include <QtGui>

class SituationModel;
class TrackModel;
class BoatModel;
class MarkModel;

typedef enum {
    NO_STATE,
    CREATE_TRACK,
    CREATE_BOAT,
    CREATE_MARK
} SceneState;

enum {
    BOAT_TYPE = QGraphicsItem::UserType + 1,
    MARK_TYPE
};

class SituationScene : public QGraphicsScene {
        Q_OBJECT
    public:
        SituationScene(SituationModel* situation);
        ~SituationScene() {}

        void setState(const SceneState& theValue) { m_state = theValue; }
        void setModelPressed(BoatModel *theValue) {m_modelPressed = theValue; }
        SceneState state() const { return m_state; }
        QList< BoatModel * > selectedModels() const { return m_selectedModels; }
        QList< MarkModel * > selectedMarkModels() const { return m_selectedMarkModels; }

    signals:
        void itemMoved(QList<BoatModel*> movedItems, const QPointF &movedFromPosition);

    public slots:
        void setSelectedModels();
        void addTrack(TrackModel *track);
        void deleteTrack(TrackModel *track);
        void addBoatItem(BoatModel *boat);
        void deleteBoatItem();
        void addMarkItem(MarkModel *mark);
        void deleteMarkItem();
        void setLaylines(const qreal angle);

    protected:
        void mousePressEvent(QGraphicsSceneMouseEvent *event);
        void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
        void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

    private:
        void mouseMoveBoatEvent(QGraphicsSceneMouseEvent *event);
        void mouseHeadingEvent(QGraphicsSceneMouseEvent *event);
        void mouseCreateTrackEvent(QGraphicsSceneMouseEvent *event);
        void mouseCreateBoatEvent(QGraphicsSceneMouseEvent *event);
        void mouseCreateMarkEvent(QGraphicsSceneMouseEvent *event);

        SituationModel *m_situation;
        QList<BoatModel*> m_selectedModels;
        QList<BoatModel*> m_movingModels;
        QList<MarkModel*> m_selectedMarkModels;
        BoatModel* m_modelPressed;
        TrackModel *m_trackCreated;
        QPointF m_fromPosition;
        SceneState m_state;
};

#endif
