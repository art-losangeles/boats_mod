//
// C++ Interface: trackwidget
//
// Description:
//
//
// Author: Thibaut GRIDEL <tgridel@free.fr>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef TRACKWIDGET_H
#define TRACKWIDGET_H

#include <QToolBar>
#include <QPushButton>
#include <QSpinBox>

class TrackWidget : public QToolBar {
        Q_OBJECT
    public:
        TrackWidget(QWidget *parent = 0);
        ~TrackWidget();

    private:
        QPushButton *m_addbutton;
        QSpinBox *m_headingbox;
};

#endif
