#include "sessionlayout.h"

SessionLayout::SessionLayout(int slot_number, InteractionSession *interaction_session)
{
    slot = slot_number;
    layout = new QHash<QString, int>();
    for(int i = 0; i < interaction_session->getMembers().size(); i++) {
        QString member = interaction_session->getMembers().at(i);
        layout->insert(member, -1);
    }
    static_lines = new QSet<QString>();
    rising_lines = new QSet<QString>();
    dropping_lines = new QSet<QString>();
    emerging_lines_rising = new QSet<QString>();
    emerging_lines_dropping = new QSet<QString>();
    emerging_lines_dying = new QSet<QString>();
}

QHash<QString, int>* SessionLayout::getSessionLayout()
{
    return layout;
}
int SessionLayout::getSlot()
{
    return slot;
}

QHash<QString, QString>* SessionLayout::getClassifiedMembers()
{
    classified_members = new QHash<QString, QString>();
    if (emerging_lines_rising->isEmpty()) {
        if (!static_lines->isEmpty()) {
            QSet<QString>::iterator s = static_lines->begin();
            for (; s != static_lines->end(); s++)
            {
                QString member = (*s);
                classified_members->insert(member, "static");
            }
        }
        if (!rising_lines->isEmpty()) {
            QSet<QString>::iterator r = rising_lines->begin();
            for (; r != rising_lines->end(); r++)
            {
                QString member = (*r);
                classified_members->insert(member, "rising");
            }
        }
        if (!dropping_lines->isEmpty()) {
            QSet<QString>::iterator d = dropping_lines->begin();
            for (; d != dropping_lines->end(); d++)
            {
                QString member = (*d);
                classified_members->insert(member, "dropping");
            }
        }
    }

    return classified_members;

}
