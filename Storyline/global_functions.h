#ifndef GLOBAL_FUNCTIONS_H
#define GLOBAL_FUNCTIONS_H

#include "slotbaselayout.h"
#include "slotlayout.h"
#include "slotsegment.h"
#include "slotsegments.h"
#include "interactionsession.h"
#include "interactionsessioncontainer.h"
#include <QPair>
#include <QList>
#include <QFile>
#include <QVariant>
#include <QHash>
#include <QTime>
#include <QVector>
#include <QSet>
#include <assert.h>
#include <cmath>
#include <qdebug.h>

//#include <boost/algorithm/string.hpp>
#include <limits.h>
#define SLOTS 13
#define OFFSET 4
#define OBJECTIVE_SLOPE 1.5
#define POPULATION 1000
#define FINAL_GENERATION 10
#define MUTATION_RATE 0.6
#define deviation 1
#define crossover 1
#define whitespace 1
#define ELITE_POPULATION int(POPULATION * 0.5 + 0.5)
#define JOIN_THRESHOLD 0.51

using namespace std;

//QList<QList<int>* > *populateGenomes(int population, int slots_count, int genome_length);
SlotBaseLayout* decodeGenome(QList<int>* genome, QList<InteractionSession*> *interaction_sessions);
void classifyProceedingLineSegments(SlotBaseLayout *slot_base_layout);
void classifyEmergingLineSegments(SlotBaseLayout *slot_base_layout);
bool fittnessLessThen(QPair<int, int>, QPair<int, int> );
bool startTimeLessThen_reversed(InteractionSession *is1, InteractionSession *is2);
bool startTimeLessThen(InteractionSession *is1, InteractionSession *is2);
bool checkAllLinsegmentsAreClassified(SlotBaseLayout *slot_base_layout);
void classifyLineSegments(SlotBaseLayout *slot_base_layout);
QList<InteractionSession*>* getCommonInteractionSession(QSet<InteractionSession*> sessions_1, QSet<InteractionSession*> sessions_2);
QList<InteractionSession*>* getForerunningInteractionSessions(InteractionSession* subject_interaction_session, QList<InteractionSession*> *interaction_sessions, bool is_sorted = false);
void assignMemberAlignment(SlotBaseLayout *slot_base_layout);
void rearrangeLineSegments(SlotBaseLayout *slot_base_layout);
SlotSegments* generateSlotSegments(SlotBaseLayout *slot_base_layout);
void adjustSlotSegments(SlotSegments *slot_segments, QList<SlotLayout*>* slot_layouts, int timesteps);
void removeWhiteSpace(SlotBaseLayout* slot_base_layout, QList<SlotLayout*>* slot_layouts, int time_steps);
void relaxLines(QHash<QString, QHash<int, int> *> *y_coords, QList<InteractionSession*>* interaction_sessions);
int evaluateLayout(QHash<QString, QHash<int, int> *> *layout, int time_steps);
//QHash<QString, QHash<int, int>* >* generateLayout(QList<int>* genome, InteractionSessionContainer *is_container);
//QPair<QHash<QString, QHash<int, int>* >*, int> *evaluateGenomes(QList<QList<int> *> *genome_pool, InteractionSessionContainer *is_container, QHash<int, int> *fittness_cache, int best_fittness, QHash<QString, QHash<int, int>* >*best_layout);
//QList<QList<int>*> *getEliteGenomes(QList<QList<int> *> *genome_pool, QHash<int, int> *fittness_cache);
//QList<QList<int>*> *renewGenomes(QList<QList<int> *> *genome_pool, QHash<int, int> *fittness_cache);
QHash<QString, QHash<int, int>* >* computeLayout(InteractionSessionContainer *is_container);
void initializeConnections(QList<InteractionSession*> *interaction_sessions);
void registerConnections(QList<InteractionSession*> *interaction_sessions);
void outputLayout(QString, QHash<QString, QHash<int, int>* > *layout, InteractionSessionContainer *is_container);
void loadInteractionSessions(QString, InteractionSessionContainer* iscontainer);
void loadData(QString fileName, InteractionSessionContainer *iscontainer);
bool fittnessLessThen2(QPair<QHash<QString, QHash<int, int>*>*, int>* i1, QPair<QHash<QString, QHash<int, int>*>*, int>* i2);
void permutation_oneCombination(QList<QPair<QHash<QString, QHash<int, int>*>*, int>*> *current_layout_fittness_list, QList<int> *original_combination, QList<int> *permutation_after_combination, InteractionSessionContainer *is_container, QList<InteractionSession*> *current_new_interaction_sessions);
void loop_assign_and_generate_and_evaluate(QList<QPair<QHash<QString, QHash<int, int>*>*, int> *> *current_layout_fittness_list, InteractionSessionContainer *is_container, QList<InteractionSession*> *current_new_interaction_sessions, QList<int> *permutation_after_combination);
QList<int> *randomGenerateSlots(int max_slot_number, int slotList_length);
QList<int> *random_openedSlots(int slotList_length, QList<int> *openedSlotList);
QList<QPair<InteractionSession*, QString> *> *classifyInteractionSessions(InteractionSessionContainer *is_container);
void combination_openedSlots(QList<QPair<QHash<QString, QHash<int, int>*>*, int> *> *current_layout_fittness_list, InteractionSessionContainer *is_container, QList<InteractionSession*> *current_new_interaction_sessions, QList<QList<int>*> *combinationList, QList<int> *combination_openedSlotList, QList<int> *openedSlotList, int k, int offset);
void assignSlotsToExtendingISs(InteractionSessionContainer *is_container, QList<QPair<InteractionSession*, QString> *> *classified_interaction_sessions, QList<InteractionSession*> *current_new_interaction_sessions, QList<int> *openedSlotList);
void assignSlotsToNewISs(InteractionSessionContainer *is_container, QList<InteractionSession*> *current_new_interaction_sessions, QList<int>* combination_openedSlotList);
QHash<QString, QHash<int, int>* >* generateCurrentLayout(InteractionSessionContainer *is_container);

#endif // GLOBAL_FUNCTIONS_H
