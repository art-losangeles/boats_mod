//
// C++ Implementation: XmlSituationReader
//
// Description:
//
//
// Author: Thibaut GRIDEL <tgridel@free.fr>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include <iostream>

#include "xmlsituationreader.h"

#include "commontypes.h"
#include "model/situationmodel.h"
#include "model/trackmodel.h"
#include "model/boatmodel.h"
#include "model/markmodel.h"

#include "undocommands.h"

XmlSituationReader::XmlSituationReader(SituationModel *situation)
        : m_situation(situation) {
}

XmlSituationReader::~XmlSituationReader() {}

bool XmlSituationReader::read(QIODevice *device) {
    setDevice(device);

    while (!atEnd()) {
        readNext();

        if (isStartElement()) {
            if (name() == "xmlscenario" && attributes().value("version") == "1.0")
                readSituation();
            else
                raiseError(QObject::tr("The file is not an xmlscenario version 1.0 file."));
        }
    }
    return !error();
}

QString XmlSituationReader::readUnknownElement() {
    QString elem;
    elem.append("<").append(name()).append(">");
    while (!atEnd()) {
        readNext();
        if (isCharacters())
            elem.append(text());
        else if (isStartElement())
            elem.append(readUnknownElement());

        else if (isEndElement()) {
            elem.append("</").append(name()).append(">");
            break;
        }
    }
    std::cout << "unread data " << elem.toStdString() << std::endl;
    return elem;
}

void XmlSituationReader::readSituation() {
    while (!atEnd()) {
        readNext();
        if (isEndElement())
            break;
        if (isStartElement()) {
            if (name() == "title")
                m_situation->setTitle(readElementText());
            else if (name() == "rules")
                m_situation->setRules(readElementText());
            else if (name() == "abstract")
                m_situation->setAbstract(readElementText());
            else if (name() == "description")
                m_situation->setDescription(readElementText());
            else if (name() == "layline")
                m_situation->setLaylineAngle(readElementText().toFloat(), true);
            else if (name() == "mark")
                readMark(m_situation);
            else if (name() == "track")
                readTrack(m_situation);
            else
                m_situation->appendDiscardedXml(readUnknownElement());
        }
    }
}

void XmlSituationReader::readTrack(SituationModel *situation) {
    AddTrackUndoCommand *command = new AddTrackUndoCommand(situation);
    situation->undoStack()->push(command);
    TrackModel *track = command->track();
    while (!atEnd()) {
        readNext();
        if (isEndElement())
            break;
        if (isStartElement()) {
            if (name() == "color")
                track->setColor(QColor(readElementText()));
            else if (name() == "series")
                readSeries(track, readElementText());
            else if (name() == "boat")
                readBoat(situation, track);
            else
                track->appendDiscardedXml(readUnknownElement());
        }
    }
}

void XmlSituationReader::readSeries(TrackModel *track, const QString series) {
    int i;
    for (i=0; series!=m_situation->seriesNames()[i] && i < m_situation->seriesNames().size(); i++) {
    }
    if (i != UNKNOWN) {
        track->setSeries((Series)i, true);
    }
}
void XmlSituationReader::readBoat(SituationModel *situation, TrackModel *track) {
    QPointF pos;
    qreal heading = 0;
    QStringList discarded;
    while (!atEnd()) {
        readNext();
        if (isEndElement())
            break;
        if (isStartElement()) {
            if (name() == "x")
                pos.setX(readElementText().toFloat());
            else if (name() == "y")
                pos.setY(readElementText().toFloat());
            else if (name() == "heading")
                heading = readElementText().toFloat();
            else
                discarded.append(readUnknownElement());
        }
    }
    BoatModel *boat;
    if (track->size() == 0) {
        boat = new BoatModel(track);
        boat->setPosition(pos);
        boat->setHeading(heading);
        track->addBoat(boat);
    } else {
        AddBoatUndoCommand *command = new AddBoatUndoCommand(track, pos, heading);
        situation->undoStack()->push(command);
        boat = command->boat();
    }
    foreach (QString elem, discarded) {
        boat->appendDiscardedXml(elem);
    }
}

void XmlSituationReader::readMark(SituationModel *situation) {
    QPointF pos;
    QColor color;
    QStringList discarded;
    while (!atEnd()) {
        readNext();
        if (isEndElement())
            break;
        if (isStartElement()) {
            if (name() == "x")
                pos.setX(readElementText().toFloat());
            else if (name() == "y")
                pos.setY(readElementText().toFloat());
            else if (name() == "color")
                color.setNamedColor(readElementText());
            else
                discarded.append(readUnknownElement());
        }
    }
    AddMarkUndoCommand *command = new AddMarkUndoCommand(situation, pos);
    situation->undoStack()->push(command);
    MarkModel *mark = command->mark();
    mark->setColor(color,true);
}
