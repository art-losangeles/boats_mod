//
// C++ Interface: situationwidget
//
// Description:
//
//
// Author: Thibaut GRIDEL <tgridel@free.fr>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef SITUATIONWIDGET_H
#define SITUATIONWIDGET_H

#include <QtGui>

#include "tracktablemodel.h"

class SituationModel;

class SituationWidget : public QWidget {
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

        QGroupBox *scenarioGroup;
        QLineEdit *titleEdit;
        QLineEdit *rulesEdit;
        QComboBox *seriesCombo;
        QCheckBox *laylineCheck;
        QSpinBox *laylineSpin;
        QSpinBox *lengthSpin;
        QPlainTextEdit *abstractEdit;
        QPlainTextEdit *descriptionEdit;
        QGridLayout *scenarioGrid;
        QFormLayout *scenarioForm;

        QGroupBox *trackGroup;
        TrackTableModel *trackTableModel;
        QTableView *trackTableView;
        QGridLayout *trackLayout;

        QVBoxLayout *situationLayout;
};

#endif
