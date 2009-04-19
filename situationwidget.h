//
// C++ Interface: situationwidget
//
// Description:
//
//
// Author: Thibaut GRIDEL <tgridel@free.fr>
//
// Copyright (c) 2008-2009 Thibaut GRIDEL
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
#ifndef SITUATIONWIDGET_H
#define SITUATIONWIDGET_H

#include <QtGui>

#include "tracktablemodel.h"

class SituationModel;

class SituationWidget : public QTabWidget {
    Q_OBJECT
    public:
        SituationWidget(QWidget *parent = 0);
        ~SituationWidget() {}

        void update();

    public slots:
        void setSituation(SituationModel *situation);
        void unSetSituation();

        // set the model from widgets
        void setTitle(QString title);
        void setRules(QString rules);
        void setShowLayline(bool);
        void setLayline(int angle);
        void setSeries(int series);
        void setLength(int length);
        void setAbstract();
        void setDescription();

        // set the widget from the model
        void updateAbstract(const QString abstract);
        void updateDescription(const QString description);

    private:
        SituationModel *m_situation;

        QFrame *scenarioFrame;
        QVBoxLayout *scenarioLayout;

        QGroupBox *optionsGroup;
        QComboBox *seriesCombo;
        QCheckBox *laylineCheck;
        QSpinBox *laylineSpin;
        QSpinBox *lengthSpin;
        QFormLayout *optionsForm;

        QGroupBox *trackGroup;
        TrackTableModel *trackTableModel;
        QTableView *trackTableView;
        QGridLayout *trackLayout;

        QFrame *descriptionFrame;
        QLineEdit *titleEdit;
        QLineEdit *rulesEdit;
        QPlainTextEdit *abstractEdit;
        QPlainTextEdit *descriptionEdit;
        QGridLayout *descriptionGrid;
        QFormLayout *descriptionForm;

};

#endif
