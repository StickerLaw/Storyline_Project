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
    if (!isList->isEmpty()) {

        for (; it!=isList->end();it++) {
            if (((*it)->getMembersOneStr().compare(_is->getMembersOneStr()) == 0) && ((*it)->getEndTime() == prev_time))
            {
                (*it)->extend(time);
                //(*it)->printIS();
            }
        }
    }
}
void InteractionSessionContainer::clear()
{
    isList->clear();
    prev_isList->clear();
    now_isList->clear();
}

InteractionSession* InteractionSessionContainer::find(QString _str)
{
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
