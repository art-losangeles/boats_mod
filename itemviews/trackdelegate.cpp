//
// C++ Implementation: trackdelegate
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
#include <iostream>

#include "trackdelegate.h"

#include "colorpickerwidget.h"

#include "commontypes.h"
#include "boats.h"

extern int debugLevel;

void TrackDelegate::paint(QPainter *painter,
    const QStyleOptionViewItem &option,
    const QModelIndex &index) const {

    painter->save();

    drawBackground(painter, option, index);

    QRect checkRect;
    Qt::CheckState checkState = Qt::Unchecked;
    QVariant value = index.data(Qt::CheckStateRole);
    if (value.isValid()) {
        checkState = static_cast<Qt::CheckState>(value.toInt());
        checkRect = check(option, option.rect, value);
    }
    drawCheck(painter, option, checkRect, checkState);

    switch (index.column()) {
        case TRACK_COLOR: {
            QColor trackColor = qVariantValue<QColor>(index.data());
            painter->setPen(Qt::NoPen);
            painter->setBrush(trackColor);
            qreal height = option.rect.height() / 8.0;
            qreal width = option.rect.width() / 8.0;
            QRectF rect(option.rect);
            rect.adjust(width, height, -width, -height);
            painter->drawRoundedRect(rect, width, height);
            }
            break;
        case TRACK_SERIES: {
            int series = qVariantValue<int>(index.data());
            drawDisplay(painter, option, option.rect, ENUM_NAME(Boats, Series, series));
            }
            break;
        default:
            QItemDelegate::paint(painter, option, index);
            break;
    }

    drawFocus(painter, option, option.rect);
    painter->restore();
}

QSize TrackDelegate::sizeHint(const QStyleOptionViewItem &option,
    const QModelIndex &index) const {
    return QItemDelegate::sizeHint(option, index);
}

QWidget * TrackDelegate::createEditor(QWidget *parent,
    const QStyleOptionViewItem &option,
    const QModelIndex &index) const {
    if (debugLevel & 1 << DELEGATE) std::cout << "createEditor " << index.column() << std::endl;

    switch (index.column()) {
        case TRACK_COLOR: {
            ColorPickerWidget *editor = new ColorPickerWidget(parent);
            connect(editor, SIGNAL(editingFinished()),
                    this, SLOT(commitAndCloseColor()), Qt::QueuedConnection);
            return editor;
            }
            break;
        case TRACK_SERIES: {
            QComboBox *editor = new QComboBox(parent);
            editor->addItems(Boats::seriesList());
            connect(editor, SIGNAL(activated(int)),
                    this, SLOT(commitAndCloseSeries()));
            return editor;
            }
            break;
        default:
            return QItemDelegate::createEditor(parent, option, index);
            break;
    }
}

void TrackDelegate::setEditorData(QWidget *editor,
    const QModelIndex &index) const {
    if (debugLevel & 1 << DELEGATE) std::cout << "setEditorData " << index.column() << std::endl;

    switch (index.column()) {
        case TRACK_COLOR: {
            ColorPickerWidget *colorEditor = getColorEditor(editor);
            QColor color = qVariantValue<QColor>(index.data());
            colorEditor->setColor(color);
            }
            break;
        case TRACK_SERIES: {
            QComboBox *seriesEditor = getSeriesEditor(editor);
            int series = qVariantValue<int>(index.data());
            seriesEditor->setCurrentIndex(series);
            }
            break;
        default:
            QItemDelegate::setEditorData(editor, index);
            break;
    }
}

void TrackDelegate::setModelData(QWidget *editor,
    QAbstractItemModel *model,
    const QModelIndex &index) const {
    if (debugLevel & 1 << DELEGATE) std::cout << "setModelData " << index.column() << std::endl;

    switch (index.column()) {
        case TRACK_COLOR: {
            ColorPickerWidget *colorEditor = getColorEditor(editor);
            QColor color = colorEditor->color();
            model->setData(index, qVariantFromValue(color));
            }
            break;
        case TRACK_SERIES: {
            QComboBox *seriesEditor = getSeriesEditor(editor);
            int series = seriesEditor->currentIndex();
            model->setData(index, qVariantFromValue(series));
            }
            break;
        default:
            QItemDelegate::setModelData(editor, model, index);
            break;
    }
}

void TrackDelegate::commitAndCloseColor() {
    if (debugLevel & 1 << DELEGATE) std::cout << "commitAndCloseColor" << std::endl;

    ColorPickerWidget *colorEditor = getColorEditor(sender());
    emit commitData(colorEditor);
    emit closeEditor(colorEditor);
}

void TrackDelegate::commitAndCloseSeries() {
    if (debugLevel & 1 << DELEGATE) std::cout << "commitAndCloseSeries " << std::endl;

    QComboBox *seriesEditor = getSeriesEditor(sender());
    emit commitData(seriesEditor);
    emit closeEditor(seriesEditor);
}

ColorPickerWidget * TrackDelegate::getColorEditor(QObject *editor) const {
    return qobject_cast<ColorPickerWidget *>(editor);
}

QComboBox * TrackDelegate::getSeriesEditor(QObject *editor) const {
    return qobject_cast<QComboBox *>(editor);
}
