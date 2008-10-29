//
// C++ Interface: MarkGraphicsItem
//
// Description:
//
//
// Author: Thibaut GRIDEL <tgridel@free.fr>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef MARK_H
#define MARK_H

#include <QGraphicsItem>
#include <QObject>
#include <QColor>

class MarkModel;

class MarkGraphicsItem : public QObject, public QGraphicsItem {
        Q_OBJECT
    public:
        MarkGraphicsItem(MarkModel *mark = 0, QGraphicsItem *parent = 0);
        ~MarkGraphicsItem();

        MarkModel* mark() const { return m_mark; }

        QRectF boundingRect() const;
        QPainterPath shape() const;
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                   QWidget *widget);
        int type() const;

    public slots:
        void setPosition(QPointF position);
        void setOrder(int value);
        void setColor(QColor value);
        void deleteItem(MarkModel *mark);

    private:
        MarkModel *m_mark;
        QColor m_color;
        bool m_selected;
        int m_order;
};

#endif
