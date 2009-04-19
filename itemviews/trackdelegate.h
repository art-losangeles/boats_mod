//
// C++ Interface: trackdelegate
//
// Description:
//
//
// Author: Thibaut GRIDEL <tgridel@free.fr>
//
// Copyright (c) 2009 Thibaut GRIDEL
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
