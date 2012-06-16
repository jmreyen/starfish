#include "ticketreport.h"


TicketReport::TicketReport(QObject *parent) :
    QTextDocument(parent),
    theCursor(this)
{
    QImage img("dekagrid_large.png");
    theCursor.insertImage(img);
    theCursor.insertHtml("<br>");
}

void TicketReport::insertheader(const QString &arg)
{
    theCursor.insertHtml("<h1>"+arg+"</h1><br>");
}

void TicketReport::beginInsertTicket()
{
    theMap.clear();
}

void insertRow(QTextCursor &cursor, const QString header, const QString text)
{
    if (text.length()>0) {
        cursor.insertHtml("<h2>" + header + "</h2>");
        cursor.insertHtml("<table cellspacing=0 cellpadding = 2 border=1><tr align=left><th>ID</th><th>Summary</th><th>Requested by</th></tr>"+text+"</table>");
    }
}

void TicketReport::endInsertTicket()
{
    insertRow(theCursor, "Selected Stories", theMap["selected"]);
    insertRow(theCursor, "New Stories", theMap["new"]);
    insertRow(theCursor, "Wating Stories", theMap["waiting"]);
    insertRow(theCursor, "Postponed Stories", theMap["postponed"]);
    insertRow(theCursor, "Product Backlog", theMap["backlog"]);
}


void TicketReport::insertTicket(int id, const QString &sum, const QString &desc, const QString &htd, const QString &prio, const QString &est, const QString &usr, const QString &stat)
{
    theMap[stat] += "<tr><td align=right>" + QString::number(id)+ "</td><td align=left>"+sum+"</td><td align=left>" + usr + "</td></tr>";
}
