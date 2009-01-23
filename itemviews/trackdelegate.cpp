//
// C++ Implementation: trackdelegate
//
// Description:
//
//
// Author: Thibaut GRIDEL <tgridel@free.fr>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
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
            painter->drawText(option.rect, Qt::AlignVCenter, ENUM_NAME(Boats, Series, series));
            }
            break;
        default:
            QItemDelegate::paint(painter, option, index);
            break;
    }
    if (option.state & QStyle::State_Selected)
        painter->fillRect(option.rect, option.palette.highlight());
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
                    this, SLOT(commitAndCloseColor()));
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
