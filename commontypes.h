//
// C++ Interface: commontypes
//
// Description:
//
//
// Author: Thibaut GRIDEL <tgridel@free.fr>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef COMMONTYPES_H
#define COMMONTYPES_H

#define VERSION "200811-rc1"

typedef enum {
    KEELBOAT,
    LASER,
    OPTIMIST,
    TORNADO,
    UNKNOWN
} Series;

typedef enum {
    MODEL,
    VIEW,
    COMMAND,
    ANIMATION
} DebugTraces;

#endif
