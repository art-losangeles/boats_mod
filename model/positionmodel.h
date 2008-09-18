//
// C++ Interface: positionmodel
//
// Description:
//
//
// Author: Thibaut GRIDEL <tgridel@free.fr>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef POSITIONMODEL_H
#define POSITIONMODEL_H

#include <QtGui>

class PositionModel : public QObject {
        Q_OBJECT
    public:
        PositionModel(QObject *parent = 0);
        ~PositionModel();

        QPointF position() const { return m_position; }
        int order() const { return m_order; }
        virtual void setPosition(const QPointF& theValue, bool update = false);
        void setOrder(const int theValue, bool update = false);
        QStringList discardedXml() const { return m_discardedXml; };
        void appendDiscardedXml(const QString& theValue);

    signals:
        void positionChanged(QPointF position);
        void orderChanged(int order);

    private:
        QPointF m_position;
        int m_order;
        QStringList m_discardedXml;
};

#endif
