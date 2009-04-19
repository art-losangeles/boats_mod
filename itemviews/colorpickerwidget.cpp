//
// C++ Implementation: colorpickerwidget
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
#include <iostream>

#include "colorpickerwidget.h"

#include "commontypes.h"

extern int debugLevel;

ColorPickerWidget::ColorPickerWidget(QWidget *parent)
    : QWidget(parent) {
}

void ColorPickerWidget::setColor(QColor color) {
    if (color != m_color) {
        m_color = color;
        QColorDialog::setCustomColor(0, color.rgb());
        pick();
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
