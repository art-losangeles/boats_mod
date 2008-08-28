#ifndef MARKMODEL_H
#define MARKMODEL_H

#include <QtGui>

class SituationModel;

class MarkModel : public QObject {
        Q_OBJECT
    public:
        MarkModel(SituationModel *situation, QObject *parent = 0);
        ~MarkModel();

        SituationModel* situation() const { return m_situation; }
        QPointF position() const { return m_position; }
        int order() const { return m_order; }
        QStringList discardedXml() const { return m_discardedXml; };
        void appendDiscardedXml(const QString& theValue);

        void setPosition(const QPointF& theValue, bool update = false);
        void setOrder(const int theValue, bool update = false);

    signals:
        void positionChanged(QPointF position);
        void orderChanged(int order);

    private:
        SituationModel *m_situation;
        QPointF m_position;
        int m_order;
        QStringList m_discardedXml;
};

#endif
