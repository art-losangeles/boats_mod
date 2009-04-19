//
// C++ Implementation: BoatApplication
//
// Description:
//
//
// Author: Thibaut GRIDEL <tgridel@free.fr>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "boatapplication.h"
#include "mainwindow.h"

bool BoatApplication::event(QEvent *event) {
        switch (event->type()) {
        case QEvent::FileOpen:
            if (m_window) {
                m_window->openFile(static_cast<QFileOpenEvent *>(event)->file());
                return true;
            }
            break;
        default:
            break;
        }
        return QApplication::event(event);
    }
