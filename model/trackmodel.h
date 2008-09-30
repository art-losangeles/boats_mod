//
// C++ Interface: trackmodel
//
// Description:
//
//
// Author: Thibaut GRIDEL <tgridel@free.fr>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef TRACKMODEL_H
#define TRACKMODEL_H

#include <QtGui>

class BoatModel;
class SituationModel;

class TrackModel : public QObject {
        Q_OBJECT
    friend class XmlSituationReader;
    friend class XmlSituationWriter;
    friend class SituationScene;
    friend class TrackGraphicsItem;
    public:
        TrackModel(SituationModel* situation = 0, QObject *parent = 0);
        ~TrackModel();

        BoatModel * addBoat(BoatModel *boat, int order = 0);
        int deleteBoat(BoatModel *boat);

        void displayBoats();
        void hideBoats();

        SituationModel* situation() const { return m_situation; }
        QColor color() const { return m_color;};
        int size() const { return m_boats.size();};
        const QList<BoatModel*> boats() const { return m_boats; };
        const QPainterPath path() const { return m_path; };

        void setColor(const QColor& theValue, bool update = false);
        QStringList discardedXml() const { return m_discardedXml; };
        void appendDiscardedXml(const QString& theValue);

        void changingTrack(TrackModel *track);

    signals:
        void colorChanged(QColor color);
        void trackChanged(TrackModel *track);

    protected:
        QList<BoatModel*> m_boats;

    private:
        SituationModel *m_situation;
        QColor m_color;
        QPainterPath m_path;
        QStringList m_discardedXml;
};

#endif
