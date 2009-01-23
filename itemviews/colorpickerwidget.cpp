//
// C++ Implementation: colorpickerwidget
//
// Description:
//
//
// Author: Thibaut GRIDEL <tgridel@free.fr>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include <iostream>

#include "colorpickerwidget.h"

#include "commontypes.h"

extern int debugLevel;

ColorPickerWidget::ColorPickerWidget(QWidget *parent)
    : QWidget(parent) {
    connect(this, SIGNAL(pickColor()),
            this, SLOT(pick()), Qt::QueuedConnection);
}

void ColorPickerWidget::setColor(QColor color) {
    if (color != m_color) {
        m_color = color;
        QColorDialog::setCustomColor(0, color.rgb());
        emit pickColor();
    }
}

void ColorPickerWidget::pick() {
    if (debugLevel & 1 << DELEGATE) std::cout << "pick" << std::endl;
    // get the new color from dialog
    QColor newColor(QColorDialog::getColor(m_color));
    if (newColor.isValid()) {
        m_color = newColor;
    }
    emit editingFinished();
}