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

XmlSituationWriter::XmlSituationWriter(SituationModel *situation)
        : m_situation(situation) {
    setAutoFormatting(true);
}

XmlSituationWriter::~XmlSituationWriter() {}

bool XmlSituationWriter::writeFile(QIODevice *device) {
    setDevice(device);

    writeStartDocument();
    writeDTD("<!DOCTYPE xmlsituation>");
    writeStartElement("xmlsituation");
    writeAttribute("version", "1.0");
    foreach (TrackModel *track, m_situation->m_tracks)
        writeTrack(track);

    writeEndDocument();
    return true;
}

void XmlSituationWriter::writeTrack(TrackModel *track) {
    writeStartElement("track");
    writeTextElement("color",track->color().name());
    foreach (BoatModel *boat, track->m_boats)
        writeBoat(boat);
    writeEndElement();
}

void XmlSituationWriter::writeBoat(BoatModel *boat) {
    writeStartElement("boat");
    writeTextElement("x",QString::number(boat->position().x()));
    writeTextElement("y",QString::number(boat->position().y()));
    writeTextElement("heading",QString::number(boat->heading()));
    writeEndElement();
}
