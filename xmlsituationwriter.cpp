//
// C++ Implementation: XmlSituationWriter
//
// Description:
//
//
// Author: Thibaut GRIDEL <tgridel@free.fr>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "xmlsituationwriter.h"

#include "model/situationmodel.h"
#include "model/trackmodel.h"
#include "model/boatmodel.h"
#include "model/markmodel.h"

XmlSituationWriter::XmlSituationWriter(SituationModel *situation)
        : m_situation(situation) {
    setAutoFormatting(true);
}

XmlSituationWriter::~XmlSituationWriter() {}

bool XmlSituationWriter::writeFile(QIODevice *device) {
    setDevice(device);

    writeStartDocument();
    writeDTD("<!DOCTYPE xmlscenario>");
    writeStartElement("xmlscenario");
    writeAttribute("version", "1.0");
    if (!m_situation->title().isEmpty()) {
        writeTextElement("title", m_situation->title());
    }
    if (!m_situation->rules().isEmpty()) {
        writeTextElement("rules", m_situation->rules());
    }
    if (!m_situation->abstract().isEmpty()) {
        writeTextElement("abstract", m_situation->abstract());
    }
    if (!m_situation->description().isEmpty()) {
        writeTextElement("description", m_situation->description());
    }
    writeTextElement("series",ENUM_NAME(Boats, Series, m_situation->situationSeries()));
    writeTextElement("layline",QString::number(m_situation->laylineAngle()));
    foreach (const QString discarded, m_situation->discardedXml())
        writeUnknownElement(discarded);
    foreach (const MarkModel *mark, m_situation->marks())
        writeMark(mark);
    foreach (const TrackModel *track, m_situation->tracks())
        writeTrack(track);

    writeEndDocument();
    return true;
}

void XmlSituationWriter::writeTrack(const TrackModel *track) {
    writeStartElement("track");
    writeTextElement("color",track->color().name());
    writeTextElement("series",ENUM_NAME(Boats, Series, track->series()));
    foreach (const QString discarded, track->discardedXml())
        writeUnknownElement(discarded);
    foreach (const BoatModel *boat, track->boats())
        writeBoat(boat);
    writeEndElement();
}

void XmlSituationWriter::writeBoat(const BoatModel *boat) {
    writeStartElement("boat");
    writeTextElement("x",QString::number(boat->position().x()));
    writeTextElement("y",QString::number(boat->position().y()));
    writeTextElement("heading",QString::number(boat->heading()));
    foreach (const QString discarded, boat->discardedXml())
        writeUnknownElement(discarded);
    writeEndElement();
}

void XmlSituationWriter::writeMark(const MarkModel *mark) {
    writeStartElement("mark");
    writeTextElement("x",QString::number(mark->position().x()));
    writeTextElement("y",QString::number(mark->position().y()));
    writeTextElement("color",mark->color().name());
    writeTextElement("zone",QString::number(mark->zone()));
    writeTextElement("length",QString::number(mark->length()));
    foreach (const QString discarded, mark->discardedXml())
        writeUnknownElement(discarded);
    writeEndElement();
}

void XmlSituationWriter::writeUnknownElement(const QString &discarded) {
    if (!discarded.isEmpty()) {
        QXmlStreamReader reader(discarded);
        while (!reader.atEnd()) {
            reader.readNext();
            if (reader.isStartElement()
                || reader.isEndElement()
                || reader.isCharacters()) {
                writeCurrentToken(reader);
            }
        }
    }
}
