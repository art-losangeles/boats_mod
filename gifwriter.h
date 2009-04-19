//
// C++ Interface: gifwriter
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
#ifndef GIFWRITER_H
#define GIFWRITER_H

#include <QIODevice>
#include <QImage>

#include <gif_lib.h>

class GifWriter {
public:
    GifWriter();

    void setDevice (QIODevice *device) { m_device = device; }
    QIODevice *device() const { return m_device; }

    void setColorMap(QImage &image) { m_colormap = image.colorTable(); }
    QVector<QRgb> colormap() const { return m_colormap; }

    bool write(QList<QImage*> imageList);
    int writeData(const GifByteType *bytes, int size);

private:
    QIODevice *m_device;
    QVector<QRgb> m_colormap;
};

#endif // GIFWRITER_H
