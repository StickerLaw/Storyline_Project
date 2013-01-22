#ifndef SLOTBASELAYOUT_H
#define SLOTBASELAYOUT_H
#include <QHash>
#include <interactionsession.h>
#include <sessionlayout.h>

class SlotBaseLayout
{
public:
    SlotBaseLayout(int slot_count);
    bool setInteractionSessionToSlot(InteractionSession*, int);
    bool checkAvailability(InteractionSession*, int);
    QHash<InteractionSession*, SessionLayout*>* getSessionsLayout();
    QList<QList<InteractionSession*>*> *getSlots();
private:
    QList<InteractionSession*>* Slot;
    QHash<InteractionSession*, SessionLayout*> *sessions_layout;
    QList<QList<InteractionSession*>*> *Slots;
    bool _availability;

};

#endif // SLOTBASELAYOUT_H
