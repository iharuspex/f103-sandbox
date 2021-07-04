#include <libopencm3/stm32/gpio.h>

#include "app_tasks.h"

qTask_t CommunicationTask, HardwareCheckTask, CheckUserEventsTask, SignalAnalisysTask;

void CommunicationTask_Callback(qEvent_t e) {

}

void HardwareCheckTask_Callback(qEvent_t e) {
    gpio_toggle(GPIOC, GPIO13);	/* LED on/off */
}

void CheckUserEventsTask_Callback(qEvent_t e) {

}

void SignalAnalisysTask_Callback(qEvent_t e) {

}

void IdleTask_Callback(qEvent_t e) {

}
