#include "slotsegments.h"

SlotSegments::SlotSegments(int slots_count)
{

    slot_segments = new QList<QList<SlotSegment *>* >();
    for (int slot_idx = 0; slot_idx < slots_count; slot_idx++)
    {
        slot = new QList<SlotSegment *>();
        slot_segments->append(slot);
    }

}
QList<SlotSegment *>* SlotSegments::getSlot()
{
    return slot;
}

QList<QList<SlotSegment *>* >* SlotSegments::getSlotSegments()
{
    return slot_segments;
}

SlotSegment* SlotSegments::belongToSlotSegment(InteractionSession *interaction_session, int slot_idx = INT_MAX)
{
    if (slot_idx == INT_MAX)
    {
        if (!slot_segments->isEmpty()) {
            for (int i = 0; i < slot_segments->size(); i++)
            {
                slot = slot_segments->at(i);
                for (int j = 0; j < slot->size(); j++)
                {
                    SlotSegment* slsg = slot->at(j);
                    for (int c = 0; c < slsg->getInteractionSessions()->size(); c++)
                    {
                        if (slsg->getInteractionSessions()->at(c) == interaction_session)
                        {
                            return slsg;
                        }
                    }
                }
            }
        }
    }
    else
    {
        if (!slot_segments->isEmpty()) {
            slot = slot_segments->at(slot_idx);
            for (int k = 0; k < slot->size(); k++)
            {
                SlotSegment* slsg = slot->at(k);
                for (int c = 0; c < slsg->getInteractionSessions()->size(); c++)
                {
                    //if (slsg->getInteractionSessions()->at(c)->getEndTime() == interaction_session->getEndTime() && slsg->getInteractionSessions()->at(c)->getMembersOneStr().compare(interaction_session->getMembersOneStr()) == 0)
                    if (slsg->getInteractionSessions()->at(c) == interaction_session)
                    {
                        return slsg;
                    }
                }
            }
        }
    }
    return NULL;
}

