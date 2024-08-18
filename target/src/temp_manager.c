/*
 * Temperature Manager
 * Andy Ming
 * 18.08.2024
 *
 */

#include "temp_manager.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "hal/temp_hal.h"

static QueueHandle_t tempQueue;
void tempTask(void *pvParameters);
static TaskHandle_t tempTaskHndl;

void tempInit(void){
    bool success = temp_hal_init();
    configASSERT(success);
    tempQueue = xQueueCreate(1, sizeof(float));
    configASSERT(tempQueue != NULL);
    BaseType_t check = xTaskCreate(tempTask, "READ_TEMP", configMINIMAL_STACK_SIZE, NULL, 4, &tempTaskHndl);
    configASSERT(check == pdPASS);
}

void tempTask(void *pvParameters){
    float temp;
    #ifndef UNIT_TESTING
    while(1){
    #endif
        temp_hal_read(&temp);
        xQueueSend(tempQueue, &temp,portMAX_DELAY);
    #ifndef UNIT_TESTING
    }
    #endif
}

float tempGetTemp(void){
    float temp;
    xQueueReceive(tempQueue, &temp, portMAX_DELAY);
    return temp;
}