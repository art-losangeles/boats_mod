//
// C++ Interface: boats
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
        rib,
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
