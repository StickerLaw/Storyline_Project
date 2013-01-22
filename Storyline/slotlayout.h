#ifndef SLOTLAYOUT_H
#define SLOTLAYOUT_H
#include <QHash>
#include <QPair>
class SlotLayout
{
public:
    SlotLayout(int time_steps);
    void setItem(int idx, int base, int height);
    QHash<int, QPair<int, int>* > *getSlotLayout();
private:
    QHash<int, QPair<int, int>* > *layout;
    QPair<int, int> *value;
};

#endif // SLOTLAYOUT_H
