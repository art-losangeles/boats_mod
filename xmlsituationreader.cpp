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

#include "model/situationmodel.h"
#include "model/trackmodel.h"
#include "model/boatmodel.h"

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
            if (name() == "xmlsituation" && attributes().value("version") == "1.0")
                readSituation();
            else
                raiseError(QObject::tr("The file is not an xmlsituation version 1.0 file."));
        }
    }
    return !error();
}

QString XmlSituationReader::readUnknownElement() {
    QString elem;
    elem.append("<").append(name()).append(">");
    while (!atEnd()) {
        elem += readElementText();

        if (isEndElement()) {
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
            if (name() == "layline")
                m_situation->setLaylineAngle(readElementText().toFloat());
            if (name() == "track")
                readTrack(m_situation);
            else
                readUnknownElement();
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
            else if (name() == "boat")
                readBoat(situation, track);
            else
                readUnknownElement();
        }
    }
}

void XmlSituationReader::readBoat(SituationModel *situation, TrackModel *track) {
    QPointF pos;
    qreal heading;
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
                readUnknownElement();
        }
    }
    AddBoatUndoCommand *command = new AddBoatUndoCommand(track, pos, heading);
    situation->undoStack()->push(command);
    BoatModel *boat = command->boat();
}
