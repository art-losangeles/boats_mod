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
        void setLayline(int angle);
        void setSeries(int series);
        void setAbstract();
        void setDescription();

    private:
        SituationModel *m_situation;

        QGroupBox *scenarioGroup;
        QLineEdit *title;
        QLineEdit *rules;
        QComboBox *seriesCombo;
        QSpinBox *laylineSpin;
        QFormLayout *scenarioForm;

        QGroupBox *descriptionGroup;
        QTextEdit *abstract;
        QTextEdit *description;
        QGridLayout *descriptionLayout;

        QVBoxLayout *situationLayout;
};

#endif
