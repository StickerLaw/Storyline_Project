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
    void add(InteractionSession *_is);
    void update(InteractionSession *_is, int prev_time, int time);
    void addTemp(InteractionSession *_nis);
    void setPrevISList(QList<InteractionSession*> *now_isList);
//    InteractionSession* find(QString _str);
    QList<InteractionSession*>* getISList();
    QList<InteractionSession*>* getTempISList_now();
    QList<InteractionSession*>* getTempISList_prev();

    bool open;

    void initializeClassifiedInteractionSessions();
    void addClassifiedInteractionSession(InteractionSession* interaction_session, QString flag);
    QList<QPair<InteractionSession*, QString> *> *getClassifiedInteractionSessions();


    void setISList_prevLayout(QList<QPair<InteractionSession*, int>*> *isList_currentLayout);
    QList<QPair<InteractionSession*, int>*> *getISList_prevLayout();        
    void initializeISList_prevLayout();

    void setCurrentInteractionSessions(QList<QPair<InteractionSession*, int> *> *_all_ISs_updated_with_new_ISs);
    QList<QPair<InteractionSession*, int>*> *getCurrentInteractionSessions();

    void setExtendingISsToPrevISs(QList<QPair<InteractionSession*, int> *> *_extendingISsWithPrevISs);
    QList<QPair<InteractionSession*, int> *> *getExtendingISsWithPrevISs();

    void clear();

private:
    QList<InteractionSession*> *isList;
    QList<InteractionSession*> *prev_isList;
    QList<InteractionSession*> *now_isList;
    QList<QPair<InteractionSession*, int> *> *extendingISsWithPrevISs;
    QList<QPair<InteractionSession*, int> *> *all_ISs_updated_with_new_ISs;
    QList<QPair<InteractionSession*, int>*> *isList_prevLayout;
    QList<QPair<InteractionSession*, QString> *> *classified_interaction_sessions;

};

#endif // INTERACTIONSESSIONCONTAINER_H
