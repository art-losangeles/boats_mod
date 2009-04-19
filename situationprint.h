//
// C++ Interface: situationprint
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
#ifndef SITUATIONPRINT_H
#define SITUATIONPRINT_H

#include <QtGui>

class SituationModel;
class SituationView;

class SituationPrint : public QTextEdit {
    Q_OBJECT

    public:
        SituationPrint(SituationModel *situation, SituationView *view, QWidget *parent = 0);
        ~SituationPrint() {}
        void render(QRectF pageRect);

    public slots:
        void print(QPrinter *printer) const { QTextEdit::print(printer); }

    private:
        SituationModel *m_situation;
        SituationView *m_view;
};

#endif // SITUATIONPRINT_H
