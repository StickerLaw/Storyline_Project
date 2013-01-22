#include "slotlayout.h"

SlotLayout::SlotLayout(int time_steps)
{
    layout = new QHash<int, QPair<int, int>* >();
    for (int i = 0; i < time_steps; i++) {
        value = new QPair<int, int>();
        layout->insert(i, value);
    }

}
QHash<int, QPair<int, int>* >* SlotLayout::getSlotLayout()
{
    return layout;
}

void SlotLayout::setItem(int idx, int base, int height)
{
    QPair<int, int> *value = new QPair<int, int>(base, height);
    layout->insert(idx, value);
}
