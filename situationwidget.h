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

        // set the model from widgets
        void setTitle(QString title);
        void setRules(QString rules);
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
        QSpinBox *laylineSpin;
        QSpinBox *lengthSpin;
        QFormLayout *scenarioForm;

        QGroupBox *descriptionGroup;
        QPlainTextEdit *abstractEdit;
        QPlainTextEdit *descriptionEdit;
        QGridLayout *descriptionLayout;

        QVBoxLayout *situationLayout;
};

#endif
