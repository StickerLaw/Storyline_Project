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
    InteractionSession* find(QString _str);
    QList<InteractionSession*>* getISList();
    QList<InteractionSession*>* getTempISList_now();
    QList<InteractionSession*>* getTempISList_prev();
    void clear();
    void setPrevISList(QList<InteractionSession*> *now_isList);

private:
    QList<InteractionSession*> *isList;
    QList<InteractionSession*> *prev_isList;
    QList<InteractionSession*> *now_isList;
};

#endif // INTERACTIONSESSIONCONTAINER_H
