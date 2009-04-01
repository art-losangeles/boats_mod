//
// C++ Implementation: gifwriter
//
// Description:
//
//
// Author: Thibaut GRIDEL <tgridel@free.fr>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "gifwriter.h"

#include <QImage>
#include <QColor>
#include <QtCore>

GifWriter::GifWriter() {
}

int GifWriter::writeData(const GifByteType *bytes, int size) {
    return device()->write((char *)bytes, size);
}

static int myOutputFunc(GifFileType * fileType, const GifByteType * bytes, int size) {
    GifWriter *writer = reinterpret_cast<GifWriter*>(fileType->UserData);
    return writer->writeData(bytes, size);
}

bool GifWriter::write(QList<QImage*> imageList) {
    GifFileType *fileType;
    int retrn;
    QImage *image = imageList.first();

    fileType = EGifOpen((void*) this, myOutputFunc);
    EGifSetGifVersion("89a");

    int colors = 256;
    ColorMapObject * cmap = MakeMapObject(colors, NULL);
    for (int i = 0; i< qMin(colors, m_colormap.size()); i++) {
        QRgb color = m_colormap[i];
        cmap->Colors[i].Blue = qBlue(color);
        cmap->Colors[i].Green = qGreen(color);
        cmap->Colors[i].Red = qRed(color);
    }

    retrn = EGifPutScreenDesc(fileType, image->width(), image->height(), 8, 0, cmap);
    FreeMapObject(cmap);

    char nameExtension[11] = { 'N','E','T','S','C','A','P','E','2','.','0' };
    char loopExtension[3] = { 1, 0, 0 };
    retrn = EGifPutExtensionFirst(fileType, 0xFF, 11, &nameExtension);
    retrn = EGifPutExtensionNext(fileType, 0xFF, 3, &loopExtension);
    retrn = EGifPutExtensionLast(fileType, 0xFF, 0, NULL);
    retrn = EGifPutComment(fileType, "Boat Scenario http://boats.berlios.de");

    char gifExtension[4] = { 0, 8, 0, 0 };
    foreach (image, imageList) {
        if (image == imageList.last()) {
            gifExtension[1] = 0;
            gifExtension[2] = 2;
        }
        retrn = EGifPutExtension(fileType, 0xF9, 4, &gifExtension);
        retrn = EGifPutImageDesc(fileType, 0, 0, image->width(), image->height(), 0, NULL);
        for (int i = 0; i < image->height(); i++) {
            retrn &= EGifPutLine(fileType, (GifPixelType*)image->scanLine(i), image->width());
        }
    }

    retrn = EGifCloseFile(fileType);
    return true;
}
