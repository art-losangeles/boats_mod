//
// C++ Interface: gifwriter
//
// Description:
//
//
// Author: Thibaut GRIDEL <tgridel@free.fr>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
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
