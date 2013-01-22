#ifndef SLOTSEGMENTS_H
#define SLOTSEGMENTS_H
#include <QList>
#include <slotsegment.h>
#include <interactionsession.h>
#include <cstring>
class SlotSegments
{
public:
    SlotSegments(int slots_count);
    SlotSegment* belongToSlotSegment(InteractionSession *interaction_session, int slot_idx);
    QList<QList<SlotSegment *> *> * getSlotSegments();
    QList<SlotSegment *>* getSlot();
private:
    QList<QList<SlotSegment *> *> *slot_segments;
    QList<SlotSegment *> *slot;

};

#endif // SLOTSEGMENTS_H
