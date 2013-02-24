#include "slotbaselayout.h"

SlotBaseLayout::SlotBaseLayout(int slot_count)
{
    Slots = new QList<QList<InteractionSession*>*>;
    /*Initialize the list of the list of the slot*/
    for (int i = 0; i < slot_count; i++) {
        Slot = new QList<InteractionSession*>();
        Slots->insert(INT_MAX, Slot);
    }
    /*Key: InteractionSession, Value: SessionLayout*/
    sessions_layout = new QHash<InteractionSession*, SessionLayout*>();
}
QHash<InteractionSession*, SessionLayout*>* SlotBaseLayout::getSessionsLayout()
{
    return sessions_layout;
}
QList<QList<InteractionSession*>*>* SlotBaseLayout::getSlots()
{
    return Slots;
}

bool SlotBaseLayout::setInteractionSessionToSlot(InteractionSession *interaction_session, int slot_number)
{
    _availability = checkAvailability(interaction_session, slot_number);

    if (_availability)
    {
        Slot = Slots->at(slot_number);
        if (Slot == NULL) {
            Slot = new QList<InteractionSession*>();
            Slot->append(interaction_session);
            Slots->insert(slot_number, Slot);
        }
        else {
            Slot->append(interaction_session);
            Slots->replace(slot_number, Slot);
        }

        SessionLayout* session_layout = new SessionLayout(slot_number, interaction_session);
        sessions_layout->insert(interaction_session, session_layout);
        //qDebug() << "Set IS to Slot: " << interaction_session->getMembersOneStr() << slot_number;

    }
    return _availability;
}
bool SlotBaseLayout::checkAvailability(InteractionSession *interaction_session, int slot_number)
{
    QList<InteractionSession*> *registered_interaction_sessions = Slots->at(slot_number);
    _availability = true;

    for (int i = 0; i < registered_interaction_sessions->size(); i++)
    {
        InteractionSession *registered_interaction_session = registered_interaction_sessions->at(i);

        if (interaction_session->getStartTime() >= registered_interaction_session->getEndTime() || interaction_session->getEndTime() <= registered_interaction_session->getStartTime()) {
            _availability = true;

        }
        else {
            _availability = false;
            break;
        }
    }
    return _availability;


}
