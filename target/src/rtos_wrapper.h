#ifndef RTOS_WRAPPER_H
#define RTOS_WRAPPER_H

#include "FreeRTOS.h"
#include "task.h"

// Thin wrappers around FreeRTOS functions
BaseType_t RTOS_TaskCreate(TaskFunction_t pxTaskCode, const char * const pcName, const configSTACK_DEPTH_TYPE usStackDepth, void * const pvParameters, UBaseType_t uxPriority, TaskHandle_t * const pxCreatedTask);
uint32_t RTOS_TaskNotifyTake(BaseType_t xClearCountOnExit, TickType_t xTicksToWait);
void RTOS_TaskNotifyGiveFromISR(TaskHandle_t xTaskToNotify, BaseType_t *pxHigherPriorityTaskWoken);

#endif // RTOS_WRAPPER_H
