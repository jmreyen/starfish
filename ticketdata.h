#ifndef TICKETDATA_H
#define TICKETDATA_H

#include <QString>

struct TicketData
{
    TicketData(QString id, QString desc, QString notes, QString htd, QString imp, QString est, QString user, QString sprint, QString status) :
        theID(id),theDesc(desc), theNotes(notes), theHTD(htd), theImp(imp),
        theEst(est), theUser(user), theSprint(sprint), theStatus(status)  {}
    QString theID;
    QString theDesc;
    QString theNotes;
    QString theHTD;
    QString theImp;
    QString theEst;
    QString theUser;
    QString theSprint;
    QString theStatus;
} ;



#endif // TICKETDATA_H
