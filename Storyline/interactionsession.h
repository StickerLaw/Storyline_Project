#ifndef INTERACTIONSESSION_H
#define INTERACTIONSESSION_H
#include <QStringList>
#include <iostream>
#include <qdebug.h>
class InteractionSession
{
public:
    InteractionSession(int _id, QStringList _members, int _starttime, int _endtime, int _location);
    void setID(int _id);
    int getID();
    QStringList getMembers();
    void setMembersOneStr(QString _str);
    QString getMembersOneStr();
    void extend(int _endtime);
    void printIS();
    int getStartTime();
    int getEndTime();

    QList<InteractionSession*> *proceeding_interaction_sessions;
    QList<InteractionSession*> *following_interaction_sessions;
private:
    int id;
    QStringList members;
    QString membersOneStr;
    int starttime, endtime;
    int location;
    QList<float> *color;

};
inline bool operator==(InteractionSession &is, InteractionSession &is2)
{
    return is.getID() == is2.getID();
}
inline uint qHash(InteractionSession &key)
{
    return qHash(key.getID());
}
#endif // INTERACTIONSESSION_H
