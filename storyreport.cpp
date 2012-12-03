#include "storyreport.h"
#include "storyitem.h"
#include "fields.h"

#include <QRegExp>




AbstractStoryReport::AbstractStoryReport() :
    QString()
{
}

QString & AbstractStoryReport::wiki2html(QString &str)
{
    QRegExp regExp;
    regExp.setMinimal(true);
    //bold
    regExp.setPattern("'''(.*)'''");
    str.replace(regExp, "<b>\\1</b>");
    //italic
    regExp.setPattern("''(.*)''");
    str.replace(regExp, "<i>\\1</i>");
    //preformatted
    str.replace("{{{", "<pre>");
    str.replace("}}}", "</pre>");
    // break
    str.replace("[[br]]", "<br>", Qt::CaseInsensitive);
    return str;
}

StoryReport::StoryReport() :
    AbstractStoryReport(),
    theMap()
{
}


void StoryReport::beginInsertStory(const QString &header)
{
    QString::operator += ("<img src=\"qrc:/images/dekagrid_large.png\">");
    QString::operator += ("<h1>"+header+"</h1>");
    theMap.clear();
}

void StoryReport::insertStory(const StoryItem &story)
{
    QString id = story.data(ST_ID).toString();
    QString parentSummary = story.parent()->data(ST_DESC).toString();
    QString summary = story.data(ST_DESC).toString();
    QString description = story.data(ST_NOTES).toString();
    QString status = story.data(ST_STATUS).toString();
    QString estimation = story.data(ST_EST).toString();
    QString priority = story.data(ST_IMP).toString();

    theMap[parentSummary] +=
            "<div><h3>Backlog Item #" + id + ": " + summary + "</h3></div>"
            + "<ul>"
            + "<li>status: " + status + "</li>"
            + "<li>estimation: " + estimation + "</li>"
            + "<li>priority: " + priority + "</li>"
            + "</ul>";
    if (description.isEmpty())
        theMap[parentSummary] += "<p>No description available</p>";
    else
        theMap[parentSummary] += "<p>" + wiki2html(description) + "</p>";
}


void StoryReport::endInsertStory()
{
    foreach (const QString &str, theMap.keys()) {
        if (length()>0) {
            (*this) += "<h2>" + str + "</h2>";
            (*this) += "<p>" + theMap.value(str) + "</p>";
        }
    }
}

///

QuickReport::QuickReport() :
    AbstractStoryReport(),
    theMap()
{
}


void QuickReport::beginInsertStory(const QString &header)
{
    QString::operator += ("<img src=\"qrc:/images/dekagrid_large.png\">");
    QString::operator += ("<h1>"+header+"</h1>");
    theMap.clear();
}

void QuickReport::insertStory(const StoryItem &story)
{
    QString id = story.data(ST_ID).toString();
    QString parentSummary = story.parent()->data(ST_DESC).toString();
    QString summary = story.data(ST_DESC).toString();
    QString description = story.data(ST_NOTES).toString();
    QString status = story.data(ST_STATUS).toString();
    QString estimation = story.data(ST_EST).toString();
    QString priority = story.data(ST_IMP).toString();

    theMap[parentSummary] +=
            "<div>Backlog Item #" + id + ": " + summary
            + "(status: " + status + ", "
            + "estimation: " + estimation + ", "
            + "priority: " + priority + ")" + "</div>";
}


void QuickReport::endInsertStory()
{
    foreach (const QString &str, theMap.keys()) {
        if (length()>0) {
            (*this) += "<h2>" + str + "</h2>";
            (*this) += "<p>" + theMap.value(str) + "</p>";
        }
    }
}
///


MeetingHandout::MeetingHandout() :
    AbstractStoryReport()
{
}


void MeetingHandout::beginInsertStory(const QString &header)
{
    QString::operator += ("<img src=\"qrc:/images/dekagrid_large.png\">");
    QString::operator += ("<h1>"+header+"</h1>");
    theMap.clear();
}

void MeetingHandout::insertStory(const StoryItem &story)
{
    QString id = story.data(ST_ID).toString();
    QString parentSummary = story.parent()->parent()?story.parent()->data(ST_DESC).toString():"misc";
    QString summary = story.data(ST_DESC).toString();
    QString description = story.data(ST_NOTES).toString();
    QString status = story.data(ST_STATUS).toString();
    QString estimation = story.data(ST_EST).toString();
    QString priority = story.data(ST_IMP).toString();
    QString sprint = story.data(ST_SPRINT).toString();
    QString user = story.data(ST_USER).toString();

    if (status=="done" && sprint != "current")
        return;

    if (sprint=="current")
        theSprintReview +=
            "<tr><td width=10%>" + id + "</td><td>" + summary + "</td><td width=20%>" + user + "</td><td><img src=\"qrc:/images/" + status + ".jpg\" width=40px height=50px></td><td>" + priority + "</td></tr>";

    if (status == "new") {
        theNewStories +=
            "<tr><td width=10%>" + id + "</td><td>" + summary + "</td><td width=20%>" + user + "</td></tr>";
        if (description.isEmpty())
            theNewStories += "<tr><td></td><td>No description available</td></tr>";
        else
            theNewStories += "<tr><td></td><td>" + wiki2html(description) + "</td></tr>";
    }

    theMap[parentSummary] +=
            "<tr><td width=10%>" + id + "</td><td>" + summary + "</td><td width=20%>" + user + "</td><td><img src=\"qrc:/images/" + status + ".jpg\" width=40px height=50px></td><td>" + priority + "</td></tr>";

}

void MeetingHandout::endInsertStory()
{
    (*this) += "<h2>Sprint Review</h2>";
    (*this) += "<table border=1 cellspacing=0 cellpadding=1 width=100%>";
    (*this) += "<tr><th>ID</th><th>Summary</th><th>Requested</th><th>Status</th><th>Prio</th>";
    (*this) += theSprintReview;
    (*this) += "</table>";

    (*this) += "<div style=\"page-break-before: always\">";

    (*this) += "<h2>New Requests</h2>";
    (*this) += "<table border=1 cellspacing=0 cellpadding=1 width=100%>";
    (*this) += "<tr><th>ID</th><th>Summary</th><th>Requested</th>";
    (*this) += theNewStories;
    (*this) += "</table>";

    (*this) += "</div>";
    (*this) += "<div style=\"page-break-before: always\">";


    foreach (const QString &str, theMap.keys()) {
        (*this) += "<h2>" +  str + "</h2>";
        (*this) += "<table border=1 cellspacing=0 cellpadding=1 width=100%>";
        (*this) += "<tr><th>ID</th><th>Summary</th><th>Requested</th><th>Status</th><th>Prio</th>";
        (*this) += theMap.value(str);
        (*this) += "</table>";
    }
    (*this) += "</div>";
}




