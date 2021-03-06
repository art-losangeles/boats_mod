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

#include "commontypes.h"
#include "model/situationmodel.h"
#include "model/trackmodel.h"
#include "model/boatmodel.h"
#include "model/markmodel.h"

extern int debugLevel;

// Set Title
SetTitleUndoCommand::SetTitleUndoCommand(SituationModel* situation, QString title, QUndoCommand *parent)
        : QUndoCommand(parent),
        m_situation(situation),
        m_oldTitle(situation->title()),
        m_newTitle(title) {
    if (debugLevel & 1 << COMMAND) std::cout << "new settitleundocommand" << std::endl;
}

SetTitleUndoCommand::~SetTitleUndoCommand() {
    if (debugLevel & 1 << COMMAND) std::cout << "end settitleundocommand" << std::endl;
}

void SetTitleUndoCommand::undo() {
    if (debugLevel & 1 << COMMAND) std::cout << "undo settitleundocommand"<< std::endl;
    m_situation->setTitle(m_oldTitle, true);
}

void SetTitleUndoCommand::redo() {
    if (debugLevel & 1 << COMMAND) std::cout << "redo settitleundocommand" << std::endl;
    m_situation->setTitle(m_newTitle, true);
}

bool SetTitleUndoCommand::mergeWith(const QUndoCommand *command) {
    const SetTitleUndoCommand *settitleCommand = static_cast<const SetTitleUndoCommand*>(command);
    if (m_situation != settitleCommand->m_situation)
        return false;

    m_newTitle = settitleCommand->m_newTitle;
    return true;
}

// Set Rules
SetRulesUndoCommand::SetRulesUndoCommand(SituationModel* situation, QString rules, QUndoCommand *parent)
        : QUndoCommand(parent),
        m_situation(situation),
        m_oldRules(situation->rules()),
        m_newRules(rules) {
    if (debugLevel & 1 << COMMAND) std::cout << "new setrulesundocommand" << std::endl;
}

SetRulesUndoCommand::~SetRulesUndoCommand() {
    if (debugLevel & 1 << COMMAND) std::cout << "end setrulesundocommand" << std::endl;
}

void SetRulesUndoCommand::undo() {
    if (debugLevel & 1 << COMMAND) std::cout << "undo setrulesundocommand"<< std::endl;
    m_situation->setRules(m_oldRules, true);
}

void SetRulesUndoCommand::redo() {
    if (debugLevel & 1 << COMMAND) std::cout << "redo setrulesundocommand" << std::endl;
    m_situation->setRules(m_newRules, true);
}

bool SetRulesUndoCommand::mergeWith(const QUndoCommand *command) {
    const SetRulesUndoCommand *setrulesCommand = static_cast<const SetRulesUndoCommand*>(command);
    if (m_situation != setrulesCommand->m_situation)
        return false;

    m_newRules = setrulesCommand->m_newRules;
    return true;
}

// Set Layline
SetLaylineUndoCommand::SetLaylineUndoCommand(SituationModel* situation, const int angle, QUndoCommand *parent)
        : QUndoCommand(parent),
        m_situation(situation),
        m_oldAngle(situation->laylineAngle()),
        m_newAngle(angle) {
    if (debugLevel & 1 << COMMAND) std::cout << "new setlaylineundocommand" << std::endl;
}

SetLaylineUndoCommand::~SetLaylineUndoCommand() {
    if (debugLevel & 1 << COMMAND) std::cout << "end setlaylineundocommand" << std::endl;
}

void SetLaylineUndoCommand::undo() {
    if (debugLevel & 1 << COMMAND) std::cout << "undo setlaylineundocommand" << std::endl;
    m_situation->setLaylineAngle(m_oldAngle, true);
}

void SetLaylineUndoCommand::redo() {
    if (debugLevel & 1 << COMMAND) std::cout << "redo setlaylineundocommand" << std::endl;
    m_situation->setLaylineAngle(m_newAngle, true);
}

bool SetLaylineUndoCommand::mergeWith(const QUndoCommand *command) {
    const SetLaylineUndoCommand *setlaylineCommand = static_cast<const SetLaylineUndoCommand*>(command);
    if (m_situation != setlaylineCommand->m_situation)
        return false;

    m_newAngle = setlaylineCommand->m_newAngle;
    return true;
}

// Set Series
SetSeriesUndoCommand::SetSeriesUndoCommand(SituationModel* situation, const int series, QUndoCommand *parent)
        : QUndoCommand(parent),
        m_situation(situation),
        m_oldSeries(situation->situationSeries()),
        m_newSeries(series) {
    if (debugLevel & 1 << COMMAND) std::cout << "new setseriesundocommand" << std::endl;
}

SetSeriesUndoCommand::~SetSeriesUndoCommand() {
    if (debugLevel & 1 << COMMAND) std::cout << "end setseriesundocommand" << std::endl;
}

void SetSeriesUndoCommand::undo() {
    if (debugLevel & 1 << COMMAND) std::cout << "undo setseriesundocommand" << std::endl;
    m_situation->setSituationSeries(m_oldSeries, true);
}

void SetSeriesUndoCommand::redo() {
    if (debugLevel & 1 << COMMAND) std::cout << "redo setseriesundocommand" << std::endl;
    m_situation->setSituationSeries(m_newSeries, true);
}

bool SetSeriesUndoCommand::mergeWith(const QUndoCommand *command) {
    const SetSeriesUndoCommand *setseriesCommand = static_cast<const SetSeriesUndoCommand*>(command);
    if (m_situation != setseriesCommand->m_situation)
        return false;

    m_newSeries = setseriesCommand->m_newSeries;
    return true;
}

// Set Abstract
SetAbstractUndoCommand::SetAbstractUndoCommand(SituationModel* situation, QString abstract, QUndoCommand *parent)
        : QUndoCommand(parent),
        m_situation(situation),
        m_oldAbstract(situation->abstract()),
        m_newAbstract(abstract) {
    if (debugLevel & 1 << COMMAND) std::cout << "new setabstractundocommand" << std::endl;
}

SetAbstractUndoCommand::~SetAbstractUndoCommand() {
    if (debugLevel & 1 << COMMAND) std::cout << "end setabstractundocommand" << std::endl;
}

void SetAbstractUndoCommand::undo() {
    if (debugLevel & 1 << COMMAND) std::cout << "undo setabstractundocommand"<< std::endl;
    m_situation->setAbstract(m_oldAbstract, true);
}

void SetAbstractUndoCommand::redo() {
    if (debugLevel & 1 << COMMAND) std::cout << "redo setabstractundocommand" << std::endl;
    m_situation->setAbstract(m_newAbstract, true);
}

bool SetAbstractUndoCommand::mergeWith(const QUndoCommand *command) {
    const SetAbstractUndoCommand *setabstractCommand = static_cast<const SetAbstractUndoCommand*>(command);
    if (m_situation != setabstractCommand->m_situation)
        return false;

    m_newAbstract = setabstractCommand->m_newAbstract;
    return true;
}

// Set Description
SetDescriptionUndoCommand::SetDescriptionUndoCommand(SituationModel* situation, QString description, QUndoCommand *parent)
        : QUndoCommand(parent),
        m_situation(situation),
        m_oldDescription(situation->description()),
        m_newDescription(description) {
    if (debugLevel & 1 << COMMAND) std::cout << "new setdescriptionundocommand" << std::endl;
}

SetDescriptionUndoCommand::~SetDescriptionUndoCommand() {
    if (debugLevel & 1 << COMMAND) std::cout << "end setdescriptionundocommand" << std::endl;
}

void SetDescriptionUndoCommand::undo() {
    if (debugLevel & 1 << COMMAND) std::cout << "undo setdescriptionundocommand"<< std::endl;
    m_situation->setDescription(m_oldDescription, true);
}

void SetDescriptionUndoCommand::redo() {
    if (debugLevel & 1 << COMMAND) std::cout << "redo setdescriptionundocommand" << std::endl;
    m_situation->setDescription(m_newDescription, true);
}

bool SetDescriptionUndoCommand::mergeWith(const QUndoCommand *command) {
    const SetDescriptionUndoCommand *setdescriptionCommand = static_cast<const SetDescriptionUndoCommand*>(command);
    if (m_situation != setdescriptionCommand->m_situation)
        return false;

    m_newDescription = setdescriptionCommand->m_newDescription;
    return true;
}

// Add Track
AddTrackUndoCommand::AddTrackUndoCommand(SituationModel* situation, QUndoCommand *parent)
        : QUndoCommand(parent),
        m_situation(situation) {
    if (debugLevel & 1 << COMMAND) std::cout << "new addtrackundocommand" << std::endl;
    m_track = new TrackModel(situation);
}

AddTrackUndoCommand::~AddTrackUndoCommand() {
    if (debugLevel & 1 << COMMAND) std::cout << "end addtrackundocommand" << std::endl;
    delete m_track;
}

void AddTrackUndoCommand::redo() {
    if (debugLevel & 1 << COMMAND) std::cout << "redo addtrackundocommand" << std::endl;
    m_situation->addTrack(m_track);
}

void AddTrackUndoCommand::undo() {
    if (debugLevel & 1 << COMMAND) std::cout << "undo addtrackundocommand" << std::endl;
    m_situation->deleteTrack(m_track);
}

// Delete Track
DeleteTrackUndoCommand::DeleteTrackUndoCommand(SituationModel* situation, TrackModel* track, QUndoCommand *parent)
        : QUndoCommand(parent),
        m_situation(situation),
        m_track(track) {
    if (debugLevel & 1 << COMMAND) std::cout << "new removetrackundocommand" << std::endl;
}

DeleteTrackUndoCommand::~DeleteTrackUndoCommand() {
    if (debugLevel & 1 << COMMAND) std::cout << "end removetrackundocommand" << std::endl;
}

void DeleteTrackUndoCommand::redo() {
    if (debugLevel & 1 << COMMAND) std::cout << "redo removetrackundocommand" << std::endl;
    m_situation->deleteTrack(m_track);
}

void DeleteTrackUndoCommand::undo() {
    if (debugLevel & 1 << COMMAND) std::cout << "undo removetrackundocommand" << std::endl;
    m_situation->addTrack(m_track);
}

// Move Model
MoveModelUndoCommand::MoveModelUndoCommand(QList<PositionModel*> &modelList, const QPointF &deltaPosition, QUndoCommand *parent)
        : QUndoCommand(parent),
        m_modelList(modelList),
        m_deltaPosition(deltaPosition) {
    if (debugLevel & 1 << COMMAND) std::cout << "new movemodelundocommand" << std::endl;
}

MoveModelUndoCommand::~MoveModelUndoCommand() {
    if (debugLevel & 1 << COMMAND) std::cout << "end movemodelundocommand" << std::endl;
}

void MoveModelUndoCommand::undo() {
    if (debugLevel & 1 << COMMAND) std::cout << "undo movemodelundocommand" << std::endl;
    for(int i=0; i< m_modelList.size(); i++) {
        PositionModel *model = m_modelList[i];
        model->setPosition(model->position() - m_deltaPosition, true);
    }
}

void MoveModelUndoCommand::redo() {
    if (debugLevel & 1 << COMMAND) std::cout << "redo movemodelundocommand" << std::endl;
    for(int i=0; i< m_modelList.size(); i++) {
        PositionModel *model = m_modelList[i];
        model->setPosition(model->position() + m_deltaPosition, true);
    }
}

bool MoveModelUndoCommand::mergeWith(const QUndoCommand *command) {
    const MoveModelUndoCommand *moveCommand = static_cast<const MoveModelUndoCommand*>(command);
    if (m_modelList != moveCommand->m_modelList)
        return false;

    m_deltaPosition += moveCommand->m_deltaPosition;
    return true;
}

// Add Boat
AddBoatUndoCommand::AddBoatUndoCommand(TrackModel* track, QPointF& position, qreal heading, QUndoCommand *parent)
        : QUndoCommand(parent),
        m_track(track) {
    if (debugLevel & 1 << COMMAND) std::cout << "new addboatundocommand" << std::endl;
    m_boat = new BoatModel(track);
    m_boat->setPosition(position);
    m_boat->setHeading(heading);
}

AddBoatUndoCommand::~AddBoatUndoCommand() {
    if (debugLevel & 1 << COMMAND) std::cout << "end addboatundocommand" << std::endl;
    delete m_boat;
}

void AddBoatUndoCommand::redo() {
    if (debugLevel & 1 << COMMAND) std::cout << "redo addboatundocommand" << std::endl;
    m_track->addBoat(m_boat);
}

void AddBoatUndoCommand::undo() {
    if (debugLevel & 1 << COMMAND) std::cout << "undo addboatundocommand" << std::endl;
    m_track->deleteBoat(m_boat);
}

// Heading Boat
HeadingBoatUndoCommand::HeadingBoatUndoCommand(QList<BoatModel*> &boatList, const qreal &heading, QUndoCommand *parent)
        : QUndoCommand(parent),
        m_boatList(boatList),
        m_heading(heading) {
    if (debugLevel & 1 << COMMAND) std::cout << "new headingboatundocommand" << std::endl;
    foreach (BoatModel *boat, boatList) {
        m_headingList << boat->heading();
    }
}

HeadingBoatUndoCommand::~HeadingBoatUndoCommand() {
    if (debugLevel & 1 << COMMAND) std::cout << "end headingboatundocommand" << std::endl;
}

void HeadingBoatUndoCommand::undo() {
    if (debugLevel & 1 << COMMAND) std::cout << "undo headingboatundocommand" << std::endl;
    for(int i=0; i< m_boatList.size(); i++) {
        BoatModel *boat = m_boatList[i];
        boat->setHeading(m_headingList[i], true);
    }
}

void HeadingBoatUndoCommand::redo() {
    if (debugLevel & 1 << COMMAND) std::cout << "redo headingboatundocommand" << std::endl;
    for(int i=0; i< m_boatList.size(); i++) {
        BoatModel *boat = m_boatList[i];
        boat->setHeading(m_heading, true);
    }
}

bool HeadingBoatUndoCommand::mergeWith(const QUndoCommand *command) {
    const HeadingBoatUndoCommand *headingCommand = static_cast<const HeadingBoatUndoCommand*>(command);
    if (m_boatList != headingCommand->m_boatList)
        return false;

    m_heading = headingCommand->m_heading;
    return true;
}

// Delete Boat
DeleteBoatUndoCommand::DeleteBoatUndoCommand(TrackModel* track, BoatModel* boat, QUndoCommand *parent)
        : QUndoCommand(parent),
        m_track(track),
        m_boat(boat) {
    if (debugLevel & 1 << COMMAND) std::cout << "new deleteboatundocommand" << std::endl;
}

DeleteBoatUndoCommand::~DeleteBoatUndoCommand() {
    if (debugLevel & 1 << COMMAND) std::cout << "end deletebboatundocommand" << std::endl;
}

void DeleteBoatUndoCommand::redo() {
    if (debugLevel & 1 << COMMAND) std::cout << "redo deletebboatundocommand" << std::endl;
    m_order = m_track->deleteBoat(m_boat);
}

void DeleteBoatUndoCommand::undo() {
    if (debugLevel & 1 << COMMAND) std::cout << "undo deletebboatundocommand" << std::endl;
    m_track->addBoat(m_boat, m_order);
}

// Add Mark
AddMarkUndoCommand::AddMarkUndoCommand(SituationModel* situation, QPointF& position, QUndoCommand *parent)
        : QUndoCommand(parent),
        m_situation(situation) {
    if (debugLevel & 1 << COMMAND) std::cout << "new addmarkundocommand" << std::endl;
    m_mark = new MarkModel(situation);
    m_mark->setPosition(position);
}

AddMarkUndoCommand::~AddMarkUndoCommand() {
    if (debugLevel & 1 << COMMAND) std::cout << "end addmarkundocommand" << std::endl;
    delete m_mark;
}

void AddMarkUndoCommand::redo() {
    if (debugLevel & 1 << COMMAND) std::cout << "redo addmarkundocommand" << std::endl;
    m_situation->addMark(m_mark);
}

void AddMarkUndoCommand::undo() {
    if (debugLevel & 1 << COMMAND) std::cout << "undo addmarkundocommand" << std::endl;
    m_situation->deleteMark(m_mark);
}

// Zone Mark
ZoneMarkUndoCommand::ZoneMarkUndoCommand(SituationModel *situation, QList<MarkModel*> &markList, QUndoCommand *parent)
        : QUndoCommand(parent),
        m_situation(situation),
        m_markList(markList) {
    if (debugLevel & 1 << COMMAND) std::cout << "new zonemarkundocommand" << std::endl;
}

ZoneMarkUndoCommand::~ZoneMarkUndoCommand() {
    if (debugLevel & 1 << COMMAND) std::cout << "end zonemarkundocommand" << std::endl;
}

void ZoneMarkUndoCommand::undo() {
    if (debugLevel & 1 << COMMAND) std::cout << "undo zonemarkundocommand" << std::endl;
    for(int i=0; i< m_markList.size(); i++) {
        MarkModel *mark = m_markList[i];
        mark->setZone(!mark->zone(), true);
    }
}

void ZoneMarkUndoCommand::redo() {
    if (debugLevel & 1 << COMMAND) std::cout << "redo zonemarkundocommand" << std::endl;
    for(int i=0; i< m_markList.size(); i++) {
        MarkModel *mark = m_markList[i];
        mark->setZone(!mark->zone(), true);
    }
}

bool ZoneMarkUndoCommand::mergeWith(const QUndoCommand *command) {
    const ZoneMarkUndoCommand *zoneCommand = static_cast<const ZoneMarkUndoCommand*>(command);
    if (m_markList != zoneCommand->m_markList)
        return false;
    undo();
    m_situation->undoStack()->setIndex(m_situation->undoStack()->index()-1);
    return true;
}

// Delete Mark
DeleteMarkUndoCommand::DeleteMarkUndoCommand(SituationModel* situation, MarkModel* mark, QUndoCommand *parent)
        : QUndoCommand(parent),
        m_situation(situation),
        m_mark(mark) {
    if (debugLevel & 1 << COMMAND) std::cout << "new deletemarkundocommand" << std::endl;
}

DeleteMarkUndoCommand::~DeleteMarkUndoCommand() {
    if (debugLevel & 1 << COMMAND) std::cout << "end deletebmarkundocommand" << std::endl;
}

void DeleteMarkUndoCommand::redo() {
    if (debugLevel & 1 << COMMAND) std::cout << "redo deletebmarkundocommand" << std::endl;
    m_order = m_situation->deleteMark(m_mark);
}

void DeleteMarkUndoCommand::undo() {
    if (debugLevel & 1 << COMMAND) std::cout << "undo deletebmarkundocommand" << std::endl;
    m_situation->addMark(m_mark, m_order);
}
