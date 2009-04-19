//
// C++ Interface: Trace
//
// Description:
//
//
// Author: Thibaut GRIDEL <tgridel@free.fr>
//
// Copyright (c) 2008 Thibaut GRIDEL
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
#ifndef TRACE_H
#define TRACE_H

#include <QString>
#include <QTime>

class TraceBlock {
    public:
        TraceBlock(bool active, QString value, int *duration = 0);
        ~TraceBlock();

    private:
        static int indent;
        bool m_active;
        std::string m_value;
        QTime m_time;
        int *m_duration;
};
#endif
