#ifndef SLOTSEGMENT_H
#define SLOTSEGMENT_H
#include <interactionsession.h>
#include <sessionlayout.h>
#include <QHash>
#include <QList>
class SlotSegment
{

public:
    SlotSegment();
    void setInteractionSession(InteractionSession *interaction_session, SessionLayout *session_layout);
    QList<InteractionSession*>* getInteractionSessions();
    QHash<int, int> *bottom_coordinates;
    QHash<int, int> *top_coordinates;
    int slot;
private:
    QList<InteractionSession*> *interaction_sessions;
    int starting_time;
    int ending_time;
    int bottom_coord;
    int top_coord;

};

#endif // SLOTSEGMENT_H
