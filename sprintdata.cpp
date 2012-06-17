#include "sprintdata.h"


SprintData::SprintData(QString n, QDate d, bool b, QString dsc):
    theName(n),
    theDueDate(d),
    completed(b),
    description(dsc)
{
    parseDescription();
}

void SprintData::parseDescription()
{

}
