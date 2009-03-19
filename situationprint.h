//
// C++ Interface: situationprint
//
// Description:
//
//
// Author: Thibaut GRIDEL <tgridel@free.fr>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
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
