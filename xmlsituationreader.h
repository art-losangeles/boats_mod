//
// C++ Interface: XmlSituationReader
//
// Description:
//
//
// Author: Thibaut GRIDEL <tgridel@free.fr>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef XMLSITUATIONREADER_H
#define XMLSITUATIONREADER_H

#include <QXmlStreamReader>

#include "boats.h"

class SituationModel;
class TrackModel;
class BoatModel;

class XmlSituationReader : public QXmlStreamReader {
    public:
        XmlSituationReader(SituationModel *situation);
        ~XmlSituationReader();

        bool read(QIODevice *device);

    private:
        QString readUnknownElement();
        void readSituation();
        void readTrack(SituationModel *situation);
        void readBoat(SituationModel *situation, TrackModel *track);
        void readMark(SituationModel *situation);

        Boats::Series series(const QString series);

        SituationModel *m_situation;
};

#endif
