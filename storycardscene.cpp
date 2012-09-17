#include "storycardscene.h"

#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QTextDocument>
#include <QTextBlock>
#include <QTextCursor>


StoryCardScene::StoryCardScene(QObject *parent) :
    QGraphicsScene(0,0,298,210,parent)
{
    initDefaultLayout(0,0,298,210);
    initCard(0,0,298,210);
}

StoryCardScene::StoryCardScene(const QRectF &sceneRect, QObject *parent) :
    QGraphicsScene(QRectF(0,0,298,210), parent)
{
    initDefaultLayout(0,0,298,210);
    initCard(0,0,298,210);
}

StoryCardScene::StoryCardScene(qreal x, qreal y, qreal width, qreal height, QObject *parent) :
    QGraphicsScene(x,y,width,height)
{
    initDefaultLayout(0,0,298,210);
    initCard(x,y,width,height);
}

StoryCardScene::~StoryCardScene()
{
}


//formatting helper functions
void adjustFontSize(QGraphicsTextItem *item, double width, double height, double fontSize, int fontWeight=QFont::Normal, double deltaSize = 0)
{
    QFont font;
    font.setWeight(fontWeight);
    int docHeight;

    if (fontSize<=2)
    {
        item->setPlainText("See TRAC Ticket for details.");
        fontSize = 10;
    }

    // Determine height of text item with given width and font size
    item->setTextWidth(width);
    font = item->font();
    font.setPointSize(fontSize);
    item->setFont(font);
    docHeight=item->document()->size().height();
    // if item is not high enough, increase font size
    if (docHeight < height)
        // except when it was already made smaller in the previus recursion
        if (deltaSize<0)
            return;
        else
            adjustFontSize(item, width, height, fontSize+1, fontWeight, 1);
    //if item is too high decrease font size
    else if (docHeight > height)
        adjustFontSize(item, width, height, fontSize-1, fontWeight, -1);

    return;
}
void center(QGraphicsTextItem *item)
{
    QTextBlockFormat format;
    format.setAlignment(Qt::AlignCenter);

    QTextCursor cursor = item->textCursor();
    cursor.select(QTextCursor::Document);
    cursor.mergeBlockFormat(format);
    cursor.clearSelection();
    item->setTextCursor(cursor);
}

void StoryCardScene::setID(const QString & txt){
    theStoryID->setPlainText(txt);
    adjustFontSize(theStoryID, theIDRect.width(), theIDRect.height(), 10.);
}

void StoryCardScene::setDesc(const QString & txt)
{
    theStoryDesc->setPlainText(txt);
    adjustFontSize(theStoryDesc, theDescRect.width(), theDescRect.height(), 10., QFont::Bold);
}

void StoryCardScene::setNotes(const QString & txt)
{
    theStoryNotes->setPlainText(txt.left(200));
    adjustFontSize(theStoryNotes, theNotesRect.width(), theNotesRect.height(), 10.);
}

void StoryCardScene::setHTD(const QString & txt)
{
    theStoryHTD->setPlainText(txt);
    adjustFontSize(theStoryHTD, theHTDRect.width(), theHTDRect.height(), 10.);
}

void StoryCardScene::setImp(const QString & txt)
{
    theStoryImp->setPlainText(txt);
    adjustFontSize(theStoryImp, theImpRect.width(), theImpRect.height(), 10.);
    center(theStoryImp);
}

void StoryCardScene::setEst(const QString & txt)
{
    theStoryEst->setPlainText(txt);
    adjustFontSize(theStoryEst, theEstRect.width(), theEstRect.height(), 10.);
    center(theStoryEst);
}
void StoryCardScene::setUser(const QString & txt)
{
    theStoryUser->setPlainText(txt);
    adjustFontSize(theStoryUser, theUserRect.width(), theUserRect.height(), 10.);
    center(theStoryUser);
}

void StoryCardScene::clearCard()
{
    theStoryID->setPlainText("");
    theStoryDesc->setPlainText("");
    theStoryNotes->setPlainText("");
    theStoryHTD->setPlainText("");
    theStoryImp->setPlainText("");
    theStoryEst->setPlainText("");
    theStoryUser->setPlainText("");
}

void StoryCardScene::initCard(qreal x, qreal y, qreal width, qreal height)
{
    QGraphicsTextItem *pItem;
    QFont aFont("Arial",8);
    // add static scene items
    addRect(theDescRect); //Description
    pItem = addText("Notes", aFont);
    pItem->setPos(theNotesRect.x(), theNotesRect.y()- 2*aFont.pointSize()-2);
    addRect(theNotesRect); //Notes
    pItem = addText("How To Demo", aFont);
    pItem->setPos(theHTDRect.x(), theHTDRect.y()- 2*aFont.pointSize()-2);
    addRect(theHTDRect); //Demo
    aFont.setPointSize(7);
    pItem = addText("Importance", aFont);
    pItem->setPos(theImpRect.x(), theImpRect.y()- 2*aFont.pointSize()-2);
    addRect(theImpRect); //Importance
    pItem = addText("Estimation", aFont);
    pItem->setPos(theEstRect.x(), theEstRect.y()- 2*aFont.pointSize()-2);
    addRect(theEstRect); //Estimation

    //add variable scene items
    theStoryID = addText("");
    theStoryID->setPos(theIDRect.x(),theIDRect.y());
    theStoryID->setTextWidth(theIDRect.width());
    theStoryDesc = addText("");
    theStoryDesc->setPos(theDescRect.x(),theDescRect.y());
    theStoryDesc->setTextWidth(theDescRect.width());
    theStoryNotes = addText("");
    theStoryNotes->setPos(theNotesRect.x(),theNotesRect.y());
    theStoryNotes->setTextWidth(theNotesRect.width());
    theStoryHTD = addText("");
    theStoryHTD->setPos(theHTDRect.x(),theHTDRect.y());
    theStoryHTD->setTextWidth(theHTDRect.width());
    theStoryImp = addText("");
    theStoryImp->setPos(theImpRect.x(),theImpRect.y());
    theStoryImp->setTextWidth(theImpRect.width());
    theStoryEst = addText("");
    theStoryEst->setPos(theEstRect.x(),theEstRect.y());
    theStoryEst->setTextWidth(theEstRect.width());
    theStoryUser = addText("");
    theStoryUser->setPos(theUserRect.x(),theUserRect.y());
    theStoryUser->setTextWidth(theUserRect.width());
}

void StoryCardScene::initDefaultLayout(qreal x, qreal y, qreal width, qreal height)
{
    theIDRect.setRect(0,0,160,30);
    theDescRect.setRect(5,30, 288,65);
    theNotesRect.setRect(5,110,233,50);
    theHTDRect.setRect(5,175,233,30);
    theImpRect.setRect(243,110,50,40);
    theEstRect.setRect(243,165,50,40);
    theUserRect.setRect(160,0,148,25);
}
