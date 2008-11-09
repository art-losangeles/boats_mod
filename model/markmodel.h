#ifndef MARKMODEL_H
#define MARKMODEL_H

#include <QtGui>

#include "model/positionmodel.h"

class SituationModel;

class MarkModel : public PositionModel {
        Q_OBJECT
    public:
        MarkModel(SituationModel *situation, QObject *parent = 0);
        ~MarkModel();

        SituationModel* situation() const { return m_situation; }
        QColor color() const { return m_color; }
        void setColor(const QColor& theValue, bool update = false);

        bool zone() const { return m_zone; }
        void setZone(const bool theValue, bool update = false);

    signals:
        void colorChanged(QColor color);
        void zoneChanged(bool zone);

    private:
        SituationModel *m_situation;
        QColor m_color;
        bool m_zone;
};

#endif
