#ifndef INTERACTIONSESSIONCONTAINER_H
#define INTERACTIONSESSIONCONTAINER_H
#include <QList>
#include <interactionsession.h>
class InteractionSessionContainer
{
public:
    InteractionSessionContainer();
    void createCompareList_prev();
    void createCompareList_now();
    void InitializeLayout();
    void add(InteractionSession *_is);
    void update(InteractionSession *_is, int prev_time, int time);
    void addTemp(InteractionSession *_nis);
    InteractionSession* find(QString _str);
    QList<InteractionSession*>* getISList();
    QList<InteractionSession*>* getTempISList_now();
    QList<InteractionSession*>* getTempISList_prev();
    void clear();
    void setPrevISList(QList<InteractionSession*> *now_isList);
    int prev_time_step, current_time_step;
    void initializeClassifiedInteractionSessions();
    void addClassifiedInteractionSession(InteractionSession* interaction_session, QString flag);
    QList<QPair<InteractionSession*, QString> *> *getClassifiedInteractionSessions();
    void setSlotAssignedInteractionSesesions(QList<QPair<InteractionSession*, int>*> *now_slot_assigned_interaction_sessions);
    QList<QPair<InteractionSession*, int>*> *getInteractionSessionsWithSlot();
    bool open;

private:
    QList<InteractionSession*> *isList;
    QList<InteractionSession*> *prev_isList;
    QList<InteractionSession*> *now_isList;
    QList<QPair<InteractionSession*, QString> *> *classified_interaction_sessions;
    QList<QPair<InteractionSession*, int> *> *slot_assigned_interaction_sessions;
};

#endif // INTERACTIONSESSIONCONTAINER_H
