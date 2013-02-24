#include "global_functions.h"


//QList<QList<int>* > *populateGenomes(int population, int slots_count, int genome_length)
//{

//    QTime time = QTime::currentTime();
//    qsrand((uint)time.msec());
//    QList<QList<int> *> *genome_pool = new QList<QList<int>*>();

//    for (int i = 0; i < population; i++)
//    {

//        QList<int> *genome = new QList<int>();
//        for (int j = 0; j < genome_length; j++)
//        {
//            int random_int = qrand() % slots_count;
//            genome->append(random_int);
//        }
//        genome_pool->append(genome);
//    }

//    return genome_pool;
//}

SlotBaseLayout* decodeGenome(QList<int>* genome, QList<InteractionSession*> *interaction_sessions)
{
    SlotBaseLayout* slot_base_layout = new SlotBaseLayout(SLOTS);
    for (int idx = 0; idx < genome->size(); idx++)
    {
        InteractionSession* interaction_session = interaction_sessions->at(idx);
        int gene = genome->at(idx);
        bool _valid_layout = slot_base_layout->setInteractionSessionToSlot(interaction_session, gene);
        if (!_valid_layout) {
            return NULL;
        }

    }

    return slot_base_layout;
}

void classifyProceedingLineSegments(SlotBaseLayout *slot_base_layout)
{
    /*Get Sessionslayout*/
    QHash<InteractionSession*, SessionLayout*>::iterator i = slot_base_layout->getSessionsLayout()->begin();
    for (;i != slot_base_layout->getSessionsLayout()->end(); i++)
    {
        SessionLayout *session_layout = i.value();
        InteractionSession *interaction_session = i.key();
        /*Get proceeding interaction sessions for each interaction session*/
        QList<InteractionSession*> *proceeding_interaction_sessions = interaction_session->proceeding_interaction_sessions;
        for (int j = 0; j < proceeding_interaction_sessions->size(); j++)
        {
            //qDebug() << "proceeding is size***:" << proceeding_interaction_sessions->size();
            /*Get intersecting members between proceeding interaction session and the current interaction session*/
            SessionLayout *proceeding_session_layout = slot_base_layout->getSessionsLayout()->value(proceeding_interaction_sessions->at(j));
            QList<QString> intersecting_members;
            QSet<QString> ms = interaction_session->getMembers().toSet();
            QSet<QString> pms = proceeding_interaction_sessions->at(j)->getMembers().toSet();           
            intersecting_members = ms.intersect(pms).toList();           
            if (proceeding_session_layout->getSlot() < session_layout->getSlot())
            {
                /*Classify this member into rising line*/
                for (int m = 0; m < intersecting_members.size(); m++) {
                    //qDebug() << "rising lines:" << intersecting_members.at(m);
                    session_layout->rising_lines->insert(intersecting_members.at(m));
                    session_layout->classified_members->insert(intersecting_members.at(m), "rising");
                }
            }

            else if (proceeding_session_layout->getSlot() == session_layout->getSlot())
            {
                /*Classify this member into static line*/
                for (int m = 0; m < intersecting_members.size(); m++) {
                    //qDebug() << "static lines:" << intersecting_members.at(m);
                    session_layout->static_lines->insert(intersecting_members.at(m));
                    session_layout->classified_members->insert(intersecting_members.at(m), "static");
                }
            }
            else if (proceeding_session_layout->getSlot() > session_layout->getSlot())
            {
                /*Classify this member into dropping line*/
                for (int m = 0; m < intersecting_members.size(); m++) {
                    //qDebug() << "dropping lines:" << intersecting_members.at(m);
                    session_layout->dropping_lines->insert(intersecting_members.at(m));
                    session_layout->classified_members->insert(intersecting_members.at(m), "dropping");
                }
            }
        }
    }

}

void classifyEmergingLineSegments(SlotBaseLayout *slot_base_layout)
{
    /*Get sorted interaction sessins in reverse chronological order*/
    QList<InteractionSession*> sorted_interaction_sessions = slot_base_layout->getSessionsLayout()->keys();
    qSort(sorted_interaction_sessions.begin(), sorted_interaction_sessions.end(), startTimeLessThen);

    for (int i = 0; i < sorted_interaction_sessions.size(); i++)
    {
        /*Get non-classified members in the interaction session which will be emerging lines*/
        SessionLayout *session_layout = slot_base_layout->getSessionsLayout()->value(sorted_interaction_sessions.at(i));
        QHash<QString, QString>* clm = session_layout->getClassifiedMembers();
        QList<QString> classified_members = clm->keys();
        QStringList full_members = sorted_interaction_sessions.at(i)->getMembers();
        QStringList nonclassified_members;
        QSet<QString> fms = full_members.toSet();
        QSet<QString> ms = classified_members.toSet();
        nonclassified_members = fms.subtract(ms).toList();
        QHash<QString, QString>::iterator it = clm->begin();

        /*Classify the emering lines*/
        for (int m = 0; m < nonclassified_members.size(); m++)
        {

            /*Check the immediate following interaction sessions for the future deviation*/
            int observed_endtime = sorted_interaction_sessions.at(i)->getEndTime();
            int test = 0;
            QString member_name = nonclassified_members.at(m);

            /*Every nonclassified members goes to "will rise emerging lines"*/

                session_layout->emerging_lines_rising->insert(member_name);
                session_layout->classified_members->insert(member_name, "will_rise");

//            for (int j = i + 1; j < sorted_interaction_sessions.size(); j++)
//            {
//                InteractionSession* is_j = sorted_interaction_sessions.at(j);
//                if (is_j->getStartTime() == observed_endtime+1) {
//                    SessionLayout *future_session_layout = slot_base_layout->getSessionsLayout()->value(is_j);
//                    QHash<QString, QString>* classified_members = future_session_layout->getClassifiedMembers();
//                    if (classified_members->contains(member_name)) {
//                        qDebug() << "classified?";
//                        if (future_session_layout->dropping_lines->contains(member_name)){
//                            /*Classify this member into emerging "will drop" line*/
//                            session_layout->emerging_lines_dropping->insert(member_name);
//                            session_layout->classified_members->insert(member_name, "will_drop");
//                            test = 1;
//                        } else if (future_session_layout->rising_lines->contains(member_name)){
//                            /*Classify this member into emerging "will rise" line*/
//                            session_layout->emerging_lines_rising->insert(member_name);
//                            session_layout->classified_members->insert(member_name, "will_rise");
//                            test = 1;
//                        } else if (future_session_layout->static_lines->contains(member_name)) {
//                            observed_endtime = is_j->getEndTime();
//                        } else {
//                            //qDebug() << "Emerging lines not classified in the following IS. Very wrong.";
//                            exit(1);
//                        }
//                    }
//                }
//            }
//            if (test == 0) {
//                /*Classify this member into emerging "will die" line*/
//                //qDebug() << "emerging lines dying:" << member_name; -> checked
//                session_layout->emerging_lines_rising->insert(member_name);
//                session_layout->classified_members->insert(member_name, "will_rise");
//            }


        }

    }
}

/*Sorting function for fittness in acsending order*/
bool fittnessLessThen(QPair<int, int> p1, QPair<int, int> p2)
{
    return p1.second < p2.second;
}
/*Sorting function for start time in descending order*/
bool startTimeLessThen_reversed(InteractionSession *is1, InteractionSession *is2)
{
    return is1->getStartTime() > is2->getStartTime();
}
/*Sorting function for start time in acsending order*/
bool startTimeLessThen(InteractionSession *is1, InteractionSession *is2)
{
    return is1->getStartTime() < is2->getStartTime();
}
/*Sorting function for fittness in acsending order of layout*/
bool fittnessLessThen2(QPair<QHash<QString, QHash<int, int>*>*, float>* i1, QPair<QHash<QString, QHash<int, int>*>*, float>* i2)
{
    return i1->second < i2->second;
}
/*Sorting function for fittness in acsending order of interaction session*/
bool fittnessLessThen3(QPair<QList<QPair<InteractionSession*, int> *> *, float> *i1, QPair<QList<QPair<InteractionSession*, int> *> *, float> *i2)
{
    return i1->second < i2->second;
}
/*Check all line segments are classified*/
bool checkAllLinsegmentsAreClassified(SlotBaseLayout *slot_base_layout)
{
    QHash<InteractionSession*, SessionLayout*>::iterator i = slot_base_layout->getSessionsLayout()->begin();
    for (;i != slot_base_layout->getSessionsLayout()->end(); i++)
    {
        SessionLayout *session_layout = i.value();
        InteractionSession *interaction_session = i.key();
        QHash<QString, QString>* classified_members = session_layout->classified_members;
        QStringList full_members = interaction_session->getMembers();
        if (classified_members->size() != full_members.size())
            return false;
    }
    return true;
}

void classifyLineSegments(SlotBaseLayout *slot_base_layout)
{
    /*Classify line segments into static, rising, dropping lines for each member in the interaction session*/
    classifyProceedingLineSegments(slot_base_layout);
    /*Classify line segments into will_rise, will_drop, will_die lines for each member in the interaction session*/
    classifyEmergingLineSegments(slot_base_layout);
    //qDebug() << "All segments are classified? " << checkAllLinsegmentsAreClassified(slot_base_layout);
}

/*Get common interaction session between two sets of interaction sessions (e.g. current interaction sessions and previous interaction sessions)*/
QList<InteractionSession*>* getCommonInteractionSession(QSet<InteractionSession*> sessions_1, QSet<InteractionSession*> sessions_2)
{
    QList<InteractionSession*>* common_sessions = new QList<InteractionSession*>();
    if (!sessions_1.isEmpty() && !sessions_2.isEmpty()) {
        QSet<InteractionSession*>::iterator i = sessions_1.begin();
        for (; i != sessions_1.end(); i++) {
            QSet<InteractionSession*>::iterator j = sessions_2.begin();
            for (; j != sessions_2.end(); j++) {
                if ((*i) == (*j)) {
                    common_sessions->append(*i);
                }
            }
        }
        return common_sessions;
    }
    else return NULL;
}
/*Get forerunning interaction sessions*/
QList<InteractionSession*>* getForerunningInteractionSessions(InteractionSession* subject_interaction_session, QList<InteractionSession*>* interaction_sessions, bool is_sorted)
{
    QList<InteractionSession*>* sorted_interaction_sessions = new QList<InteractionSession*>();
    if (is_sorted == false) {
        qSort(interaction_sessions->begin(), interaction_sessions->end(), startTimeLessThen);
        sorted_interaction_sessions = interaction_sessions;
    }
    else
        sorted_interaction_sessions = interaction_sessions;
    int subject_is_idx;

    if (!sorted_interaction_sessions->isEmpty()) {
        /*Get index of subject interaction session in the sorted interaction sessions*/
        for (int k = 0; k < sorted_interaction_sessions->size(); k++) {
            if (sorted_interaction_sessions->at(k) == subject_interaction_session) {
                subject_is_idx = k;               
            }
        }
        /*Create previous interaction sessions upto the index of the sorted interaction sessions*/
        QList<InteractionSession*>* tmp_forerunning_interaction_sessions = new QList<InteractionSession*>();
        for (int i = 0; i < subject_is_idx; i++)
        {
            tmp_forerunning_interaction_sessions->append(sorted_interaction_sessions->at(i));
        }
        QList<InteractionSession*>* forerunning_interaction_sessions = new QList<InteractionSession*>();
        /*Create forerunning interaction sessions when the end time of the interaction sessions is the start time of the subject interaction session*/
        for (int j = 0; j < tmp_forerunning_interaction_sessions->size(); j++)
        {
            InteractionSession* tmp_is = tmp_forerunning_interaction_sessions->at(j);
            if (tmp_is->getEndTime() == subject_interaction_session->getStartTime())
                forerunning_interaction_sessions->append(tmp_is);
        }

        return forerunning_interaction_sessions;
    }
    else return NULL;

}
/*This function assigns the layout for the line segments in each interaction session*/
void assignMemberAlignment(SlotBaseLayout *slot_base_layout)
{

    /*Sort interaction sessions in ascending order of start time*/
    QList<InteractionSession*> *sorted_interaction_sessions = new QList<InteractionSession*>();
    QList<InteractionSession*> temp_interaction_sessions = slot_base_layout->getSessionsLayout()->keys();
    qSort(temp_interaction_sessions.begin(), temp_interaction_sessions.end(), startTimeLessThen);
    sorted_interaction_sessions = &temp_interaction_sessions;

    for (int i = 0; i < sorted_interaction_sessions->size(); i++)
    {
        InteractionSession* sis = sorted_interaction_sessions->at(i);
        qDebug() << "sorted IS:" << sis->getMembersOneStr();
        QList<InteractionSession*> *forerunning_interaction_sessions = getForerunningInteractionSessions(sis, sorted_interaction_sessions, true);
        for (int j = 0; j < forerunning_interaction_sessions->size(); j++)
        {
            InteractionSession* fis = forerunning_interaction_sessions->at(j);
            qDebug() << "fore IS:" << fis->getMembersOneStr();
        }
    }

    int bottom_pos;
    int top_pos;

    /*for interaction_session in slot_base_layout.sessions_layout:*/
    for (int i = 0; i < sorted_interaction_sessions->size(); i++)
    {

        InteractionSession *interaction_session = sorted_interaction_sessions->at(i);
        QList<InteractionSession*> *forerunning_interaction_sessions = getForerunningInteractionSessions(interaction_session, sorted_interaction_sessions, true);
        SessionLayout *session_layout = slot_base_layout->getSessionsLayout()->value(interaction_session);
        QSet<QString> *static_lines = session_layout->static_lines;

        /*Assign positions to static lines*/
        if (static_lines->size() > 0) {
            QList<QList<InteractionSession*>*> *slot_list = slot_base_layout->getSlots();
            QSet<InteractionSession*> sessions_in_slot = slot_list->at(session_layout->getSlot())->toSet();
            QSet<InteractionSession*> proceeding_sessions = interaction_session->proceeding_interaction_sessions->toSet();
            QList<InteractionSession*> *common_sessions = getCommonInteractionSession(sessions_in_slot, proceeding_sessions);
            assert(common_sessions->size() == 1);
            InteractionSession* proceeding_interaction_session = common_sessions->at(0);
            SessionLayout* proceeding_session_layout = slot_base_layout->getSessionsLayout()->value(proceeding_interaction_session);
            bottom_pos = INT_MAX;
            top_pos = INT_MAX;

            QHash<QString, int> *proceeding_layout_of_the_static_characters = new QHash<QString, int>();
            QSet<QString>::iterator j = static_lines->begin();
            for (; j != static_lines->end(); j++) {
                QString member = (*j);
                int prev_pos = proceeding_session_layout->getSessionLayout()->value(member);
                assert(prev_pos != INT_MAX);
                proceeding_layout_of_the_static_characters->insert(member, prev_pos);
                //qDebug() << "proceeding layout of the static characters" << member << prev_pos;
                if (bottom_pos == INT_MAX || bottom_pos > prev_pos)
                    bottom_pos = prev_pos;
                if (top_pos == INT_MAX || top_pos < prev_pos)
                    top_pos = prev_pos;
            }
            /*Create the members list in ascending order of positions*/
            QList<QString>* members_ASC = new QList<QString>();
            QList<int> proc_ms_pos = proceeding_layout_of_the_static_characters->values();
            /*Sort the proceeding members' positions*/
            qSort(proc_ms_pos.begin(), proc_ms_pos.end());

//            for (int p = 0; p < proc_ms_pos.size(); p++)
//            {
//                qDebug() << "proc_ms_pos:" << proc_ms_pos.at(p);
//            }
            for (int p = 0; p < proc_ms_pos.size(); p++)
            {
                /*Get the members list in ascending order of positions*/
                QString key = proceeding_layout_of_the_static_characters->key(proc_ms_pos.at(p));
                members_ASC->append(key);
                qDebug() << "members_ASC" << key << proceeding_layout_of_the_static_characters->value(key);
            }

            /*Test out from bottom to top*/
            int minimum_penalty = static_lines->size();
            QHash<QString, int> *best_layout = new QHash<QString, int>();

            for (int k = bottom_pos; k < top_pos - static_lines->size() + 2; k++)
            {

                int penalty = 0;
                QHash<QString, int> *tmp_layout = new QHash<QString, int>();
                QHash<QString, int> *tmp_layout_copy = new QHash<QString, int>();
                for (int idx = 0; idx < members_ASC->size(); idx++)
                {
                    QString member = members_ASC->at(idx);
                    tmp_layout->insert(member, k+idx);

                    if (tmp_layout->value(member) != proceeding_layout_of_the_static_characters->value(member)) {
                        penalty += 1;
                    }
                }

                if  (penalty < minimum_penalty)
                {
                    minimum_penalty = penalty;

                    QHash<QString, int>::iterator it = tmp_layout->begin();
                    for (; it != tmp_layout->end(); it++)
                    {
                        QString mem_str = it.key();
                        int y = it.value();
                        //qDebug() << "tmp layout static lines' member and pos" << mem_str << y;
                        tmp_layout_copy->insert(mem_str, y);

                    }
                    best_layout = tmp_layout_copy;
                }
            }

            assert(best_layout->size() == static_lines->size());

            if (!best_layout->isEmpty()) {
                QHash<QString, int>::iterator i = best_layout->begin();
                for (;i != best_layout->end(); i++)
                {
                    QString member = i.key();
                    int pos = i.value();
                    qDebug() << "member and pos in static lines" << member << pos;
                    session_layout->getSessionLayout()->insert(member, pos);
                }
            }
        }

        /*Get bottom and top position in the current layout*/

        bottom_pos = INT_MAX;
        top_pos = INT_MAX;
        QHash<QString, int>::iterator iter = session_layout->getSessionLayout()->begin();

        for (;iter != session_layout->getSessionLayout()->end(); iter++)
        {
            int member_pos = iter.value();

            if (member_pos != INT_MAX)
            {
                if (bottom_pos == INT_MAX || member_pos < bottom_pos)
                    bottom_pos = member_pos;
                if (top_pos == INT_MAX || member_pos > top_pos)
                    top_pos = member_pos;
            }
        }
        if (bottom_pos == INT_MAX && top_pos == INT_MAX)
        {
            bottom_pos = 1;
            top_pos = 0;
        }

        /*Assign positions to rising lines*/

        QSet<QString> *rising_lines = session_layout->rising_lines;
        QHash<QString, int> *previous_positions = new QHash<QString, int>();
        if (!forerunning_interaction_sessions->isEmpty()) {
            for (int m = 0; m < forerunning_interaction_sessions->size(); m++)
            {
                InteractionSession* tmp_is = forerunning_interaction_sessions->at(m);

                SessionLayout* prev_session_layout = slot_base_layout->getSessionsLayout()->value(tmp_is);
                int prev_slot = prev_session_layout->getSlot();

                if (prev_slot < session_layout->getSlot()) {
                    QHash<QString, int> *prev_layout = prev_session_layout->getSessionLayout();

                    if (!rising_lines->isEmpty()) {
                        QSet<QString>::iterator n = rising_lines->begin();
                        for (; n != rising_lines->end(); n++)
                        {
                            QString member = (*n);
                            if (prev_layout->contains(member))
                            {
                                int prev_pos = prev_layout->value(member);

                                previous_positions->insert(member, prev_pos+1000*prev_slot); //

                            }
                        }
                    }
                }
            }
        }

        QVector<QPair <QString, int > > *rising_character_from_top_to_bottom = new QVector<QPair <QString, int > >();
        if (!previous_positions->isEmpty()) {
            QList<int> previous_positionsonly = previous_positions->values();
            qSort(previous_positionsonly.begin(), previous_positionsonly.end(), qGreater<int>());
            for (int r = 0; r < previous_positionsonly.size(); r++) {
                QString key = previous_positions->key(previous_positionsonly.at(r));
                qDebug() << "prev position of" << key << previous_positionsonly.at(r) << "is rising";
                int value = previous_positions->value(key);
                QPair<QString, int> item;
                item.first = key;
                item.second = value;
                rising_character_from_top_to_bottom->push_back(item);
            }

            QHash<QString, int> *s_layout = session_layout->getSessionLayout();
            QVector<QPair <QString, int> >::iterator j = rising_character_from_top_to_bottom->begin();

            for (; j != rising_character_from_top_to_bottom->end(); j++) {

                QString member = j->first;
                qDebug() << member << "is located at" << bottom_pos - 1;
                s_layout->insert(member, bottom_pos - 1);
                bottom_pos = bottom_pos - 1;
            }
        }

        /*Assign positions to dropping lines*/

        QSet<QString> *dropping_lines = session_layout->dropping_lines;
        previous_positions = new QHash<QString, int>();

        if (!forerunning_interaction_sessions->isEmpty()) {
            for (int k = 0; k < forerunning_interaction_sessions->size(); k++)
            {
                InteractionSession* tmp_is = forerunning_interaction_sessions->at(k);
                SessionLayout* prev_session_layout = slot_base_layout->getSessionsLayout()->value(tmp_is);
                int prev_slot = prev_session_layout->getSlot();
//                if (tmp_is->getMembersOneStr().compare("D,E,F,G") == 0)
//                {
//                    qDebug() << "the prev slot is" << prev_slot << "the current slot is" << session_layout->getSlot();
//                }
                if (prev_slot > session_layout->getSlot())
                {

                    QHash<QString, int> *prev_layout = prev_session_layout->getSessionLayout();
                    if (!dropping_lines->isEmpty()) {
                        QSet<QString>::iterator l = dropping_lines->begin();
                        for (; l != dropping_lines->end(); l++)
                        {
                            QString member = (*l);
                            if (prev_layout->keys().contains(member))
                            {
                                int prev_pos = prev_layout->value(member);
                                //qDebug() << member << prev_pos;
                                previous_positions->insert(member, prev_pos+1000*prev_slot); //
                            }
                        }
                    }
                }
            }
        }

        QVector<QPair <QString, int > > *dropping_character_from_bottom_to_top = new QVector<QPair <QString, int > >();

        if (!previous_positions->isEmpty()) {
            QList<int> previous_positionsonly = previous_positions->values();
            qSort(previous_positionsonly.begin(), previous_positionsonly.end());
            for (int r = 0; r < previous_positionsonly.size(); r++) {

                QString key = previous_positions->key(previous_positionsonly.at(r));
                qDebug() << "prev position of" << key << previous_positionsonly.at(r) << "is dropping";
                int value = previous_positions->value(key);
                QPair<QString, int> item;
                item.first = key;
                item.second = value;
                dropping_character_from_bottom_to_top->push_back(item);
            }

            QHash<QString, int> *s_layout = session_layout->getSessionLayout();
            QVector<QPair <QString, int> >::iterator it = dropping_character_from_bottom_to_top->begin();

            for (; it != dropping_character_from_bottom_to_top->end(); it++) {

                QString member = it->first;
                qDebug() << member << "is located at" << top_pos + 1;
                s_layout->insert(member, top_pos + 1);
                top_pos = top_pos + 1;
            }


        }

        /*Assign positions to emerging lines*/

        QSet<QString> *emerging_lines_will_rise = session_layout->emerging_lines_rising;
        QSet<QString> *emerging_lines_will_drop = session_layout->emerging_lines_dropping;
        QSet<QString> *emerging_lines_will_die = session_layout->emerging_lines_dying;
        if (!emerging_lines_will_rise->isEmpty()) {
            QSet<QString>::iterator q = emerging_lines_will_rise->begin();
            for (; q != emerging_lines_will_rise->end(); q++)
            {
                QString member = (*q);

                QHash<QString, int> *s_layout = session_layout->getSessionLayout();
                s_layout->insert(member, top_pos + 1);
                qDebug() << "will rise" << member << top_pos + 1;
                top_pos = top_pos + 1;
            }
        }
        if (!emerging_lines_will_drop->isEmpty()) {
            QSet<QString>::iterator w = emerging_lines_will_drop->begin();
            for (; w != emerging_lines_will_drop->end(); w++)
            {
                QString member = (*w);
                QHash<QString, int> *s_layout = session_layout->getSessionLayout();
                s_layout->insert(member, bottom_pos - 1);
                qDebug() << "will drop" << member << bottom_pos-1;
                bottom_pos = bottom_pos - 1;
            }
        }
        if (!emerging_lines_will_die->isEmpty()) {
            QSet<QString>::iterator e = emerging_lines_will_die->begin();
            for (; e != emerging_lines_will_die->end(); e++)
            {
                QString member = (*e);
                QHash<QString, int> *s_layout = session_layout->getSessionLayout();
                s_layout->insert(member, bottom_pos - 1);
                qDebug() << "will die" << member << bottom_pos-1;
                bottom_pos = bottom_pos - 1;
            }
        }
    }

}


void rearrangeLineSegments(SlotBaseLayout *slot_base_layout)
{
    classifyLineSegments(slot_base_layout);
    assignMemberAlignment(slot_base_layout);
}

SlotSegments* generateSlotSegments(SlotBaseLayout *slot_base_layout)
{
    QHash<InteractionSession*, SessionLayout*>* sessions_layout = slot_base_layout->getSessionsLayout();

    SlotSegments* slot_segments = new SlotSegments(SLOTS);
    QList<QList<InteractionSession*>*> *slot_list = slot_base_layout->getSlots();

    if (!slot_list->isEmpty()) {

        for (int slot_idx = 0; slot_idx < slot_list->size(); slot_idx++)
        {

            if (slot_list->at(slot_idx)->size() > 0) {
                /*Sort interaction sessions by its starting time*/
                QList<InteractionSession*>* sorted_interaction_sessions = slot_list->at(slot_idx);                
                qSort(sorted_interaction_sessions->begin(), sorted_interaction_sessions->end(), startTimeLessThen);

                for (int j = 0; j < sorted_interaction_sessions->size(); j++)
                {
                    InteractionSession *interaction_session = sorted_interaction_sessions->at(j);
                    QStringList members = interaction_session->getMembers();
                    SessionLayout *session_layout = sessions_layout->value(interaction_session);
                    assert(session_layout != NULL);
                    QList<InteractionSession*> *proceeding_interaction_sessions = interaction_session->proceeding_interaction_sessions;
                    qDebug() << "IS: " << interaction_session->getMembersOneStr() << session_layout->getSlot();
                    if (!proceeding_interaction_sessions->isEmpty()) {
                        for (int k = 0; k < proceeding_interaction_sessions->size(); k++)
                        {
                            InteractionSession *proceeding_interaction_session = proceeding_interaction_sessions->at(k);
                            SessionLayout *proc_session_layout = sessions_layout->value(proceeding_interaction_session);
                            QList<QString> common_members;
                            QSet<QString> ms = members.toSet();
                            QSet<QString> pms = proceeding_interaction_session->getMembers().toSet();
                            common_members = pms.intersect(ms).toList();
                            qDebug() << "PIS: " << proceeding_interaction_session->getMembersOneStr() << proc_session_layout->getSlot();
                            if (common_members.size() > 0) {
                                /*These interaction sessions are connected*/
                                for (int k = 0; k < common_members.size(); k++) {
                                    qDebug() << common_members.at(k);
                                }
                                SlotSegment *proceeding_IS_slot_segment = slot_segments->belongToSlotSegment(proceeding_interaction_session, slot_idx);
                                if (proceeding_IS_slot_segment == NULL)
                                {
                                }
                                else
                                {
                                    /*These interaction sessions belong to the same slot*/
                                    proceeding_IS_slot_segment->setInteractionSession(interaction_session, session_layout);
                                    qDebug() << "This IS" << interaction_session->getMembersOneStr() << "belongs to the PIS's' slot segment";
                                    break;
                                }
                            }
                        }
                    }
                    SlotSegment* IS_slot_segment = slot_segments->belongToSlotSegment(interaction_session, slot_idx);

                    if (IS_slot_segment == NULL)
                    {
                        SlotSegment* new_slot_segment = new SlotSegment();
                        new_slot_segment->setInteractionSession(interaction_session, session_layout);
                        qDebug() << "The new slot segment emerges for this IS:" << interaction_session->getMembersOneStr();
                        QList<SlotSegment *>* slot_segment_list = slot_segments->getSlotSegments()->at(slot_idx);

                        if (slot_segment_list == NULL) {
                            slot_segment_list = new QList<SlotSegment *>();
                            //qDebug() << "no slot, we make new slot";
                            slot_segment_list->append(new_slot_segment);
                            slot_segments->getSlotSegments()->insert(slot_idx, slot_segment_list);
                        }
                        else {
                            //qDebug() << "append this segment to the slot";
                            slot_segment_list->append(new_slot_segment);
                            slot_segments->getSlotSegments()->replace(slot_idx, slot_segment_list);
                        }
                    }
                }
            }
        }
    }
    return slot_segments;
}

void adjustSlotSegments(SlotSegments* slot_segments, QList<SlotLayout*>* slot_layouts, int timesteps)
{

    /*Set center slot layout*/
    QList<QList<SlotSegment *> *> * _slot_segments = slot_segments->getSlotSegments();

//    /*Check  if the slot segments are correctly stored*/ -> Checked!
    qDebug() << "Check if the slot segments are correctly stored";
    for (int i = 0; i < _slot_segments->size(); i++)
    {
        QList<SlotSegment *> *_slots = _slot_segments->at(i);
        for (int j = 0; j < _slots->size(); j++)
        {
            SlotSegment *segment = _slots->at(j);
            QList<InteractionSession*> *slotSegmentISs = segment->getInteractionSessions();
            /*In one segment*/
            for (int l = 0; l < slotSegmentISs->size(); l++)
            {

                InteractionSession* is = slotSegmentISs->at(l);
                qDebug() << l << is->getMembersOneStr() << is->getStartTime() << is->getEndTime();
            }
        }
    }

    int center_slot_idx = SLOTS/2;

    assert(slot_layouts->at(center_slot_idx)->getSlotLayout()->size() == timesteps);
    for (int i = 0; i < slot_layouts->at(center_slot_idx)->getSlotLayout()->size(); i++)
    {
        slot_layouts->at(center_slot_idx)->setItem(i, 0, 0);
    }

    QList<SlotSegment *> *slot_segment_ary = _slot_segments->at(center_slot_idx);

    if (slot_segment_ary->size() > 0) {
        for (int j = 0; j < slot_segment_ary->size(); j++)
        {
            SlotSegment *slot_segment = slot_segment_ary->at(j);
            QHash<int, int> *bottom_coords = slot_segment->bottom_coordinates;
            QHash<int, int> *top_coords = slot_segment->top_coordinates;

            QHash<int, int>::iterator it = bottom_coords->begin();
            for (; it != bottom_coords->end(); it++)
            {
                int time_step = it.key();
                int bottom_val = it.value();
                int top_val = top_coords->value(time_step);

                int height = top_val - bottom_val;
                slot_layouts->at(center_slot_idx)->setItem(time_step, bottom_val, height);
            }
        }

    }
    //qDebug() << "center done";
    /*Set the layout for the slots above center slot
    **Shift each slots down toward the center slot one by one.
    */
    for (int slot_idx = center_slot_idx + 1; slot_idx < SLOTS; slot_idx++)
    {
        /*Set base to the slot directly under*/
        int under_slot_idx = slot_idx - 1;

        for (int time_step = 0; time_step < timesteps; time_step++)
        {
            QPair<int, int> *under = slot_layouts->at(under_slot_idx)->getSlotLayout()->value(time_step);
            int bottom_under = under->first;
            int height_under = under->second;
            assert((bottom_under != INT_MAX) && (height_under != INT_MAX));
            slot_layouts->at(slot_idx)->setItem(time_step, bottom_under + height_under, 0);
        }

        /*Stack slot segments on top of one another*/
        QList<SlotSegment*> *slot_segment_ary = slot_segments->getSlotSegments()->at(slot_idx);

        if (slot_segment_ary->size() > 0) {
            for (int s = 0; s < slot_segment_ary->size(); s++)
            {
                SlotSegment *slot_segment = slot_segment_ary->at(s);
                QHash<int, int> *bottom_coords = slot_segment->bottom_coordinates;
                QHash<int, int> *top_coords = slot_segment->top_coordinates;

                /*Retrieve the biggest offset*/
                int highest_base = INT_MAX;
                QHash<int, int>::iterator bot = bottom_coords->begin();
                for (; bot != bottom_coords->end(); bot++)
                {
                    int time_step = bot.key();
                    int bottom_val = bot.value();

                    SlotLayout* slot_layout = slot_layouts->at(slot_idx);
                    if (!slot_layout->getSlotLayout()->isEmpty()) {

                        QPair<int, int> *min = slot_layout->getSlotLayout()->value(time_step);
                        //qDebug() << "timestep" << time_step;
                        if (min == NULL) {
                            qDebug() << "timestep" << time_step;
                            exit(3);
                        }
                        else {
                            int min_base = min->first;

                            int necessary_base_val = min_base - bottom_val + OFFSET;
                            if (necessary_base_val > highest_base || highest_base == INT_MAX)
                            {
                                highest_base = necessary_base_val;
                            }
                        }
                    }
                }
                /*Bump up the layout of the slot for the offset*/
                assert(highest_base != INT_MAX);
                bot = bottom_coords->begin();
                for (; bot != bottom_coords->end(); bot++)
                {
                    int time_step = bot.key();
                    int height = top_coords->value(time_step) - bot.value() + 1;
                    assert(height > 0);
                    slot_layouts->at(slot_idx)->setItem(time_step, highest_base, height);
                }
            }
        }
    }
    //qDebug() << "above done";
    /*Set the layout for the slots under center slot*/
    for (int i = 0; i < center_slot_idx - 1; i++)
    {
        int slot_idx = center_slot_idx - 1 - i;

        /*Set base to the slot directly above*/
        int above_slot_idx = slot_idx + 1;
        for (int time_step = 0; time_step < timesteps; time_step++)
        {
            QPair<int, int> *above = slot_layouts->at(above_slot_idx)->getSlotLayout()->value(time_step);
            int bottom_above = above->first;
            int height_above = above->second;
            assert(bottom_above != INT_MAX && height_above != INT_MAX);
            slot_layouts->at(slot_idx)->setItem(time_step, bottom_above, 0);

        }
        /*Stack slot segments on top of one another*/
        QList<SlotSegment*> *slot_segment_ary = slot_segments->getSlotSegments()->at(slot_idx);
        if (slot_segment_ary->size() > 0) {

            for (int ss = 0; ss < slot_segment_ary->size(); ss++)
            {
                SlotSegment *slot_segment = slot_segment_ary->at(ss);
                QHash<int, int> *bottom_coords = slot_segment->bottom_coordinates;
                QHash<int, int> *top_coords = slot_segment->top_coordinates;

                /*Retrieve the biggest offset*/
                int lowest_base = INT_MAX;
                QHash<int, int>::iterator top = top_coords->begin();
                for (; top != top_coords->end(); top++)
                {
                    int time_step = top.key();
                    int top_val = top_coords->value(time_step);

                    SlotLayout* slot_layout = slot_layouts->at(slot_idx);
                    if (!slot_layout->getSlotLayout()->isEmpty()) {
                        QPair<int, int> *min = slot_layout->getSlotLayout()->value(time_step);
                        if (min == NULL) {
                            qDebug() << "min is NULL";
                            exit(3);
                        }
                        else {
                            int min_base = min->first;
                            int height = top_val - bottom_coords->value(time_step) + 1;
                            int necessary_base_val =  min_base - height - OFFSET;
                            if (necessary_base_val < lowest_base || lowest_base == INT_MAX) {
                                lowest_base = necessary_base_val;
                            }
                        }
                    }
                }
                /*Bump up the layout of the slot for the offset*/
                assert(lowest_base != INT_MAX);
                QHash<int, int>::iterator bot = bottom_coords->begin();
                for (; bot != bottom_coords->end(); bot++)
                {
                    int time_step = bot.key();
                    int height = top_coords->value(time_step) - bot.value() + 1;
                    assert(height > 0);
                    slot_layouts->at(slot_idx)->setItem(time_step, lowest_base, height);                    
                }
            }
        }
    }
    //qDebug() << "under done";
}

void removeWhiteSpace(SlotBaseLayout* slot_base_layout, QList<SlotLayout*>* slot_layouts, int time_steps)
{
    SlotSegments* slot_segments = generateSlotSegments(slot_base_layout);
    //qDebug() << "generated slot segments";
    adjustSlotSegments(slot_segments, slot_layouts, time_steps);
    //qDebug() << "adjusted slot segments";
}

void relaxLines(QHash<QString, QHash<int, int>* > *y_coords, QList<QPair<InteractionSession*, int>*>* slot_assigned_interaction_sessions)
{
    /*Sort all interaction sessions in chronological order*/
    /*Need sort for streaming? I don't think so*/
    QList<InteractionSession*> *sorted_interaction_sessions = new QList<InteractionSession*>();
    for (int i = 0; i < slot_assigned_interaction_sessions->size(); i++)
    {
        sorted_interaction_sessions->append(slot_assigned_interaction_sessions->at(i)->first);
    }
    //QList<InteractionSession*> *sorted_interaction_sessions = slot_assigned_interaction_sessions;
    qSort(sorted_interaction_sessions->begin(), sorted_interaction_sessions->end(), startTimeLessThen);

    /*Check any subsequent pair of interaction sessions are relaxable
    **(subset member of the other)
    */
    for (int i = 0; i < slot_assigned_interaction_sessions->size(); i++)
    {
        QPair<InteractionSession*, int> *pair = slot_assigned_interaction_sessions->at(i);
        InteractionSession* interaction_session = pair->first;
        QList<InteractionSession*> *forerunning_interaction_sessions = getForerunningInteractionSessions(interaction_session, sorted_interaction_sessions, true);
        QStringList members = interaction_session->getMembers();

        QSet<QString> members_set = members.toSet();
        QSet<QString>::iterator m = members_set.begin();
        for (int j = 0; j < forerunning_interaction_sessions->size(); j++)
        {
            InteractionSession* forerunning_interaction_session = forerunning_interaction_sessions->at(j);
            QStringList forerunning_members = forerunning_interaction_session->getMembers();

            bool is_is_subset_of_fis = false;
            bool fis_is_subset_of_is = false;
            QSet<QString> forerunning_members_set = forerunning_members.toSet();
            /*Check interaction session is subset of forerunning_interaction_session*/
            is_is_subset_of_fis = forerunning_members_set.contains(members_set);
            /*Check forerunning_interaction_session is subset of interaction_session*/
            fis_is_subset_of_is = members_set.contains(forerunning_members_set);
            assert(is_is_subset_of_fis == false || fis_is_subset_of_is == false);

            /*The right interaction session is subset of the left*/
            if (is_is_subset_of_fis)
            {
                //int best_match_diff = INT_MAX;
                //int best_match_time_step = -1;
                int start_time = interaction_session->getStartTime();
                int end_time = interaction_session->getEndTime();                
                QList<int> *diffs = new QList<int>();
                QList<int> *without_diffs = new QList<int>();
                float slope = INT_MAX;
                float wo_slope = INT_MAX;
                int duration;
                QHash<int, int> *member_coords;
                int member_coord_of_the_member_at_the_start_time;
                int member_coord_of_the_member_right_before_the_start_time;
                int member_coord_of_the_member_at_the_end_time;
                int member_coord_of_the_member_right_before_the_end_time;

                try {
                    for (int r = 0; r < members.size(); r++) {
                        QString member = members.at(r);
                        member_coords = y_coords->value(member);
                        QHash<int, int>::iterator y = member_coords->begin();
                        for (; y != member_coords->end(); y++) {

                            if (y.key() == start_time) {
                                member_coord_of_the_member_at_the_start_time = y.value();
                            }
                            else if (y.key() == end_time) {
                                member_coord_of_the_member_at_the_end_time = y.value();
                            }
                            else if (y.key() == start_time - 1) {
                                member_coord_of_the_member_right_before_the_start_time = y.value();
                            }
                            else if (y.key() == end_time - 1) {
                                member_coord_of_the_member_right_before_the_end_time = y.value();
                            }
                        }

                        int prev_differential = abs(member_coord_of_the_member_at_the_start_time - member_coord_of_the_member_right_before_the_start_time);
                        int next_differential = abs(member_coord_of_the_member_at_the_end_time - member_coord_of_the_member_right_before_the_end_time);
                        diffs->append(prev_differential + next_differential);
                        int without_diff = abs(member_coord_of_the_member_at_the_end_time - member_coord_of_the_member_right_before_the_start_time);
                        without_diffs->append(without_diff);

                    }
                    int sum = 0;
                    int sum_wo = 0;
                    for (int i = 0; i < diffs->size(); i++) {
                        sum = sum + diffs->at(i);
                    }
                    for (int j = 0; j < without_diffs->size(); j++) {
                        sum_wo = sum_wo + without_diffs->at(j);
                    }
                    float avg_diff = float(float(sum)/float(diffs->size()));
                    float avg_wo_diff = float(float(sum_wo)/float(without_diffs->size()));
                    duration = abs(end_time - start_time);
                    slope = float(avg_diff/duration);
                    wo_slope = float(avg_wo_diff/duration);
                }
                catch (float slope) {
                    slope = OBJECTIVE_SLOPE;
                }
                catch (float wo_slope) {
                    wo_slope = OBJECTIVE_SLOPE + 1.0;
                }
                try {
                    if (duration < 3)
                    {
                        /*Apply relaxation*/
                        for (int m = 0; m < members.size(); m++) {
                            QString member = members.at(m);                        
                            member_coords = y_coords->value(member);
                            QHash<int, int>::iterator y = member_coords->begin();
                            for (; y != member_coords->end(); y++) {
                                if (y.key() == end_time) {
                                    member_coord_of_the_member_at_the_end_time = y.value();
                                }
                                else if (y.key() == start_time - 1) {
                                    member_coord_of_the_member_right_before_the_start_time = y.value();
                                }
                            }
                            int pos_1 = member_coord_of_the_member_right_before_the_start_time;
                            int pos_2 = member_coord_of_the_member_at_the_end_time;
                            for (int time_step = start_time; time_step <= end_time; time_step++)
                            {
                                member_coords->insert(time_step, float(float(end_time - time_step)/duration)*pos_1+float(float(time_step - start_time)/duration)*pos_2);
                            }
                        }
                    }
                }
                catch (int m) {
                    //do nothing
                }
            }
            else if (fis_is_subset_of_is)
            {
                //int best_match_diff = INT_MAX;
                //int best_match_time_step = -1;
                int start_time = forerunning_interaction_session->getStartTime();
                int end_time = forerunning_interaction_session->getEndTime();
                QList<int> *diffs = new QList<int>();
                QList<int> *without_diffs = new QList<int>();
                float slope = INT_MAX;
                float wo_slope = INT_MAX;
                int duration;
                QHash<int, int> *member_coords;
                int member_coord_of_the_member_at_the_start_time;
                int member_coord_of_the_member_right_before_the_start_time;
                int member_coord_of_the_member_at_the_end_time;
                int member_coord_of_the_member_right_before_the_end_time;
                try {
                    for (int r = 0; r < members.size(); r++) {
                        QString member = members.at(r);
                        member_coords = y_coords->value(member);
                        QHash<int, int>::iterator y = member_coords->begin();
                        for (; y != member_coords->end(); y++) {

                            if (y.key() == start_time) {
                                member_coord_of_the_member_at_the_start_time = y.value();
                            }
                            else if (y.key() == end_time) {
                                member_coord_of_the_member_at_the_end_time = y.value();
                            }
                            else if (y.key() == start_time - 1) {
                                member_coord_of_the_member_right_before_the_start_time = y.value();
                            }
                            else if (y.key() == end_time - 1) {
                                member_coord_of_the_member_right_before_the_end_time = y.value();
                            }
                        }
                        int prev_differential = abs(member_coord_of_the_member_at_the_start_time - member_coord_of_the_member_right_before_the_start_time);
                        int next_differential = abs(member_coord_of_the_member_at_the_end_time - member_coord_of_the_member_right_before_the_end_time);
                        diffs->append(prev_differential + next_differential);
                        int without_diff = abs(member_coord_of_the_member_at_the_end_time - member_coord_of_the_member_right_before_the_start_time);
                        without_diffs->append(without_diff);
                    }
                    int sum = 0;
                    int wo_sum = 0;
                    for (int l = 0; l < diffs->size(); l++) sum += diffs->at(l);
                    for (int h = 0; h < without_diffs->size(); h++) wo_sum += without_diffs->at(h);
                    float avg_diff = float(float(sum)/float(diffs->size()));
                    float avg_wo_diff = float(float(wo_sum)/float(without_diffs->size()));
                    slope = float(avg_diff/duration);
                    wo_slope = float(avg_wo_diff/duration);
                }
                catch(float slope) {
                    slope = OBJECTIVE_SLOPE;
                }
                catch(float wo_slope) {
                    wo_slope = OBJECTIVE_SLOPE + 1.0;
                }
                try {
                    if (duration < 3)
                    {
                        /*Apply relaxation*/
                        for (int m = 0; m < members.size(); m++) {
                            QString member = members.at(m);
                            member_coords = y_coords->value(member);
                            QHash<int, int>::iterator y = member_coords->begin();
                            for (; y != member_coords->end(); y++) {
                                if (y.key() == end_time) {
                                    member_coord_of_the_member_at_the_end_time = y.value();
                                }
                                else if (y.key() == start_time - 1) {
                                    member_coord_of_the_member_right_before_the_start_time = y.value();
                                }
                            }
                            int pos_1 = member_coord_of_the_member_right_before_the_start_time;
                            int pos_2 = member_coord_of_the_member_at_the_end_time;
                            for (int time_step = start_time; time_step < end_time+1; time_step++)
                            {
                                member_coords->insert(time_step, float(float(end_time - time_step)/duration)*pos_1+float(float(time_step - start_time)/duration)*pos_2);
                            }
                        }
                    }
                }
                catch (int m) {
                    //do nothing
                }
            }
        }
    }
}

//QHash<QString, QHash<int, int>*> *generateLayout(QList<int> *genome, InteractionSessionContainer *is_container)
//{

//    QList<InteractionSession*> *interaction_sessions = is_container->getISList();
//    //QList<InteractionSession*> *interaction_sessions = is_container->getTempISList_now();
//    SlotBaseLayout *slot_base_layout = decodeGenome(genome, interaction_sessions);

//    if (slot_base_layout == NULL) {
//        /*No feasible layout with this genome sequence*/
//        return NULL;
//    }

//    rearrangeLineSegments(slot_base_layout);

//    int time_steps = interaction_sessions->last()->getEndTime()+1;

//    QList<SlotLayout*> *slot_layouts = new QList<SlotLayout*>();
//    for (int i = 0; i < SLOTS; i++) {
//        SlotLayout *slot_layout = new SlotLayout(time_steps);
//        slot_layouts->append(slot_layout);
//    }

//    removeWhiteSpace(slot_base_layout, slot_layouts, time_steps);

//    QHash<QString, QHash<int, int>*> *y_coords = new QHash<QString, QHash<int, int>*>();
//    QHash<InteractionSession*, SessionLayout*>::iterator j = slot_base_layout->getSessionsLayout()->begin();
//    for (; j != slot_base_layout->getSessionsLayout()->end(); j++)
//    {
//        InteractionSession* interaction_session = j.key();
//        SessionLayout* session_layout = j.value();
//        int end_time = interaction_session->getEndTime();
//        int start_time = interaction_session->getStartTime();

//        QHash<QString, int>* layout = session_layout->getSessionLayout();
//        int slot_number = session_layout->getSlot();
//        SlotLayout* slot_layout = slot_layouts->at(slot_number);
//        QHash<QString, int>::iterator k = layout->begin();
//        for (; k != layout->end(); k++)
//        {
//            QString member = k.key();
//            int y_in_slot = k.value();
//            QHash<int, int> *y_with_timestep = y_coords->value(member);
//            if (y_with_timestep == NULL)
//                y_with_timestep = new QHash<int, int>();

//            for (int time_step = start_time; time_step < end_time; time_step++)
//            {
//                QPair<int, int> *size = slot_layout->getSlotLayout()->value(time_step);
//                int bottom_base = size->first;
//                y_with_timestep->insert(time_step, y_in_slot+bottom_base);
//                y_coords->insert(member, y_with_timestep);
//            }
//        }
//    }

//    relaxLines(y_coords, interaction_sessions);

//    return y_coords;
//}

float evaluateLayout(QHash<QString, QHash<int, int> *> *layout, int time_steps)
{    

//    if (time_steps > 18)
//    {
//        sleep(1);
//    }

    /*Definition : layout[member][time_step] == the y coordinate of the member at the time_step*/
    if (layout == NULL) {
        return -1;
    }
    /*Count deviations*/
    float overall_deviations = 0;

    QHash<QString, QHash<int, int> *>::iterator it = layout->begin();

    for (; it != layout->end(); it++) {
        float deviations = 0;
        QString member = it.key();
        QHash<int, int> *member_layout = it.value();
        QList<int> member_timesteps = member_layout->keys();




        for (int time_step_idx = 1; time_step_idx < member_timesteps.size(); time_step_idx++)
        {
//            if (!layout->value(member)->contains(time_step_idx - 1)
//                    || !layout->value(member)->contains(time_step_idx))
//            {
//                qDebug() << "This member is not in this timestep"<< member;
//                continue;
//            }



            int previous_timestep = member_timesteps.at(time_step_idx - 1);
            int current_timestep = member_timesteps.at(time_step_idx);

            if (previous_timestep != current_timestep - 1)
                continue;

    //            if (!member_layout->contains(previous_timestep)
    //                    || !member_layout->contains(current_timestep))
    //            {
    //                qDebug() << "This member is not in this timestep"<< member;
    //                continue;
    //            }
                int previous_y = member_layout->value(previous_timestep);
                int current_y = member_layout->value(current_timestep);



                qDebug() << "dev:" << member << "prev_timestep" << previous_timestep << "current_timestep" << current_timestep << "prev_y" << previous_y << "current_y" << current_y;
                if (previous_y != INT_MAX && current_y != INT_MAX)
                {
                    if (previous_y != current_y) {
                        float diff = abs(float(current_y) - float(previous_y));

                        deviations += diff;
                    }
                }

        }
        overall_deviations += deviations;
    }
    /*Count crossovers*/
    float crossovers = 0;

    QList<QString> members = layout->keys();

//    for (int i = 0; i < members.size(); i++) {
//        qDebug() << "members [" << i << "]: " << members.value(i);
//    }

//    for (int time_step = 1; time_step < time_steps; time_step++)
    int time_step = time_steps -1;
    if(time_step > 0)
    {
        for (int i = 0; i < members.size() - 1; i++)
        {
            for (int j = i; j < members.size(); j++)
            {
                if (!layout->value(members.at(i))->contains(time_step - 1)
                            || !layout->value(members.at(i))->contains(time_step)
                               || !layout->value(members.at(j))->contains(time_step - 1)
                                  || !layout->value(members.at(j))->contains(time_step)) continue;

                int previous_i = layout->value(members.at(i))->value(time_step - 1);
                int previous_j = layout->value(members.at(j))->value(time_step - 1);
                int current_i = layout->value(members.at(i))->value(time_step);
                int current_j = layout->value(members.at(j))->value(time_step);
                qDebug() << "eval: prev_i" << previous_i << "of" << members.at(i) << "prev_j" << previous_j << "of" << members.at(j) << "current_i" << current_i << "of" << members.at(i) << "current_j" << current_j << "of" << members.at(j);
                //if ((previous_i - current_i) * (previous_j - current_j) < 0) {
                //if ((previous_i - previous_j) * (current_i - current_j) < 0) {
                if ( (members.at(i).compare(members.at(j)) != 0 && ((previous_i == previous_j) && (current_i == current_j))) || ((previous_i > previous_j) && (current_i < current_j)) || ((previous_i < previous_j) && (current_i > current_j))) {
                    crossovers += 1.0;
                    qDebug() << "eval: crossover++";
                }

            }
        }
    }

    /*Count visualization space*/
    float screen_height = 0;
    int screen_top = INT_MAX;
    int screen_bottom = INT_MAX;
    it = layout->begin();
    for (; it != layout->end(); it++)
    {
        QString member = it.key();
        QHash<int, int> *position_ary = it.value();
        QList<int> position_ary_ylist = position_ary->values();
        qSort(position_ary_ylist.begin(), position_ary_ylist.end());
        int top_val = position_ary_ylist.last();
        int bottom_val = position_ary_ylist.first();

        if (screen_top == INT_MAX)
        {
            screen_top = top_val;
        }
        else
        {
            screen_top = qMax(screen_top, top_val);
        }
        if (screen_bottom == INT_MAX)
        {
            screen_bottom = bottom_val;
        }
        else
        {
            screen_bottom = qMax(screen_bottom, bottom_val);
        }
    }
    screen_height = float(float(screen_top) - float(screen_bottom));

    float fittness = float(overall_deviations * float(deviation)) + float(crossovers * float(crossover)) + float(screen_height * float(whitespace));
    qDebug() << "in the evaluation function" << fittness << "[" << float(overall_deviations) << ", " << float(crossovers) << ", " << float(screen_height) << "]";

    return fittness;
}

//QPair<QHash<QString, QHash<int, int>* >*, int> *evaluateGenomes(QList<QList<int>* > *genome_pool, InteractionSessionContainer *is_container, QHash<int, int> *fittness_cache, int best_fittness, QHash<QString, QHash<int, int>* >*best_layout)
//{
//    //QList<InteractionSession*> *interaction_sessions = is_container->getISList();
//    QList<InteractionSession*> *interaction_sessions = is_container->getTempISList_now();
//    int time_steps = interaction_sessions->last()->getEndTime()

//    QPair<QHash<QString, QHash<int, int>* >*, int> *best;
//    for (int idx = 0; idx < genome_pool->size(); idx++)
//    {

//        QList<int> *genome = genome_pool->at(idx);
//        QString gene_str = "";
//        for (int gene_idx = 0; gene_idx < genome->size(); gene_idx++) {
//            gene_str.append(QString::number(genome->at(gene_idx)));
//        }
//        int genome_hash = qHash(gene_str);

//        if (!fittness_cache->contains(genome_hash))
//        {

//            QHash<QString, QHash<int, int>*> *layout = generateLayout(genome, is_container);
//            //qDebug() << "layout generated";
//            int fittness = evaluateLayout(layout, time_steps);
//            fittness_cache->insert(genome_hash, fittness);
//            if (fittness >= 0)
//            {
//                if (fittness < best_fittness || best_fittness == 0)
//                {
//                    best_layout = layout;
//                    best_fittness = fittness;
//                }
//            }
//        }
//        best = new QPair<QHash<QString, QHash<int, int>* >*, int>(best_layout, best_fittness);
//    }

//    return best;
//}

//QList<QList<int>*> *getEliteGenomes(QList<QList<int>*> *genome_pool, QHash<int, int> *fittness_cache)
//{
//    /*Get Elite Genomes*/
//    QList<int> *genome_fittness = new QList<int>();
//    QList<QPair<int, int> > *genome_idx_and_fittness = new QList<QPair<int, int> >();
//    for (int idx = 0; idx < genome_pool->size(); idx++)
//    {
//        QList<int>* genome = genome_pool->at(idx);
//        QString gene_str = "";
//        for (int gene_idx = 0; gene_idx < genome->size(); gene_idx++) {
//            gene_str.append(QString::number(genome->at(gene_idx)));
//        }
//        int genome_hash = qHash(gene_str);
//        int fittness = fittness_cache->value(genome_hash);
//        genome_fittness->append(fittness);
//        QPair<int, int> pair_idx_fittness;
//        pair_idx_fittness.first = idx;
//        pair_idx_fittness.second = fittness;
//        genome_idx_and_fittness->append(pair_idx_fittness);
//    }

//    /*Genome Sort: sorting genome index according to genome fittness*/
//    QList<int> *genome_idx_ASC = new QList<int>();
//    qSort(genome_idx_and_fittness->begin(), genome_idx_and_fittness->end(), fittnessLessThen);

//    for (int i = 0; i < genome_idx_and_fittness->size(); i++) {
//        QPair<int, int> pair = genome_idx_and_fittness->at(i);
//        if (pair.second != -1) {
//            genome_idx_ASC->append(pair.first);
//        }
//    }
//    QList<int> *elite_idx_ary = new QList<int>();

//    for (int i = 0; i < genome_idx_ASC->size(); i++) {

//        if (i == ELITE_POPULATION) {
//            break;
//        }
//        elite_idx_ary->append(genome_idx_ASC->at(i));
//    }

//    QList<QList<int> *> *elite_genomes = new QList<QList<int>*>();
//    for (int i = 0; i < elite_idx_ary->size(); i++) {
//        int idx = elite_idx_ary->at(i);
//        QString elite_genomes_str = "";
//        for (int j = 0; j < genome_pool->at(idx)->size(); j++) {
//            elite_genomes_str.append(QString::number(genome_pool->at(idx)->at(j)));
//        }
//        elite_genomes->append(genome_pool->at(idx));
//    }

//    return elite_genomes;
//}

//QList<QList<int>*> *renewGenomes(QList<QList<int>*> *genome_pool, QHash<int, int> *fittness_cache)
//{
//    /*Renew Genomes*/
//    QList<QList<int>*> *elite_genomes = getEliteGenomes(genome_pool, fittness_cache);
//    QList<QList<int>*> *new_genome_pool = elite_genomes;
//    int elite_genomes_count = elite_genomes->size();

//    QTime time = QTime::currentTime();
//    qsrand((uint)time.msec());

//    /*Generate the new generation pool*/
//    while (new_genome_pool->size() < POPULATION) {

//        QList<int> *randomGenomes = new QList<int>();

//        for (int i = 0; i < 2; i++) {
//            int random_int = qrand() % elite_genomes_count;
//            randomGenomes->append(random_int);
//        }
//        QList<int> *mom_genome = elite_genomes->at(randomGenomes->at(0));
//        QList<int> *dad_genome = elite_genomes->at(randomGenomes->at(1));
//        QList<int> *switches = new QList<int>();
//        int mom_genome_gene_count = mom_genome->size();
//        QList<int> *randomSwitches = new QList<int>();
//        for (int i = 0; i < 2; i++) {
//            int random_int = qrand() % (mom_genome_gene_count+1);
//            randomSwitches->append(random_int);
//        }
//        switches->append(randomSwitches->at(0));
//        switches->append(randomSwitches->at(1));

//        qSort(switches->begin(), switches->end());

//        /*Switching of the gene*/
//        int switch_idx = switches->at(0);
//        int switch_sdx = switches->at(1);

//        QList<int> *mom_frag = new QList<int>();
//        QList<int> *dad_frag = new QList<int>();
//        for (int f = switch_idx; f < switch_sdx; f++) {
//            mom_frag->append(mom_genome->at(f));
//            dad_frag->append(dad_genome->at(f));
//        }
//        QList<int> *daughter_genome = new QList<int>();
//        QList<int> *son_genome = new QList<int>();

//        /*Genome crossover*/
//        for (int i = 0; i < mom_genome->size(); i++)
//        {
//            int mom_val = mom_genome->at(i);
//            int dad_val = dad_genome->at(i);
//            if (i >= switch_idx && i < switch_sdx)
//            {
//                mom_val = dad_frag->at(i-switch_idx);
//                dad_val = mom_frag->at(i-switch_idx);
//            }
//            daughter_genome->append(mom_val);
//            son_genome->append(dad_val);
//        }

//        /*Mutation of the gene*/
//        /*Daughter*/
//        float tmp_mutation_prob = (float)qrand()/(float)RAND_MAX;
//        if (tmp_mutation_prob < MUTATION_RATE)
//        {
//            int random_idx = qrand() % (daughter_genome->size() - 1);
//            int random_val = qrand() % (SLOTS - 1);
//            daughter_genome->replace(random_idx, random_val);
//        }
//        /*Son*/
//        tmp_mutation_prob = (float)qrand()/(float)RAND_MAX;
//        if (tmp_mutation_prob < MUTATION_RATE)
//        {
//            int random_idx = qrand() % (son_genome->size() - 1);
//            int random_val = qrand() % (SLOTS - 1);
//            son_genome->replace(random_idx, random_val);
//        }
//        new_genome_pool->append(daughter_genome);
//        new_genome_pool->append(son_genome);
//    }
//    QList<QList<int>*> *final_new_genome_pool = new QList<QList<int>*>();
//    for (int r = 0; r < new_genome_pool->size(); r++) {
//        if (r >= POPULATION) {
//            break;
//        }
//        final_new_genome_pool->append(new_genome_pool->at(r));
//    }
//    return final_new_genome_pool;
//}

/*Try generating one layout*/
QHash<QString, QHash<int, int>* >* generateCurrentLayout(InteractionSessionContainer *is_container)
{
    /*Create slot base layout and set it with interaction session and its slot number */
    QList<QPair<InteractionSession*, int>*> *slot_assigned_interaction_sessions = is_container->getCurrentInteractionSessions();

    qDebug() << "In generate layout, check if slot is well assigned into interaction sessions.";
    for (int i = 0; i < slot_assigned_interaction_sessions->size(); i++)
    {
        qDebug() << slot_assigned_interaction_sessions->at(i)->first->getMembersOneStr() << slot_assigned_interaction_sessions->at(i)->second << "Time:" << slot_assigned_interaction_sessions->at(i)->first->getStartTime() << slot_assigned_interaction_sessions->at(i)->first->getEndTime();
//        for (int j = 0; j < slot_assigned_interaction_sessions->at(i)->first->proceeding_interaction_sessions->size(); j++)
//        {
//            qDebug() << "proceeding:" << slot_assigned_interaction_sessions->at(i)->first->proceeding_interaction_sessions->at(j)->getMembersOneStr();
//        } -> checked
    }
    SlotBaseLayout * slot_base_layout = NULL;

    if (!slot_assigned_interaction_sessions->isEmpty()) {
        slot_base_layout = new SlotBaseLayout(SLOTS);

        for (int i = 0; i < slot_assigned_interaction_sessions->size(); i++)
        {            
            QPair<InteractionSession*, int>* slot_assigned_interaction_session = slot_assigned_interaction_sessions->at(i);
            InteractionSession *interaction_session = slot_assigned_interaction_session->first;
            int slot_number = slot_assigned_interaction_session->second;            
            bool _valid_layout = slot_base_layout->setInteractionSessionToSlot(interaction_session, slot_number);
            if (!_valid_layout) {
                return NULL;
            }
        }
    }

    if (slot_base_layout == NULL) {
        /*No feasible layout with this genome sequence*/
        qDebug() << "slot_base_layout is null";
        return NULL;
    }

    rearrangeLineSegments(slot_base_layout);

    int time_steps = slot_assigned_interaction_sessions->last()->first->getEndTime();

    QList<SlotLayout*> *slot_layouts = new QList<SlotLayout*>();
    for (int i = 0; i < SLOTS; i++) {
        SlotLayout *slot_layout = new SlotLayout(time_steps);
        slot_layouts->append(slot_layout);
    }

    removeWhiteSpace(slot_base_layout, slot_layouts, time_steps);

    QHash<QString, QHash<int, int>*> *y_coords = new QHash<QString, QHash<int, int>*>();
    QHash<InteractionSession*, SessionLayout*>::iterator j = slot_base_layout->getSessionsLayout()->begin();
    for (; j != slot_base_layout->getSessionsLayout()->end(); j++)
    {
        InteractionSession* interaction_session = j.key();
        SessionLayout* session_layout = j.value();
        int end_time = interaction_session->getEndTime();
        int start_time = interaction_session->getStartTime();

        QHash<QString, int>* layout = session_layout->getSessionLayout();
        int slot_number = session_layout->getSlot();
        SlotLayout* slot_layout = slot_layouts->at(slot_number);
        QHash<QString, int>::iterator k = layout->begin();
        for (; k != layout->end(); k++)
        {
            QString member = k.key();
            int slot_number = k.value();
            QHash<int, int> *y_with_timestep = NULL;
            //if (y_with_timestep == NULL)
            if (!y_coords->contains(member))
            {
                y_with_timestep = new QHash<int, int>();
            }
            else
            {
                y_with_timestep = y_coords->value(member);
            }
            for (int time_step = start_time; time_step < end_time; time_step++)
            {
                QPair<int, int> *size = slot_layout->getSlotLayout()->value(time_step);
                if (size != NULL) {
                    int bottom_base = size->first;
                    //qDebug() << interaction_session->getMembersOneStr() << time_step; -> correctly working
                    y_with_timestep->insert(time_step, slot_number+bottom_base);
                    y_coords->insert(member, y_with_timestep);
                }
            }
        }
    }

    //relaxLines(y_coords, slot_assigned_interaction_sessions); //-> cause connection or collapse between lines

    if (y_coords == NULL) {
        qDebug() << "y_coords is NULL";
    }
    return y_coords;

}
QList<int> *randomGenerateSlots(int max_slot_number, int slotList_length)
{
    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());

    QList<int> *slotList = new QList<int>();
    if (slotList_length > 0)
    {
        while (slotList->size() < slotList_length)
        {
            int random_int = (qrand() % (max_slot_number-1)) + 1;
            if (!slotList->contains(random_int))
            {
                slotList->append(random_int);
                //qDebug() << "random:" << random_int;
            }
        }

        return slotList;
    }
    else {
        return NULL;
    }
}
//QList<int> *random_openedSlots(int slotList_length, QList<int> *openedSlotList)
//{
//    QTime time = QTime::currentTime();
//    qsrand((uint)time.msec());
//    QList<int> *random_openedSlotList = new QList<int>();

//    if (slotList_length > 0)
//    {
//        for (int i = 0; i < slotList_length; i++)
//        {
//            int random_index = qrand() % openedSlotList->size();
//            int random_opened_slot = openedSlotList->at(random_index);
//            random_openedSlotList->append(random_opened_slot);
//            for (int j = 0; j < i; j++)
//            {
//                if (random_openedSlotList->at(j) == random_openedSlotList->at(i))
//                {
//                    i--;
//                }
//            }
//        }
//        return random_openedSlotList;
////        QList<int>* countList = new QList<int>();
////        for (int i = 0; i < random_openedSlotList->size(); i++)
////        {
////            for (int j = 0; j < random_openedSlotList->size(); j++)
////            {
////                if (random_openedSlotList->at(i) == random_openedSlotList->at(j)) {
////                    count++;
////                }
////            }
////            countList->append(count);

////            count = 0;
////        }
////        qSort(countList->begin(), countList->end());
////        int max = countList->last();
////        if (max >= 2) {
////            qDebug() << "again random:!";
////            random_openedSlots(slotList_length, openedSlotList);
////        }
////        else if (max < 2){
////            return random_openedSlotList;
////        }
//    }
//    else {
//        return NULL;
//    }
//}
void permutation_oneCombination(QList<QPair<QList<QPair<InteractionSession*, int> *> *, float> *> *currentISsList,
                                QList<QPair<QHash<QString, QHash<int, int>*>*, float>*> *current_layout_fittness_list,
                                QList<int> *original_combination,
                                QList<int> *permutation_after_combination,
                                InteractionSessionContainer *is_container,
                                QList<InteractionSession*> *current_new_interaction_sessions)
{
    if (original_combination->isEmpty())
    {
        qDebug();
        qDebug() << "permutation_after_combination";
        if (!permutation_after_combination->isEmpty())
        {
            for (int i = 0; i < permutation_after_combination->size(); ++i) {
                qDebug() << permutation_after_combination->at(i);
            }

            tryLayout(currentISsList, current_layout_fittness_list, is_container, current_new_interaction_sessions, permutation_after_combination);
            return;
        }

    }
    for (int i = 0; i < original_combination->size(); i++)
    {
        QList<int> *original2_combination = new QList<int>();
        for (int j = 0; j < original_combination->size(); j++)
        {
            original2_combination->append(original_combination->at(j));
        }
        original2_combination->removeAt(i);
        QList<int> *permutation2 = new QList<int>();
        for (int j = 0; j < permutation_after_combination->size(); j++)
        {
            permutation2->append(permutation_after_combination->at(j));
        }
        permutation2->append(original_combination->at(i));
        permutation_oneCombination(currentISsList, current_layout_fittness_list, original2_combination, permutation2, is_container, current_new_interaction_sessions);
    }
}

void combinationLayout(
        QList<QPair<QList<QPair<InteractionSession*, int> *> *, float> *> *currentISsList,
        QList<QPair<QHash<QString, QHash<int, int>*>*, float> *> *current_layout_fittness_list,
        InteractionSessionContainer *is_container,
        QList<InteractionSession*> *current_new_interaction_sessions,
        QList<int> *combination,
        QList<int> *openedSlotList, int k, int offset)
{

    if (k == 0)
    {
        QList<int> *permutation_after_combination = new QList<int>();
        if (!combination->isEmpty())
        {
            permutation_oneCombination(currentISsList, current_layout_fittness_list, combination, permutation_after_combination, is_container, current_new_interaction_sessions);
            return;
        }
    }
    for (int i = offset; i <= openedSlotList->size() - k; ++i)
    {
        int slot_number = openedSlotList->at(i);
        combination->push_back(slot_number);
        /* recursively generate combinations of integers from i+1..n
        */
        combinationLayout(currentISsList, current_layout_fittness_list, is_container, current_new_interaction_sessions, combination, openedSlotList, k-1, i+1);
        combination->pop_back();
    }

}
void assignSlotsToExtendingISs(QList<QPair<InteractionSession*, int> *> *extendingISsWithPrevISs,
                             InteractionSessionContainer *is_container,
                             QList<QPair<InteractionSession*, QString> *> *classified_interaction_sessions,
                             QList<InteractionSession*> *current_new_interaction_sessions,
                               QList<int> *openedSlotList)
{
    QList<InteractionSession*> *current_extending_interaction_sessions = new QList<InteractionSession*>();
    QList<int> *closedSlotList = new QList<int>();

    /*First, assign all extending interaction sessions with the same slot of the previous interaction sessions'*/
    if (!is_container->getISList_prevLayout()->isEmpty())
    {

        for (int i = 0; i < is_container->getISList_prevLayout()->size(); i++)
        {
            for (int j = 0; j < is_container->getISList()->size(); j++) {
                /*If the current interaction sessions have the interaction sessions which have the same interaction sessions in the previous best layout,
                copy the interaction session and its slot number and append to the current interaction sessions*/
                if (is_container->getISList()->at(j)->getMembersOneStr().compare(is_container->getISList_prevLayout()->at(i)->first->getMembersOneStr()) == 0 && is_container->getISList()->at(j)->getStartTime() == is_container->getISList_prevLayout()->at(i)->first->getStartTime())
                {
                    QPair<InteractionSession*, int>* is_and_slot = new QPair<InteractionSession*, int>();
                    is_and_slot->first = is_container->getISList()->at(j);
                    is_and_slot->second = is_container->getISList_prevLayout()->at(i)->second;
                    extendingISsWithPrevISs->append(is_and_slot);
                    //qDebug() << "After assigning slots to the extending interaction sessions, the layout has this interaction session:" << is_and_slot->first->getMembersOneStr() << "Time:" << is_and_slot->first->getStartTime() << is_and_slot->first->getEndTime() << "Slot:" << is_and_slot->second;
                }
            }
        }
    }

    if (!classified_interaction_sessions->isEmpty())
    {
        /*Make two groups according to interaction session's flag, extend and new*/
        for (int i = 0; i < classified_interaction_sessions->size(); i++)
        {

            QPair<InteractionSession*, QString> *classified_interaction_session = classified_interaction_sessions->at(i);
            InteractionSession* interaction_session = classified_interaction_session->first;
            QString flag = classified_interaction_session->second;
            if (flag.compare("extend") == 0)
            {
                current_extending_interaction_sessions->append(interaction_session);
                //qDebug() << "added to extend list" << interaction_session->getMembersOneStr();
            }
            if (flag.compare("new") == 0)
            {
                current_new_interaction_sessions->append(interaction_session);
                //qDebug() << "added to new list" << interaction_session->getMembersOneStr();
            }
        }
        /*Currently, if there are extending interaction sessions,*/
        if (!current_extending_interaction_sessions->isEmpty())
        {
            /*Get the closed slots*/
            for (int i = 0; i < current_extending_interaction_sessions->size(); i++)
            {
                InteractionSession *current_extending_interaction_session = current_extending_interaction_sessions->at(i);

                for (int j = 0; j < extendingISsWithPrevISs->size(); j++)
                {
                    QPair<InteractionSession*, int> *prev_slot_assigned_interaction_session = extendingISsWithPrevISs->at(j);
                    InteractionSession* prev_interaction_session = prev_slot_assigned_interaction_session->first;
                    int slot_number = prev_slot_assigned_interaction_session->second;
                    /*If the current extending interaction session is same with the interaction session in the current interaction sessions,
                    the slot of the current interaction session is closed*/
                    if (current_extending_interaction_session->getMembersOneStr().compare(prev_interaction_session->getMembersOneStr()) == 0 && current_extending_interaction_session->getEndTime() == prev_interaction_session->getEndTime())
                    {
                        //qDebug() << "in assigning slots to extending ISs: current IS has to have" << prev_interaction_session->getMembersOneStr() << prev_interaction_session->getStartTime() << prev_interaction_session->getEndTime() << "Slot:" << slot_number;
                        closedSlotList->append(slot_number);
                    }
                }
            }
            /*Get the opened slots*/
            for (int i = 1; i < SLOTS; i++)
            {
                /*There are extending ISs*/
                if (!closedSlotList->isEmpty())
                {
                    if (!closedSlotList->contains(i))
                    {
                        //qDebug() << "openslot list has" << i;
                        openedSlotList->append(i);
                    }
                }
                /*There is no extending ISs*/
                else
                {
                    //qDebug() << "openslot list has" << i;
                    openedSlotList->append(i);
                }
            }
        }
        /*Else if there is no extending interaction sessions,*/
        else
        {
            /*Get the opened slots: every slots (0 to SLOTS-1)
            * Do I have to include 0? */
            for (int i = 1; i < SLOTS; i++)
            {
                openedSlotList->append(i);
            }
        }
        /*If there are new interaction sessions,*/
        if (current_new_interaction_sessions->isEmpty())
        {
            /*Set current interaction sessions
                (to be used for generating its layout)*/
            is_container->setCurrentInteractionSessions(extendingISsWithPrevISs);
        }
        /*Else if there is no new interaction sessions*/
        else {
            /*Set current interaction sessions (from the interaction sessions of the previous best layout to the current extending interaction sessions)
                (to be used for trying generating layouts with the new interaction sessions)*/
            is_container->setExtendingISsToPrevISs(extendingISsWithPrevISs);
        }
    }
}


void tryLayout(QList<QPair<QList<QPair<InteractionSession*, int> *> *, float> *> *current_layout_candidate_ISList,
               QList<QPair<QHash<QString, QHash<int, int>*>*, float> *> *current_layout_fittness_list,
                         InteractionSessionContainer *is_container,
               QList<InteractionSession*> *current_new_interaction_sessions,
                         QList<int>* permutation_after_combination)
{
    /*Assign slots to the new ISs*/
    assignSlotsToNewISs(is_container, current_new_interaction_sessions, permutation_after_combination);

    /*Generate current layout for the one case of permutation of combination*/
    QHash<QString, QHash<int, int>* > *current_layout = generateCurrentLayout(is_container);

    if (current_layout != NULL)
    {
        /*Evaluate*/
        float fittness = evaluateLayout(current_layout, is_container->getCurrentInteractionSessions()->last()->first->getEndTime());
        //qDebug() << "The fittness of this layout:" << fittness;
        QPair<QHash<QString, QHash<int, int>*>*, float> *current_layout_fittness = new QPair<QHash<QString, QHash<int, int>*>*, float>();
        current_layout_fittness->first = current_layout;
        current_layout_fittness->second = fittness;
        /*Make a list of layouts with fittness*/
        current_layout_fittness_list->append(current_layout_fittness);

        QPair<QList<QPair<InteractionSession*, int> *> *, float> *current_ISs_fittness = new QPair<QList<QPair<InteractionSession*, int> *> *, float>();
        current_ISs_fittness->first = is_container->getCurrentInteractionSessions();
        current_ISs_fittness->second = fittness;
        current_layout_candidate_ISList->append(current_ISs_fittness);
    }
}
void assignSlotsToNewISs(InteractionSessionContainer *is_container,
                      QList<InteractionSession*> *current_new_interaction_sessions,
                      QList<int>* permutation_after_combination)
{
    QList<QPair<InteractionSession*, int> *> *all_ISs_updated_with_new_ISs = new QList<QPair<InteractionSession*, int>*>();
    /*Get previous best slot assigned ISs*/
    if (!is_container->getExtendingISsWithPrevISs()->isEmpty())
    {
        for (int t = 0; t < is_container->getExtendingISsWithPrevISs()->size(); t++)
        {
            for (int j = 0; j < is_container->getISList()->size(); j++)
            {
                if (is_container->getExtendingISsWithPrevISs()->at(t)->first->getMembersOneStr().compare(is_container->getISList()->at(j)->getMembersOneStr()) == 0 && is_container->getExtendingISsWithPrevISs()->at(t)->first->getStartTime() == is_container->getISList()->at(j)->getStartTime())
                {
                    QPair<InteractionSession*, int> *tt = new QPair<InteractionSession*, int>();
                    tt->first = is_container->getISList()->at(j);
                    tt->second = is_container->getExtendingISsWithPrevISs()->at(t)->second;
                    all_ISs_updated_with_new_ISs->append(tt);
                    //qDebug() << "Before assigning slots to the new interaction sessions, the layout has this interaction session:" << tt->first->getMembersOneStr() << "Time:" << tt->first->getStartTime() << tt->first->getEndTime() << "Slot:" << tt->second;
                }
            }
        }
    }

    /*Second, assign all new interaction sessions with the one of the combinations*/
    for (int i = 0; i < current_new_interaction_sessions->size(); i++)
    {
        InteractionSession *current_new_interaction_session = current_new_interaction_sessions->at(i);
        for (int j = 0; j < is_container->getISList()->size(); j++)
        {
            if (current_new_interaction_session->getMembersOneStr().compare(is_container->getISList()->at(j)->getMembersOneStr()) == 0 && current_new_interaction_session->getStartTime() == is_container->getISList()->at(j)->getStartTime())
            {
                int slot_number = 0;

                slot_number = permutation_after_combination->at(i);
//                qDebug() << "Assigning this slot number to new IS: " << slot_number;
//                if (slot_number == 0 && current_new_interaction_session->getMembersOneStr().compare("D,G") == 0) {
//                    qDebug() << "break";
//                }
                QPair<InteractionSession*, int>* pair = new QPair<InteractionSession*, int>();
                pair->first = is_container->getISList()->at(j);
                pair->second = slot_number;
                all_ISs_updated_with_new_ISs->append(pair);
            }
        }
    }

    is_container->setCurrentInteractionSessions(all_ISs_updated_with_new_ISs);

}

QList<QPair<InteractionSession*, QString> *> *classifyInteractionSessions(InteractionSessionContainer *is_container)
{
    QList<InteractionSession*> *current_interaction_sessions = is_container->getTempISList_now();
    QList<InteractionSession*> *prev_interaction_sessions = is_container->getTempISList_prev();
    QList<QPair<InteractionSession*, QString> *> *classified_interaction_sessions = new QList<QPair<InteractionSession*, QString> *>();
    int count = 0;

    is_container->initializeClassifiedInteractionSessions();

    for (int i = 0; i < current_interaction_sessions->size(); i++)
    {
        InteractionSession *current_interaction_session = current_interaction_sessions->at(i);
        if (prev_interaction_sessions->isEmpty()) {
            is_container->addClassifiedInteractionSession(current_interaction_session, "new");
        }
        else {
            InteractionSession *prev_interaction_session = NULL;
            for (int j = 0; j < prev_interaction_sessions->size(); j++)
            {
                prev_interaction_session = prev_interaction_sessions->at(j);
                if (current_interaction_session->getMembersOneStr().compare(prev_interaction_session->getMembersOneStr()) != 0)
                {
                    count++;
                }

            }
            if (count == prev_interaction_sessions->size()) {
                is_container->addClassifiedInteractionSession(current_interaction_session, "new");

            }
            if (count < prev_interaction_sessions->size()) {
                is_container->addClassifiedInteractionSession(current_interaction_session, "extend");
            }

            count = 0;
        }
    }

    classified_interaction_sessions = is_container->getClassifiedInteractionSessions();
    return classified_interaction_sessions;

}

QHash<QString, QHash<int, int>* >* computeLayout(InteractionSessionContainer *is_container)
{
    /*For this timestep,*/
    /*Classify the interaction sessions into "new" and "extend"*/
    QList<QPair<InteractionSession*, QString> *> *classified_interaction_sessions = classifyInteractionSessions(is_container);
    /*Initialize current new interaction sessions*/
    QList<InteractionSession*> *current_new_interaction_sessions = new QList<InteractionSession*>();
    /*Initialize combination*/
    QList<int> *combination = new QList<int>();
    /*Initialize opened slots*/
    QList<int> *openedSlotList = new QList<int>();

    QList<QPair<InteractionSession*, int> *> *extendingISsWithPrevISs = new QList<QPair<InteractionSession*, int> *>();

    /*Return NULL layout when current IS list is empty*/
    if (is_container->getTempISList_now()->isEmpty())
    {
        qDebug() << "No current ISs";
        return NULL;
    }

    else {

        if (!classified_interaction_sessions->isEmpty())
        {

            /*Initialize currently possible layout-fittness list*/
            QList<QPair<QHash<QString, QHash<int, int>*>*, float>*> *current_layout_fittness_list = new QList<QPair<QHash<QString, QHash<int, int>*>*, float>*>();
            /*Initialize currently possible interaction session list (from start upto now), which will be the best current IS list */
            QList<QPair<QList<QPair<InteractionSession*, int> *> *, float> *> *currentISsList = new QList<QPair<QList<QPair<InteractionSession*, int> *> *, float> *>();
            /*If there are extending ISs, assign slots to the extending ISs*/
             /*Before assigning all combination of opened slots, get openedSlotList and current new interaction sessions*/
            assignSlotsToExtendingISs(extendingISsWithPrevISs, is_container, classified_interaction_sessions, current_new_interaction_sessions, openedSlotList);
            /*Get slot list length*/
            int slotList_length = current_new_interaction_sessions->size();

            //qDebug() << "current_new_interaction_sessions size:" << current_new_interaction_sessions->size() << "opened_slot_list size: " << openedSlotList->size();

            if (!current_new_interaction_sessions->isEmpty())
            {
                if (is_container->getTempISList_prev()->isEmpty())
                {
                    QList<int> *randomList = randomGenerateSlots(SLOTS, slotList_length);
                    assignSlotsToNewISs(is_container, current_new_interaction_sessions, randomList);
                    /*Generate current layout for the extended interaction sessions*/
                    QHash<QString, QHash<int, int>* > *current_layout = generateCurrentLayout(is_container);
                    int fittness = evaluateLayout(current_layout, is_container->getCurrentInteractionSessions()->last()->first->getEndTime());
                    //qDebug() << "The fittness of this layout is" << fittness;
                    /*Set current best interaction sessions into is_container*/
                    is_container->setISList_prevLayout(is_container->getCurrentInteractionSessions());

                    /*Return the current layout*/
                    return current_layout;

                }
                else {
                    //qDebug() << "Combination started!";
                    /*Repeat the following procedure until the combination cases are finished: get the one combination of open slot numbers, assign it to current interaction sessions, generate its layout, evaluate it, and store the layout's fittness into the list*/
                    combinationLayout(currentISsList, current_layout_fittness_list, is_container, current_new_interaction_sessions, combination, openedSlotList, slotList_length, 0);

                    /*After getting all evaluations of layouts,*/

                    QPair<QHash<QString, QHash<int, int>*>*, float>* best = NULL;
                    QPair<QList<QPair<InteractionSession*, int>*>*, float> *best_currentISs = NULL;
                    if (!current_layout_fittness_list->isEmpty())
                    {
                        /*Compare evaluated current layouts and get the current layout with the minimum fittness*/
                        qSort(current_layout_fittness_list->begin(), current_layout_fittness_list->end(), fittnessLessThen2);
                        /*Compare evaluated current interaction sessions and get the slot assigned interaction sessions of the current best layout.*/
                        qSort(currentISsList->begin(), currentISsList->end(), fittnessLessThen3);
                        best = current_layout_fittness_list->first();
                        best_currentISs = currentISsList->first();
                    }
                    if (best == NULL) {
                        return NULL;
                    }
                    else if (best != NULL){
                        QHash<QString, QHash<int, int>*> *best_layout = best->first;
                        qDebug() << "best fittness: " << best->second;
                        /*Set the current best whole slot assigned ISs to the previous best whole slot assigned ISs.*/
                        is_container->setISList_prevLayout(best_currentISs->first);
                        for (int k = 0; k < best_currentISs->first->size(); k++)
                        {
                            QPair<InteractionSession*, int> *kk = best_currentISs->first->at(k);
                            InteractionSession* is = kk->first;
                            int slot = kk->second;

                            qDebug() << "Set the current best ISs:" << is->getMembersOneStr() << "Time:" << is->getStartTime() << is->getEndTime() << "Slot:" << slot;
                        }


                        return best_layout;
                    }
                }
            }
            else
            {
                /*Generate current layout for the extended interaction sessions*/
                QHash<QString, QHash<int, int>* > *current_layout = generateCurrentLayout(is_container);
                float fittness = evaluateLayout(current_layout, is_container->getCurrentInteractionSessions()->last()->first->getEndTime());
                //qDebug() << "The fittness of this layout is" << fittness;
                /*Set current best interaction sessions into is_container*/
                is_container->setISList_prevLayout(is_container->getCurrentInteractionSessions());

                /*Return the current layout*/
                return current_layout;

            }
        }
        else {
            return NULL;
        }
    }
    return NULL;
}
void initConnections(QList<InteractionSession*> *interaction_sessions)
{
    for (int i = 0; i < interaction_sessions->size(); i++)
    {
        InteractionSession* interaction_session = interaction_sessions->at(i);
        interaction_session->proceeding_interaction_sessions->clear();
        interaction_session->following_interaction_sessions->clear();
    }
}

void registerConnections(QList<InteractionSession*> *interaction_sessions)
{

    for (int i = 0; i < interaction_sessions->size(); i++)
    {
        InteractionSession* interaction_session_i = interaction_sessions->at(i);
        for (int j = i+1; j < interaction_sessions->size(); j++)
        {
            //qDebug() << i << j;
            InteractionSession* interaction_session_j = interaction_sessions->at(j);
            if (interaction_session_i->getStartTime() == interaction_session_j->getEndTime()) {
//                qDebug() << interaction_session_i->getMembersOneStr() << " after " << interaction_session_j->getMembersOneStr();
                interaction_session_i->proceeding_interaction_sessions->append(interaction_session_j);
                interaction_session_j->following_interaction_sessions->append(interaction_session_i);
                assert(interaction_sessions->at(i)->proceeding_interaction_sessions->size() > 0);
            }
            if (interaction_session_i->getEndTime() == interaction_session_j->getStartTime()) {
//                qDebug() << interaction_session_j->getMembersOneStr() << " after " << interaction_session_i->getMembersOneStr();
                interaction_session_j->proceeding_interaction_sessions->append(interaction_session_i);
                interaction_session_i->following_interaction_sessions->append(interaction_session_j);
                assert(interaction_sessions->at(j)->proceeding_interaction_sessions->size() > 0);
            }
        }
    }
}

void outputLayout(QString outputFileName, QHash<QString, QHash<int, int>* > *layout, InteractionSessionContainer *is_container)
{
    QList<InteractionSession*> *interaction_sessions = is_container->getISList();
    int time_steps = interaction_sessions->last()->getEndTime();
    //QList<QPair<InteractionSession*, int>*> *slot_assigned_interaction_sessions = is_container->getInteractionSessionsWithSlot();
    //int time_steps = slot_assigned_interaction_sessions->last()->first->getEndTime()+1;

    QFile file(outputFileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;
    QTextStream stream(&file);
    QHash<QString, QHash<int, int>* >::iterator i = layout->begin();
    for (; i != layout->end(); i++)
    {
        QString member = i.key();
        QHash<int, int> *member_coords = i.value();
        stream << member;

        for (int time_step = 0; time_step < time_steps; time_step++)
        {
            stream << "\t";
            if (member_coords->contains(time_step)) {
                stream << member_coords->value(time_step);
            }
            else {
                stream << " ";
            }
        }
        stream << "\n";
    }

    file.close();
}
void loadInteractionSessions(QString fileName, InteractionSessionContainer *iscontainer) {
    int time_count = 0;
    int node_count = 0;
    int interaction_session_count = 0;
    int location_count = 0;
    QHash<QString, int> *node_index_dict = new QHash<QString, int>();
    QHash<QString, int> *location_index_dict = new QHash<QString, int>();

    QFile file;
    file.setFileName(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }
    QByteArray line;
    for (int i = 0; i < 5; i++) {
        line = file.readLine();
        QStringList temp;
        temp = ((QString)line).split("=");
        if (temp[0] == "TIME_COUNT") {
            time_count = ((QString)temp[1]).toInt();

        }
        else if (temp[0] == "NODE_COUNT") {
            node_count = ((QString)temp[1]).toInt();

        }
        else if (temp[0] == "INTERACTION_SESSION_COUNT") {
            interaction_session_count = ((QString)temp[1]).toInt();

        }
        else if (temp[0] == "LOCATION_COUNT") {
            location_count = ((QString)temp[1]).toInt();

        }
    }
    /*Get node names*/
    line = file.readLine();
    QString line2 = QString::fromAscii(line).trimmed();
    QString subString2 = line2.mid(1, line2.size()-2);
    subString2.trimmed();

    QStringList temp2 = subString2.split(",");
    assert(temp2.size() == node_count);
    for (int j = 0; j < temp2.size(); j++) {
        QString temp_name_val = temp2[j];
        QStringList temp_name_val_pair = temp_name_val.split(':');
        QString temp_name = temp_name_val_pair[0].trimmed();
        QString name = temp_name.mid(1, temp_name.size()-2);
        QString temp_val = temp_name_val_pair[1].trimmed();
        int val = ((QString)temp_val).toInt();
        node_index_dict->insert(name, val);
    }
    /*Get location names*/
    line = file.readLine();
    QString line3 = QString::fromAscii(line).trimmed();
    QString subString3 = line3.mid(1, line3.size()-2);
    QStringList temp3 = subString3.split(",");
    assert(temp3.size() == location_count);
    for (int k = 0; k < temp3.size(); k++) {
        QString temp_name_val = temp3[k];
        QStringList temp_name_val_pair = temp_name_val.split(':');
        QString temp_name = temp_name_val_pair[0].trimmed();
        QString name = temp_name.mid(1, temp_name.size()-2);
        QString temp_val = temp_name_val_pair[1].trimmed();
        int val = ((QString)temp_val).toInt();
        location_index_dict->insert(name, val);
    }
    /*Get interaction sessions*/
    for (int is_idx = 0; is_idx < interaction_session_count; is_idx++) {
        line = file.readLine();
        QString lineStr = ((QString)line);
        lineStr.trimmed();
        QStringList tmp = lineStr.split(":");
        QString name = tmp[1].trimmed();

        line = file.readLine();
        lineStr = (QString)line;
        lineStr.trimmed();
        tmp = lineStr.split(":");
        int id_number = tmp[1].trimmed().toInt();

        line = file.readLine();
        lineStr = (QString)line;
        lineStr.trimmed();
        tmp = lineStr.split(":");
        int start = tmp[1].trimmed().toInt();

        line = file.readLine();
        lineStr = (QString)line;
        lineStr.trimmed();
        tmp = lineStr.split(":");
        int end = tmp[1].trimmed().toInt();

        line = file.readLine();
        lineStr = (QString)line;
        lineStr.trimmed();
        tmp = lineStr.split(":");
        QString members_str = tmp[1].trimmed();
        QString subString_members_str = members_str.mid(1, members_str.size()-2);

        QStringList tmp_members_str = subString_members_str.split(",");
        QString membersOnestr;
        QStringList membersList;
        for (int n = 0; n < tmp_members_str.size(); n++) {
            int val = tmp_members_str.at(n).toInt();
            QString name = node_index_dict->key(val);
            membersList.append(name);
        }
        membersOnestr = membersList.join(",");

        int location = -1;

        InteractionSession* is = new InteractionSession(id_number, membersList, start, end, location);
        is->setMembersOneStr(membersOnestr);
        iscontainer->add(is);
        line = file.readLine(); //for location part
        line = file.readLine(); //for one blank line
    }
    file.close();
}

/*Read and load data into Interaction Session data structure*/
/*We will call "compute storylines" inside of loadData function...*/
void loadData(QString fileName, InteractionSessionContainer *iscontainer)
{
    QFile file;
    file.setFileName(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }
    QByteArray line;
    int id = 0;
    int temp_id = 0;
    int location = -1;
    int prev_time = 0;
    iscontainer->createCompareList_prev();
    while(!file.atEnd()) {
        line = file.readLine();
        QStringList temp;

        temp = ((QString)line).split("\t");

        int time = ((QString)temp[0]).toInt();

        iscontainer->createCompareList_now();

        if (temp.size() > 1)
        {

            qDebug() << "When " << time << ": ";
            for (int i = 1; i < temp.size(); i++)
            {
                /*Get the list of the members per time*/
                QStringList members;
                QString membersSingleStr;
                members = ((QString)temp[i]).trimmed().split(",");
                members.sort();
                membersSingleStr = members.join(",");
                location = i;

                //InteractionSession* existing_is = iscontainer->find(membersSingleStr);
                /*Get interaction sessions of the previous time*/
                QList<InteractionSession*> *_tisList = iscontainer->getTempISList_prev();
                InteractionSession* existing_is = NULL;

                if (!_tisList->isEmpty()) {
                    for (int j = 0; j < _tisList->size(); j++)
                    {
                        /*Check if the current members exists in the previous interaction sessions*/
                        if (_tisList->at(j)->getMembersOneStr().compare(membersSingleStr) == 0)
                        {
                            existing_is = _tisList->at(j);
                        }
                    }
                }
                /*If no exist in the previous interaction sessions, create new interaction session for these current members*/
                if (existing_is == NULL) {
                    InteractionSession* is = new InteractionSession(id, members, time, time+1, location);
                    is->setMembersOneStr(membersSingleStr);
                    iscontainer->add(is);

                    id++;
                }
                /*Exist in the previous interaction sessions, update the ending time of the interaction session of the current members*/
                else {
                    iscontainer->update(existing_is, prev_time, time+1);

                }

            }

        }


        /*Set the interaction sessions of the current timeslot*/
        for (int k = 1; k < temp.size(); k++)
        {
            QStringList members;
            QString membersSingleStr;
            members = ((QString)temp[k]).trimmed().split(",");
            members.sort();
            membersSingleStr = members.join(",");
            location = k;
            InteractionSession* temp_is = new InteractionSession(temp_id, members, time, time+1, location);
            temp_is->setMembersOneStr(membersSingleStr);
            iscontainer->addTemp(temp_is);
            temp_id++;
        }
        /*Print prevIS list and current IS list for each time*/
//        QList<InteractionSession*> *tempISList_now = iscontainer->getTempISList_now();
//        QList<InteractionSession*> *tempISList_prev = iscontainer->getTempISList_prev();
//        qDebug() << "Previous interaction sessions:";
//        for (int j = 0; j < tempISList_prev->size(); j++) {
//            InteractionSession *is = tempISList_prev->at(j);
//            is->printIS();
//        }
//        qDebug() << "Current interaction sessions:";
//        for (int j = 0; j < tempISList_now->size(); j++) {
//            InteractionSession *is = tempISList_now->at(j);
//            is->printIS();
//        }

        /*Set interaction sessions of the previous timeslot*/
        iscontainer->setPrevISList(iscontainer->getTempISList_now());


        /*Update the previous time*/
        prev_time = time+1;


        /*Print all IS list for each time*/
        qDebug() << "Whole interaction sessions:";
        QList<InteractionSession*> *tempISList = iscontainer->getISList();
        for (int i = 0; i < tempISList->size(); i++) {
            InteractionSession *is = tempISList->at(i);
            is->printIS();
        }
        qDebug() << "\n";

    }
    file.close();
}
