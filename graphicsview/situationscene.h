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
class PositionModel;
class BoatModel;
class MarkModel;
class BoatAnimation;

/**
    \enum SceneState

    This enum defines the different modal states of the edition. When
    the user wants to input data to the scenario, he sets the current
    edition mode.

    \value NO_STATE No particular mode is set. In this mode item selection
    and movement is allowed.

    \value CREATE_TRACK The Create Track mode will create a new TrackModel
    object in the scenario. This mode is then overriden to CREATE_BOAT as
    a natural action is to append boat positions.

    \value CREATE_BOAT The Create Boat will append a BoatModel to the
    track of the lastly selected boat object.

    \value CREATE_MARK The Create Mark will create a new MarkModel to
    the scenario.

    \value ANIMATE The Animation will be prepared, and no further input
    will be allowed in this mode.

*/

typedef enum {
    NO_STATE,
    CREATE_TRACK,
    CREATE_BOAT,
    CREATE_MARK,
    ANIMATE
} SceneState;

enum {
    BOAT_TYPE = QGraphicsItem::UserType + 1,
    MARK_TYPE
};

/**
    \class SituationScene

    \brief The Scene for a scenario

    The class represents the Scene for a Scenario, according to the
    Graphics View Framework.

    It inherits QGraphicsScene and manages user input.

    The class handles a modal input status to trigger various actions
    depending on the mode.

    Action on Objects is handled by overloading the event methods:
    keyPressEvent(), mousePressEvent(), mouseReleaseEvent() and
    mouseMoveEvent().

    Specialised methods handle the possible events and generally use
    Undo Framework classes to modify the ScenarioModel.

    QGraphicsItem Objects selection is handled through setSelectedModels()
    slot, and updates the various Lists needed to determine matching Model
    objects.

    The class provides slots to react to SituationModel changes and update
    drawing accordingly.

    The class prepares the drawing of animation as well, through
    setAnimation() and unSetAnimation() methods.

    \sa QGraphicsScene, SituationModel, BoatGraphicsItem, MarkGraphicsItem
*/

class SituationScene : public QGraphicsScene {
        Q_OBJECT
    public:
        SituationScene(SituationModel* situation);
        ~SituationScene() {}

        void setState(const SceneState& theValue) { m_state = theValue; emit stateChanged(m_state); }
        void setModelPressed(BoatModel *theValue) {m_modelPressed = theValue; }
        SceneState state() const { return m_state; }
        QList< BoatModel * > selectedBoatModels() const { return m_selectedBoatModels; }
        QList< MarkModel * > selectedMarkModels() const { return m_selectedMarkModels; }
        void setActionMenu(QMenu *theValue) { m_actionMenu = theValue; }

    signals:
        void itemMoved(QList<BoatModel*> movedItems, const QPointF &movedFromPosition);
        void stateChanged(SceneState newState);
        void selectedModelsChanged();

    public slots:
        // Slot for selection mechanism
        void setSelectedModels();

        // Slots for SituationModel signals
        void addTrack(TrackModel *track);
        void deleteTrack(TrackModel *track);
        void addBoatItem(BoatModel *boat);
        void deleteBoatItem();
        void addMarkItem(MarkModel *mark);
        void deleteMarkItem();
        void setLaylines(const int angle);

        // Slots for animation signals
        void setAnimation(QTimeLine *timer);
        void unSetAnimation();

    protected:
        // Overloaded methods for Events
        void keyPressEvent(QKeyEvent *event);
        void mousePressEvent(QGraphicsSceneMouseEvent *event);
        void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
        void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

    private:
        // Specialised Event methods
        void mouseSelectEvent(QGraphicsSceneMouseEvent *event);
        void mouseMoveModelEvent(QGraphicsSceneMouseEvent *event);
        void mouseHeadingEvent(QGraphicsSceneMouseEvent *event);
        void mouseCreateTrackEvent(QGraphicsSceneMouseEvent *event);
        void mouseCreateBoatEvent(QGraphicsSceneMouseEvent *event);
        void mouseCreateMarkEvent(QGraphicsSceneMouseEvent *event);

        SituationModel *m_situation;

        // Bookkeeping references to selected models
        /// \a m_selectedModels holds the list of selected PositionModel
        QList<PositionModel*> m_selectedModels;

        /// \a m_selectedBoatsModels holds the list of selected BoatModel
        QList<BoatModel*> m_selectedBoatModels;

        /// \a m_selectedMarkModels holds the list of selected MarkModel
        QList<MarkModel*> m_selectedMarkModels;

        /// \a m_animationItems holds the list of BoatAnimation items
        /// created for animation mode
        QList<BoatAnimation*> m_animationItems;

        /// \a m_modelPressed holds the BoatModel being pressed
        BoatModel* m_modelPressed;

        /// \a m_trackCreated holds the last selected TrackModel
        TrackModel *m_trackCreated;

        /// \a m_fromPosition holds the QPointF where mouse was pressed
        QPointF m_fromPosition;

        /// \a m_state holds the SceneState for the current scenario
        SceneState m_state;

        /// \a m_time holds the timer used for movement filtering
        QTime m_time;

        /// \a m_clickTime holds the timer used for click/press detection
        QTime m_clickTime;

        QMenu *m_actionMenu;
};

#endif
