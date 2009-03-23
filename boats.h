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

#define FLAG_NAME(o,e,v) (o::staticMetaObject.enumerator(o::staticMetaObject.indexOfEnumerator(#e)).valueToKeys((v)))

#define FLAG_VALUE(o,e,n) (o::staticMetaObject.enumerator(o::staticMetaObject.indexOfEnumerator(#e)).keysToValue((n)))

class Boats {
    Q_GADGET

public:
    enum Series {
        keelboat,
        laser,
        optimist,
        tornado,
        startboat,
        unknown
    };
    Q_ENUMS (Series);

    static QList<QString> seriesList();

    enum Overlap {
        none = 0x0,
        starboard = 0x1,
        port = 0x2
    };
    Q_ENUMS (Overlap);
    Q_DECLARE_FLAGS(Overlaps, Overlap)

private:
    static QList<QString> m_seriesList;

};

Q_DECLARE_OPERATORS_FOR_FLAGS(Boats::Overlaps)

#endif
