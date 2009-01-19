//
// C++ Implementation: boats
//
// Description:
//
//
// Author: Thibaut GRIDEL <tgridel@free.fr>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "boats.h"


QList<QString> Boats::m_seriesList;

QList<QString> Boats::seriesList() {
    if (!m_seriesList.size()) {
        for (int i=0; i< ENUM_SIZE(Boats, Series)-1; i++) {
            m_seriesList << ENUM_NAME(Boats, Series, i);
        }
    }
    return m_seriesList;
}
