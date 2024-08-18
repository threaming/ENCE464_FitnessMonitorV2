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

QueueHandle_t tempQueue;
void tempTask(void *pvParameters);
TaskHandle_t tempTaskHndl;

void tempInit(void){
    temp_hal_init();
    tempQueue = xQueueCreate(1, sizeof(float));
    xTaskCreate(tempTask, "READ_TEMP", configMINIMAL_STACK_SIZE, NULL, 4, &tempTaskHndl);
}

void tempTask(void *pvParameters){
    float temp;
    while(1){
        temp_hal_read(&temp);
        xQueueSend(tempQueue, &temp,10000);
    }
}

float tempGetTemp(void){
    float temp;
    xQueueReceive(tempQueue, &temp, 0);
    return temp;
}