//
// C++ Interface: boats
//
// Description:
//
//
// Author: Thibaut GRIDEL <tgridel@free.fr>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef BOATS_H
#define BOATS_H

#include <QtGui>

#define ENUM_NAME(o,e,v) (o::staticMetaObject.enumerator(o::staticMetaObject.indexOfEnumerator(#e)).valueToKey((v)))

#define ENUM_VALUE(o,e,n) (o::staticMetaObject.enumerator(o::staticMetaObject.indexOfEnumerator(#e)).keyToValue((n)))

#define ENUM_SIZE(o,e) (o::staticMetaObject.enumerator(o::staticMetaObject.indexOfEnumerator(#e)).keyCount())

class Boats {
    Q_GADGET

public:
    enum Series {
        keelboat,
        laser,
        optimist,
        tornado,
        unknown
    };
    Q_ENUMS (Series);

    static QList<QString> seriesList();

private:
    static QList<QString> m_seriesList;

};

#endif
