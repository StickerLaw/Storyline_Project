#include "interactionsession.h"


InteractionSession::InteractionSession(int _id, QStringList _members, int _starttime, int _endtime, int _location)
{
    id = _id;
    members = _members;
    starttime = _starttime;
    endtime = _endtime;
    location = _location;
    /*for genetic algorithm*/
    proceeding_interaction_sessions = new QList<InteractionSession*>();
    following_interaction_sessions = new QList<InteractionSession*>();
}


void InteractionSession::setID(int _id)
{
    id = _id;
}
int InteractionSession::getID()
{
    return id;
}

QStringList InteractionSession::getMembers()
{
    return members;
}

void InteractionSession::setMembersOneStr(QString _str)
{
    membersOneStr = _str;
}

QString InteractionSession::getMembersOneStr()
{
    return membersOneStr;
}
int InteractionSession::getStartTime()
{
    return starttime;
}

int InteractionSession::getEndTime()
{
    return endtime;
}

void InteractionSession::extend(int _endtime)
{
    endtime = _endtime;
}

void InteractionSession::printIS()
{
    qDebug() << "\tIS id: " << getID() << " members: " << getMembersOneStr() << " starttime: " << starttime << " endtime: " << endtime << " location: " << location;
}


