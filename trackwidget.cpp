//
// C++ Implementation: trackwidget
//
// Description:
//
//
// Author: Thibaut GRIDEL <tgridel@free.fr>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include <QAction>

#include "trackwidget.h"

TrackWidget::TrackWidget(QWidget *parent)
        : QToolBar(parent),
        m_addbutton(new QPushButton("Add After", parent)),
        m_headingbox(new QSpinBox(parent)) {

    m_addbutton->setCheckable(true);
    m_addbutton->show();
    QAction *addaction = addWidget(m_addbutton);
    addaction->setVisible(true);

    m_headingbox->setRange(0, 359);
    m_headingbox->setWrapping(true);
    m_headingbox->show();
    addWidget(m_headingbox);

}


TrackWidget::~TrackWidget() {
    delete m_addbutton;
    delete m_headingbox;
}
