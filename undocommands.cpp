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

#include "model/situationmodel.h"
#include "model/trackmodel.h"
#include "model/boatmodel.h"

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
AddBoatUndoCommand::AddBoatUndoCommand(TrackModel* track, QUndoCommand *parent)
        : QUndoCommand(parent),
        m_track(track) {
    std::cout << "new addboatundocommand" << std::endl;
    m_boat = new BoatModel(track);
    m_boat->setPosition(QPoint(0,-20*track->size()));
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
    m_track->deleteBoat(m_boat);
}

void DeleteBoatUndoCommand::undo() {
    m_track->addBoat(m_boat);
}
