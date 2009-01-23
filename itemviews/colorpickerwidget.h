//
// C++ Interface: colorpickerwidget
//
// Description:
//
//
// Author: Thibaut GRIDEL <tgridel@free.fr>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef COLORPICKERWIDGET_H
#define COLORPICKERWIDGET_H

#include <QtGui>

class ColorPickerWidget : public QWidget
{
     Q_OBJECT

    public:
        ColorPickerWidget(QWidget *parent = 0);
        ~ColorPickerWidget() {}

        void setColor(QColor color);
        const QColor color() const { return m_color; }

    signals:
        void pickColor();
        void editingFinished();

    public slots:
        void pick();

    private:
        QColor m_color;
};

#endif
