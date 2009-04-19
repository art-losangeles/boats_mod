//
// C++ Implementation: XmlSituationReader
//
// Description:
//
//
// Author: Thibaut GRIDEL <tgridel@free.fr>
//
// Copyright (c) 2008-2009 Thibaut GRIDEL
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
//
#include <iostream>

#include "xmlsituationreader.h"

#include "commontypes.h"
#include "situationmodel.h"
#include "trackmodel.h"
#include "boatmodel.h"
#include "markmodel.h"

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
            if (name() == "title") {
                m_situation->undoStack()->push(
                        new SetTitleUndoCommand(m_situation,
                                                readElementText()));

            } else if (name() == "rules") {
                m_situation->undoStack()->push(
                        new SetRulesUndoCommand(m_situation,
                                                readElementText()));

            } else if (name() == "abstract") {
                m_situation->undoStack()->push(
                        new SetAbstractUndoCommand(m_situation,
                                                   readElementText()));

            } else if (name() == "description") {
                m_situation->undoStack()->push(
                        new SetDescriptionUndoCommand(m_situation,
                                                      readElementText()));

            } else if (name() == "series") {
                m_situation->undoStack()->push(
                        new SetSituationSeriesUndoCommand(m_situation,
                                                          series(readElementText())));

            } else if (name() == "showlayline") {
                if (readElementText() == "0") {
                    m_situation->undoStack()->push(
                            new SetShowLaylineUndoCommand(m_situation));
                }

            } else if (name() == "layline") {
                m_situation->undoStack()->push(
                        new SetLaylineUndoCommand(m_situation,
                                                  readElementText().toInt()));

            } else if (name() == "length") {
                m_situation->setSituationLength(readElementText().toInt());

            } else if (name() == "mark") {
                readMark(m_situation);

            } else if (name() == "track") {
                readTrack(m_situation);

            } else {
                m_situation->appendDiscardedXml(readUnknownElement());
            }
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
                track->setSeries(series(readElementText()));
            else if (name() == "boat")
                readBoat(situation, track);
            else
                track->appendDiscardedXml(readUnknownElement());
        }
    }
}

void XmlSituationReader::readBoat(SituationModel *situation, TrackModel *track) {
    QPointF pos;
    qreal heading = 0;
    qreal trim = 0;
    Boats::Overlaps overlap = Boats::none;
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
            else if (name() == "trim")
                trim = readElementText().toFloat();
            else if (name() == "overlap") {
                overlap = (Boats::Overlaps)FLAG_VALUE(Boats, Overlap, readElementText().toStdString().c_str());
            }
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
    boat->setTrim(trim);
    boat->setOverlap(overlap);
    foreach (const QString elem, discarded) {
        boat->appendDiscardedXml(elem);
    }
}

void XmlSituationReader::readMark(SituationModel *situation) {
    QPointF pos;
    QColor color;
    bool zone = false;
    int length = 0;
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
            else if (name() == "zone")
                zone = (readElementText() == "1");
            else if (name() == "length")
                length = (readElementText().toInt());
            else
                discarded.append(readUnknownElement());
        }
    }
    AddMarkUndoCommand *command = new AddMarkUndoCommand(situation, pos);
    situation->undoStack()->push(command);
    MarkModel *mark = command->mark();
    mark->setColor(color);
    mark->setZone(zone);
    if (length != 0) {
        mark->setLength(length);
    }
}

Boats::Series XmlSituationReader::series(const QString series) {
    int i;
    i = ENUM_VALUE(Boats, Series, series.toStdString().c_str());
    if (i != -1) {
        return (Boats::Series)i;
    } else {
        return Boats::keelboat;
    }
}
