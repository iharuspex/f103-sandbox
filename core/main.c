#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/cm3/systick.h>

#include "QuarkTS.h"

#include "app_tasks.h"

#define TIMER_TICK 0.001 /* 1 ms */

static volatile uint64_t _millis = 0;

static void systick_setup() {
    // set the systick clock source to our main clock
    systick_set_clocksource(STK_CSR_CLKSOURCE_AHB);
    // clear the Current Value Register so that we start at 0
    STK_CVR = 0;
    // in order to trigger an interrupt every millisecond, we can set the reload
    // value to be the speed of the processor / 1000 - 1
    systick_set_reload(rcc_ahb_frequency / 1000 - 1);
    // enable interrupts from the system tick clock
    systick_interrupt_enable();
    // enable the system tick counter
    systick_counter_enable();
}

// get the current value of the millis counter
uint64_t millis() {
    return _millis;
}

void sys_tick_handler(void) {
    // Increment our monotonic clock
    _millis++;
    qClock_SysTick();
}

// Delay a given number of milliseconds in a blocking manner
void delay(uint64_t duration) {
    const uint64_t until = millis() + duration;
    while (millis() < until);
}

static void gpio_setup(void)
{
    rcc_clock_setup_in_hse_8mhz_out_72mhz();

	rcc_periph_clock_enable(RCC_GPIOC);
	gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_2_MHZ,
		      GPIO_CNF_OUTPUT_PUSHPULL, GPIO13);
}

int main(void)
{
	gpio_setup();
    systick_setup();

    qOS_Setup(NULL, TIMER_TICK, IdleTask_Callback);
    qOS_Add_Task(&HardwareCheckTask, HardwareCheckTask_Callback,
                 qLowest_Priority, 0.25f, qPeriodic, qEnabled, NULL);
    qOS_Add_Task(&SignalAnalisysTask, SignalAnalisysTask_Callback,
                 qHigh_Priority, 0.1f, 200, qEnabled, NULL);
    qOS_Add_EventTask(&CheckUserEventsTask, CheckUserEventsTask_Callback,
                      qMedium_Priority, NULL);
    qOS_Add_Task(&CommunicationTask, CommunicationTask_Callback,
                 qHigh_Priority, qTimeImmediate, qPeriodic, qEnabled, NULL);
    qOS_Run();

	while (1) {}

	return 0;
}
