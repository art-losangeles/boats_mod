//
// C++ Implementation: situationprint
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

#include "situationprint.h"

#include "situationmodel.h"
#include "situationview.h"

extern int debugLevel;


SituationPrint::SituationPrint(SituationModel *situation, SituationView *view, QWidget *parent)
    : QTextEdit(parent),
    m_situation(situation),
    m_view(view) {
    setAcceptRichText(true);
}

void SituationPrint::render() {
    if (!m_situation) {
        return;
    }
    QTextCursor cursor(document());

    QTextBlockFormat headingblock;
    headingblock.setAlignment(Qt::AlignHCenter);
    QTextCharFormat headingchar;
    headingchar.setFontPointSize(20);
    headingchar.setFontWeight(2);

    QTextBlockFormat descblock;
    descblock.setAlignment(Qt::AlignLeading);
    QTextCharFormat descchar;
    descchar.setFontPointSize(14);
    descchar.setFontWeight(2);

    QTextBlockFormat textblock;
    textblock.setAlignment(Qt::AlignLeading);
    textblock.setIndent(1);
    QTextCharFormat textchar;
    textchar.setFontPointSize(12);

    cursor.insertBlock(headingblock);
    cursor.insertText(m_situation->title(), headingchar);

    if (!m_situation->rules().isEmpty()) {
        cursor.insertBlock();
        cursor.insertBlock(descblock);
        cursor.insertText(tr("Rule "), descchar);
        cursor.insertText(m_situation->rules(), textchar);
    }


    if (!m_situation->abstract().isEmpty()) {
        cursor.insertBlock();
        cursor.insertBlock(descblock);
        cursor.insertBlock();
        cursor.insertBlock(textblock);
        cursor.insertText(m_situation->abstract(), textchar);
    }

    cursor.insertBlock();
    cursor.insertBlock(headingblock);
    QPixmap image(m_view->screenShot());
    document()->addResource(QTextDocument::ImageResource,
         QUrl("mydata://image.png"), QVariant(image));
    cursor.insertImage("mydata://image.png");

    if (!m_situation->description().isEmpty()) {
        cursor.insertBlock();
        cursor.insertBlock(descblock);
        cursor.insertBlock();
        cursor.insertBlock(textblock);
        cursor.insertText(m_situation->description(), textchar);
    }
}
