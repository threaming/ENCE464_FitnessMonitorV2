#include "hal/timer_hal.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "driverlib/interrupt.h"
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "prompt_to_move.h"
#include "mocks/rtos_mocks/rtos_wrapper.h"
#include "mocks/prompt_to_move_mock.h"

#include <stdio.h>

// Task handle for the task that will be notified by the timer interrupt
static TaskHandle_t xTaskHandle = NULL;

// Interrupt handler for Timer0A
static void timer_interrupt(void)
{
    // Clear the timer interrupt
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

    // Notify the task
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    RTOS_TaskNotifyGiveFromISR(xTaskHandle, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

// Timer initialization function
void timer_hal_init(uint8_t timer_period_seconds)
{
    // Enable the Timer0 peripheral
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_TIMER0)) {}

    // Configure Timer0 as a 32-bit periodic timer
    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);

    // Set the timer load value based on the period in seconds
    uint32_t ui32Period = (SysCtlClockGet() * timer_period_seconds);
    TimerLoadSet(TIMER0_BASE, TIMER_A, ui32Period - 1);

    // Register the interrupt handler
    TimerIntRegister(TIMER0_BASE, TIMER_A, timer_interrupt);

    // Set the interrupt priority
    IntPrioritySet(INT_TIMER0A, pdTM4C_RTOS_INTERRUPT_PRIORITY(1));

    // Enable the Timer0A interrupt
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

    // Enable the timer
    TimerEnable(TIMER0_BASE, TIMER_A);
}

// Task function that waits for the timer interrupt notification
static void vTaskFunction(void *pvParameters)
{
    while (1)
    {
        // Wait for the notification from the timer interrupt
        RTOS_TaskNotifyTake(pdTRUE, portMAX_DELAY);

        // Call the handler function
        act_on_prompt_to_move();
    }
}

// Function to create the task
void timer_hal_start_task(void)
{
    // Create the task
    RTOS_TaskCreate(vTaskFunction, "Task", 256, NULL, 1, &xTaskHandle);
}

void timer_hal_reset(uint8_t timer_period_seconds)
{
    // Disable the timer
    TimerDisable(TIMER0_BASE, TIMER_A);

    // Clear any pending interrupts
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

    // Reload the timer with the initial value
    TimerLoadSet(TIMER0_BASE, TIMER_A, SysCtlClockGet() * timer_period_seconds);

    // Re-enable the timer
    TimerEnable(TIMER0_BASE, TIMER_A);
}