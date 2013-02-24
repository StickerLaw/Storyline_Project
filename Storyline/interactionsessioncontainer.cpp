#include "interactionsessioncontainer.h"

InteractionSessionContainer::InteractionSessionContainer()
{
    isList = new QList<InteractionSession*>();

}
void InteractionSessionContainer::createCompareList_prev()
{
    prev_isList = new QList<InteractionSession*>();

}
void InteractionSessionContainer::createCompareList_now()
{
    now_isList = new QList<InteractionSession*>();
}

void InteractionSessionContainer::initializeISList_prevLayout()
{
    isList_prevLayout = new QList<QPair<InteractionSession*, int>*>();
}
void InteractionSessionContainer::setISList_prevLayout(QList<QPair<InteractionSession*, int>*> *isList_currentLayout)
{
    isList_prevLayout = isList_currentLayout;
}

QList<QPair<InteractionSession*, int>*> *InteractionSessionContainer::getISList_prevLayout()
{
    return isList_prevLayout;
}
void InteractionSessionContainer::setExtendingISsToPrevISs(QList<QPair<InteractionSession*, int> *> *_extendingISsWithPrevISs)
{
    extendingISsWithPrevISs = _extendingISsWithPrevISs;
}

QList<QPair<InteractionSession*, int> *> *InteractionSessionContainer::getExtendingISsWithPrevISs()
{
    return extendingISsWithPrevISs;
}

void InteractionSessionContainer::setCurrentInteractionSessions(QList<QPair<InteractionSession*, int> *> *_all_ISs_updated_with_new_ISs)
{
    all_ISs_updated_with_new_ISs = _all_ISs_updated_with_new_ISs;
}
QList<QPair<InteractionSession*, int>*> *InteractionSessionContainer::getCurrentInteractionSessions()
{
    return all_ISs_updated_with_new_ISs;
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

//InteractionSession* InteractionSessionContainer::find(QString _str)
//{
//    if (isList->isEmpty()) qDebug() << "nothing in the interaction session container yet";
//    if (prev_isList->isEmpty()) qDebug() << "no interaction sessions for the previous time";
//    else {
//        for (int j = 0; j < prev_isList->size(); j++)
//        {
//            if (prev_isList->at(j)->getMembersOneStr().compare(_str) == 0)
//            {
//                InteractionSession* existing_is = prev_isList->at(j);
//                return existing_is;
//            }
//            else return NULL;
//        }
//    }
//    return NULL;
//}

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
