//
// C++ Implementation: undocommands
//
// Description:
//
//
// Author: Thibaut GRIDEL <tgridel@free.fr>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "undocommands.h"

#include <iostream>
#include <cmath>

#include "model/situationmodel.h"
#include "model/trackmodel.h"
#include "model/boatmodel.h"
#include "model/markmodel.h"

// Add Track
AddTrackUndoCommand::AddTrackUndoCommand(SituationModel* situation, QUndoCommand *parent)
        : QUndoCommand(parent),
        m_situation(situation) {
    std::cout << "new addtrackundocommand" << std::endl;
    m_track = new TrackModel(situation);
}

AddTrackUndoCommand::~AddTrackUndoCommand() {
    std::cout << "end addtrackundocommand" << std::endl;
    delete m_track;
}

void AddTrackUndoCommand::redo() {
    m_situation->addTrack(m_track);
}

void AddTrackUndoCommand::undo() {
    m_situation->deleteTrack(m_track);
}

// Delete Track
DeleteTrackUndoCommand::DeleteTrackUndoCommand(SituationModel* situation, TrackModel* track, QUndoCommand *parent)
        : QUndoCommand(parent),
        m_situation(situation),
        m_track(track) {
    std::cout << "new removetrackundocommand" << std::endl;
}

DeleteTrackUndoCommand::~DeleteTrackUndoCommand() {
    std::cout << "end removetrackundocommand" << std::endl;
}

void DeleteTrackUndoCommand::redo() {
    m_situation->deleteTrack(m_track);
}

void DeleteTrackUndoCommand::undo() {
    m_situation->addTrack(m_track);
}

// Add Boat
AddBoatUndoCommand::AddBoatUndoCommand(TrackModel* track, QPointF& position, qreal heading, QUndoCommand *parent)
        : QUndoCommand(parent),
        m_track(track) {
    std::cout << "new addboatundocommand" << std::endl;
    m_boat = new BoatModel(track);
    m_boat->setPosition(position);
    m_boat->setHeading(heading);
}

AddBoatUndoCommand::~AddBoatUndoCommand() {
    std::cout << "end addboatundocommand" << std::endl;
    delete m_boat;
}

void AddBoatUndoCommand::redo() {
    m_track->addBoat(m_boat);
}

void AddBoatUndoCommand::undo() {
    m_track->deleteBoat(m_boat);
}

// Move Boat
MoveBoatUndoCommand::MoveBoatUndoCommand(QList<BoatModel*> &boatList, const QPointF &deltaPosition, const qreal &heading, QUndoCommand *parent)
        : QUndoCommand(parent),
        m_boatList(boatList),
        m_deltaPosition(deltaPosition),
        m_heading(heading) {
    std::cout << "new moveboatundocommand" << std::endl;
    if (heading != 0) {
        foreach (BoatModel *boat, boatList) {
            m_headingList << boat->heading();
        }
    }
}

MoveBoatUndoCommand::~MoveBoatUndoCommand() {
    std::cout << "end moveboatundocommand" << std::endl;
}



void MoveBoatUndoCommand::undo() {
    for(int i=0; i< m_boatList.size(); i++) {
        BoatModel *boat = m_boatList[i];
        boat->setPosition(boat->position() - m_deltaPosition, true);
        if (!m_headingList.isEmpty()) {
            boat->setHeading(m_headingList[i], true);
        }
    }
}

void MoveBoatUndoCommand::redo() {
    for(int i=0; i< m_boatList.size(); i++) {
        BoatModel *boat = m_boatList[i];
            boat->setPosition(boat->position() + m_deltaPosition, true);
        if (!m_headingList.isEmpty()) {
            boat->setHeading(m_heading, true);
        }
    }
}

bool MoveBoatUndoCommand::mergeWith(const QUndoCommand *command) {
    const MoveBoatUndoCommand *moveCommand = static_cast<const MoveBoatUndoCommand*>(command);
    if (m_boatList != moveCommand->m_boatList)
        return false;

    m_deltaPosition += moveCommand->m_deltaPosition;
    if (!(moveCommand->m_headingList.isEmpty())) {
        m_heading = moveCommand->m_heading;
        if (m_headingList.isEmpty()) {
            m_headingList = moveCommand->m_headingList;
        }
    }
    return true;
}

// Delete Boat
DeleteBoatUndoCommand::DeleteBoatUndoCommand(TrackModel* track, BoatModel* boat, QUndoCommand *parent)
        : QUndoCommand(parent),
        m_track(track),
        m_boat(boat) {
    std::cout << "new deleteboatundocommand" << std::endl;
}

DeleteBoatUndoCommand::~DeleteBoatUndoCommand() {
    std::cout << "end deletebboatundocommand" << std::endl;
}

void DeleteBoatUndoCommand::redo() {
    m_order = m_track->deleteBoat(m_boat);
}

void DeleteBoatUndoCommand::undo() {
    m_track->addBoat(m_boat, m_order);
}

// Add Mark
AddMarkUndoCommand::AddMarkUndoCommand(SituationModel* situation, QPointF& position, QUndoCommand *parent)
        : QUndoCommand(parent),
        m_situation(situation) {
    std::cout << "new addmarkundocommand" << std::endl;
    m_mark = new MarkModel(situation);
    m_mark->setPosition(position);
}

AddMarkUndoCommand::~AddMarkUndoCommand() {
    std::cout << "end addmarkundocommand" << std::endl;
    delete m_mark;
}

void AddMarkUndoCommand::redo() {
    m_situation->addMark(m_mark);
}

void AddMarkUndoCommand::undo() {
    m_situation->deleteMark(m_mark);
}

// Delete Mark
DeleteMarkUndoCommand::DeleteMarkUndoCommand(SituationModel* situation, MarkModel* mark, QUndoCommand *parent)
        : QUndoCommand(parent),
        m_situation(situation),
        m_mark(mark) {
    std::cout << "new deletemarkundocommand" << std::endl;
}

DeleteMarkUndoCommand::~DeleteMarkUndoCommand() {
    std::cout << "end deletebmarkundocommand" << std::endl;
}

void DeleteMarkUndoCommand::redo() {
    m_order = m_situation->deleteMark(m_mark);
}

void DeleteMarkUndoCommand::undo() {
    m_situation->addMark(m_mark, m_order);
}
