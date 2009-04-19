//
// C++ Interface: XmlSituationWriter
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
#ifndef XMLSITUATIONWRITER_H
#define XMLSITUATIONWRITER_H

#include <QXmlStreamWriter>

class SituationModel;
class TrackModel;
class BoatModel;
class MarkModel;

class XmlSituationWriter : public QXmlStreamWriter {
    public:
        XmlSituationWriter(SituationModel *situation);
        ~XmlSituationWriter();

        bool writeFile(QIODevice *device);

    private:
        void writeTrack(const TrackModel *track);
        void writeBoat(const BoatModel *boat);
        void writeMark(const MarkModel *mark);
        void writeUnknownElement(const QString &discarded);

        SituationModel *m_situation;
};

#endif
