//
// C++ Implementation: tracktablemodel
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

#include "tracktablemodel.h"

#include "commontypes.h"
#include "boats.h"
#include "model/situationmodel.h"
#include "model/trackmodel.h"
#include "undocommands.h"

extern int debugLevel;

TrackTableModel::TrackTableModel(SituationModel *situation, QObject *parent)
        : QAbstractTableModel(parent),
        m_situation(situation) {
    if (debugLevel & 1 << MODEL) std::cout << "new tracktable " << this << std::endl;
}

TrackTableModel::~TrackTableModel() {
    if (debugLevel & 1 << MODEL) std::cout << "end tracktable " << this << std::endl;
}

QVariant TrackTableModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
        return QVariant();

    if (index.row() >= m_situation->size() || index.row() < 0)
        return QVariant();

    if (role == Qt::DisplayRole) {
        switch (index.column()) {
            case TRACK_COLOR:
                return m_situation->tracks()[index.row()]->color();
                break;
            case TRACK_SERIES:
                return m_situation->tracks()[index.row()]->series();
                break;
            default:
                return QVariant();
                break;
        }
    }
    return QVariant();
}

QVariant TrackTableModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        switch (section) {
            case TRACK_COLOR:
                return tr("Color");

            case TRACK_SERIES:
                return tr("Series");

            default:
                return QVariant();
        }
    }
    return QVariant();
}

Qt::ItemFlags TrackTableModel::flags(const QModelIndex &index) const {
    Q_UNUSED(index);
    return Qt::ItemIsEnabled|Qt::ItemIsEditable;
}

bool TrackTableModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    Q_UNUSED(role);
    if (!index.isValid())
        return false;

    if (index.row() >= m_situation->size() || index.row() < 0)
        return false;

    switch (index.column()) {
        case TRACK_COLOR:
            if (qVariantCanConvert<QColor>(value)) {
                QColor newValue = qVariantValue<QColor>(value);
                m_situation->tracks()[index.row()]->setColor(newValue, true);
                emit dataChanged(index,index);
                return true;
            }
            break;
        case TRACK_SERIES:
            if (qVariantCanConvert<int>(value)) {
                int newValue = qVariantValue<int>(value);
                if (newValue >= 0 && newValue < Boats::unknown) {
                    Boats::Series seriesValue = (Boats::Series)newValue;
                    TrackModel *track = m_situation->tracks()[index.row()];
                    if (seriesValue != track->series()) {
                        m_situation->undoStack()->push(new SetSeriesUndoCommand(track, seriesValue));
                    }
                    emit dataChanged(index,index);
                    return true;
                }
            }
            break;
        default:
            return false;
            break;
    }
    return false;
}

void TrackTableModel::addTrack(TrackModel *track) {
    int order = track->order();
    connect(track, SIGNAL(colorChanged(QColor)),
            this, SLOT(updateTrack()));
    connect(track, SIGNAL(seriesChanged(Boats::Series)),
            this, SLOT(updateTrack()));
    beginInsertRows(QModelIndex(), order, order);
    endInsertRows();
}

void TrackTableModel::updateTrack() {
    TrackModel *track = (TrackModel*) sender();
    int order = track->order();
    for (int i = TRACK_COLOR; i <= TRACK_SERIES; i++) {
        QModelIndex ind = index(i, order);
        emit dataChanged(ind, ind);
    }
}

void TrackTableModel::deleteTrack(TrackModel *track) {
    int order = track->order();
    disconnect(track,0,this,0);
    beginRemoveRows(QModelIndex(), order, order);
    endRemoveRows();
}
