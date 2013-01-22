#include "slotsegment.h"
#include <assert.h>

SlotSegment::SlotSegment()
{
    slot = -1;
    interaction_sessions = new QList<InteractionSession*>();
    starting_time = -1;
    ending_time = -1;
    bottom_coordinates = new QHash<int, int>();
    top_coordinates = new QHash<int, int>();
}

void SlotSegment::setInteractionSession(InteractionSession *interaction_session, SessionLayout *session_layout)
{
    interaction_sessions->append(interaction_session);

    if (starting_time == -1 || starting_time > interaction_session->getStartTime())
        starting_time = interaction_session->getStartTime();
    if (ending_time == -1 || ending_time < interaction_session->getEndTime())
        ending_time = interaction_session->getEndTime();

    bottom_coord = INT_MAX;
    top_coord = INT_MAX;

        QHash<QString, int>::iterator i = session_layout->getSessionLayout()->begin();
        int member_coordinate;
        for (;i != session_layout->getSessionLayout()->end(); i++)
        {
            member_coordinate = i.value();
            if (bottom_coord == INT_MAX || bottom_coord > member_coordinate)
                bottom_coord = member_coordinate;
            if (top_coord == INT_MAX || top_coord < member_coordinate)
                top_coord = member_coordinate;

        }

    assert(top_coord != INT_MAX);
    assert(bottom_coord != INT_MAX);
    for (int time_step = interaction_session->getStartTime(); time_step <= interaction_session->getEndTime(); time_step++)
    {
        bottom_coordinates->insert(time_step, bottom_coord);
        top_coordinates->insert(time_step, top_coord);
    }
}
QList<InteractionSession*>* SlotSegment::getInteractionSessions()
{
    return interaction_sessions;
}
