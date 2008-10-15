//
// C++ Interface: Trace
//
// Description:
//
//
// Author: Thibaut GRIDEL <tgridel@free.fr>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
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
