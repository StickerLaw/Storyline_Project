#include "interactionsessioncontainer.h"

InteractionSessionContainer::InteractionSessionContainer()
{
    isList = new QList<InteractionSession*>();
    slot_assigned_interaction_sessions = new QList<QPair<InteractionSession*, int> *> ();
}
void InteractionSessionContainer::createCompareList_prev()
{
    prev_isList = new QList<InteractionSession*>();

}
void InteractionSessionContainer::createCompareList_now()
{
    now_isList = new QList<InteractionSession*>();
}
void InteractionSessionContainer::initializeClassifiedInteractionSessions()
{
    classified_interaction_sessions = new QList<QPair<InteractionSession*, QString> *> ();
}
void InteractionSessionContainer::addClassifiedInteractionSession(InteractionSession* interaction_session, QString flag)
{
    QPair<InteractionSession*, QString> *pair = new QPair<InteractionSession*, QString>();
    pair->first = interaction_session;
    pair->second = flag;
    classified_interaction_sessions->append(pair);
}

QList<QPair<InteractionSession*, QString> *> *InteractionSessionContainer::getClassifiedInteractionSessions()
{
    return classified_interaction_sessions;
}

void InteractionSessionContainer::setSlotAssignedInteractionSesesions(QList<QPair<InteractionSession*, int>*> *_slot_assigned_interaction_sessions)
{
    slot_assigned_interaction_sessions = _slot_assigned_interaction_sessions;
}

QList<QPair<InteractionSession*, int>*> *InteractionSessionContainer::getInteractionSessionsWithSlot()
{
    return slot_assigned_interaction_sessions;
}
void InteractionSessionContainer::add(InteractionSession *_is)
{
    isList->append(_is);
}
void InteractionSessionContainer::addTemp(InteractionSession *_nis)
{
    now_isList->append(_nis);
}

void InteractionSessionContainer::update(InteractionSession *_is, int prev_time, int time)
{
    QList<InteractionSession*>::iterator it = isList->begin();
    int i = 0;
    if (!isList->isEmpty()) {

        for (; it!=isList->end();it++) {
            if (((*it)->getMembersOneStr().compare(_is->getMembersOneStr()) == 0) && ((*it)->getEndTime() == prev_time))
            {
                (*it)->extend(time);
                //isList->replace(i, (*it)); -> not necessarily used
                //(*it)->printIS(); -> checked already
            }
            i++;
        }
    }
//    QList<InteractionSession*>::iterator it_ = now_isList->begin();
//    int j = 0;
//    if (!now_isList->isEmpty()) {

//        for (; it_!=now_isList->end();it_++) {
//            if (((*it_)->getMembersOneStr().compare(_is->getMembersOneStr()) == 0) && ((*it_)->getEndTime() == prev_time))
//            {
//                (*it_)->extend(time);
//                //isList->replace(i, (*it)); -> not necessarily used
//                //(*it)->printIS(); -> checked already
//            }
//            j++;
//        }
//    }
}
void InteractionSessionContainer::clear()
{
    isList->clear();
    prev_isList->clear();
    now_isList->clear();
}
QList<InteractionSession*>* InteractionSessionContainer::getISList()
{
    return isList;
}
QList<InteractionSession*>* InteractionSessionContainer::getTempISList_now()
{
    return now_isList;
}
QList<InteractionSession*>* InteractionSessionContainer::getTempISList_prev()
{
    return prev_isList;
}

void InteractionSessionContainer::setPrevISList(QList<InteractionSession*> *now_isList)
{
    prev_isList = now_isList;
}
