#ifndef F013_SANDBOX_APP_TASKS_H
#define F013_SANDBOX_APP_TASKS_H

#include "QuarkTS.h"

extern qTask_t CommunicationTask, HardwareCheckTask, CheckUserEventsTask, SignalAnalisysTask;

void CommunicationTask_Callback(qEvent_t);
void HardwareCheckTask_Callback(qEvent_t);
void CheckUserEventsTask_Callback(qEvent_t);
void SignalAnalisysTask_Callback(qEvent_t);

void IdleTask_Callback(qEvent_t);

#endif //F013_SANDBOX_APP_TASKS_H
