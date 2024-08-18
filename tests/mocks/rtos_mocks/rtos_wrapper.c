#include "rtos_wrapper.h"
#include "FreeRTOS.h"
#include "task.h"

BaseType_t RTOS_TaskCreate(TaskFunction_t pxTaskCode, const char * const pcName, const configSTACK_DEPTH_TYPE usStackDepth, void * const pvParameters, UBaseType_t uxPriority, TaskHandle_t * const pxCreatedTask) {
    return xTaskCreate(pxTaskCode, pcName, usStackDepth, pvParameters, uxPriority, pxCreatedTask);
}

uint32_t RTOS_TaskNotifyTake(BaseType_t xClearCountOnExit, TickType_t xTicksToWait) {
    return ulTaskNotifyTake(xClearCountOnExit, xTicksToWait);
}

void RTOS_TaskNotifyGiveFromISR(TaskHandle_t xTaskToNotify, BaseType_t *pxHigherPriorityTaskWoken) {
    vTaskNotifyGiveFromISR(xTaskToNotify, pxHigherPriorityTaskWoken);
}
