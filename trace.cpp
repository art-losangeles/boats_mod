//
// C++ Implementation: Trace
//
// Description:
//
//
// Author: Thibaut GRIDEL <tgridel@free.fr>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
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
