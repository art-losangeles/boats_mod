//
// C++ Implementation: BoatApplication
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
