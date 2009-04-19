//
// C++ Implementation: Trace
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
#include <iostream>
#include <iomanip>

#include "trace.h"

int TraceBlock::indent = 0;

TraceBlock::TraceBlock(bool active, QString value, int *duration)
    : m_active(active),
    m_value(value.toStdString()),
    m_duration(duration) {
    if (m_active) {
        indent++;
        std::cout << std::string(indent,' ') << "enter "
        << m_value << std::endl;
        m_time.start();
    }
}

TraceBlock::~TraceBlock() {
    if (m_active) {
        int duration = m_time.elapsed();
        std::cout << std::string(indent,' ') << "exit  "
        << m_value << " " << duration << std::endl;
        indent--;
        if (m_duration) {
            *m_duration += duration;
        }
    }
}
