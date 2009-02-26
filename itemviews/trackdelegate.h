//
// C++ Interface: trackdelegate
//
// Description:
//
//
// Author: Thibaut GRIDEL <tgridel@free.fr>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef TRACKDELEGATE_H
#define TRACKDELEGATE_H

#include <QtGui>

class ColorPickerWidget;

class TrackDelegate : public QItemDelegate
{
     Q_OBJECT

    public:
        TrackDelegate(QWidget *parent = 0) : QItemDelegate(parent) {}

        void paint(QPainter *painter, const QStyleOptionViewItem &option,
        const QModelIndex &index) const;

        QSize sizeHint(const QStyleOptionViewItem &option,
        const QModelIndex &index) const;

        QWidget *createEditor(QWidget *parent,
        const QStyleOptionViewItem &option,
        const QModelIndex &index) const;

        void setEditorData(QWidget *editor, const QModelIndex &index) const;

        void setModelData(QWidget *editor, QAbstractItemModel *model,
        const QModelIndex &index) const;

    private slots:
        void commitAndCloseColor();
        void commitAndCloseSeries();

    private:
        ColorPickerWidget * getColorEditor(QObject *editor) const;
        QComboBox * getSeriesEditor(QObject *editor) const;
};

#endif
