#ifndef MARKMODEL_H
#define MARKMODEL_H

#include <QtGui>

#include "model/positionmodel.h"

class SituationModel;

/**
    \class MarkModel

    \brief The Model for a Mark

    The class represents the Model for a Mark, according to an
    Observer Pattern.

    MarkModel inherits PositionModel and contains data which represents
    a mark, like its color, and whether the zone is displayed.

    \sa SituationModel

*/

class MarkModel : public PositionModel {
        Q_OBJECT
    public:
        MarkModel(SituationModel *situation, QObject *parent = 0);
        ~MarkModel();

        // Setters and Getters for Model Data
        QColor color() const { return m_color; }
        void setColor(const QColor& theValue, bool update = false);

        bool zone() const { return m_zone; }
        void setZone(const bool theValue, bool update = false);

        // Setters and Getters for Non model Data
        SituationModel* situation() const { return m_situation; }

        int length() const { return m_length; }
        void setLength(const int theValue, bool update = false);

    signals:
        void colorChanged(QColor color);
        void zoneChanged(bool zone);
        void lengthChanged(int length);

    private:
        // Model Data
        /// \a m_color holds the color of the Mark
        QColor m_color;

        /// \a m_zone holds whether the zone of the Mark is displayed
        bool m_zone;

        // Non model Data
        /// \a m_situation keeps a pointer to the SituationModel to which
        /// it belongs
        SituationModel *m_situation;

        /// \a m_length keeps the length of Main Series of the Scenario
        int m_length;
};

#endif
