#ifndef SESSIONLAYOUT_H
#define SESSIONLAYOUT_H
#include <interactionsession.h>
#include <QHash>
#include <QSet>
#include <QList>
#include <QDebug>
class SessionLayout
{
public:
    SessionLayout(int slot_number, InteractionSession *interaction_session);
    QHash<QString, QString>* getClassifiedMembers();
    QHash<QString, QString>* getClassifiedMembers_only();
    QHash<QString, int>* getSessionLayout();
    int getSlot();
    QSet<QString> *static_lines;
    QSet<QString> *rising_lines;
    QSet<QString> *dropping_lines;
    QSet<QString> *emerging_lines_rising;
    QSet<QString> *emerging_lines_dropping;
    QSet<QString> *emerging_lines_dying;
    QHash<QString, QString> *classified_members;
private:
    int slot;
    QHash<QString, int> *layout;

};

#endif // SESSIONLAYOUT_H
