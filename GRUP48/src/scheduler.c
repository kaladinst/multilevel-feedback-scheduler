#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "scheduler.h"

/*Global değişkenler*/
#define MAX_TASKS 25
QueueHandle_t OncelikZero;
QueueHandle_t OncelikOne;
QueueHandle_t OncelikTwo;
QueueHandle_t OncelikThree;
Task_t G_TaskList[MAX_TASKS];
int G_TaskCount = 0;
int G_CurrentTime = 0;

/* Random renk oluşturma fonksiyonu*/
const char* getRandomColor() {
    int r = rand() % 6;
    switch (r) {
        case 0: return COLOR_RED;
        case 1: return COLOR_GREEN;
        case 2: return COLOR_YELLOW;
        case 3: return COLOR_BLUE;
        case 4: return COLOR_MAGENTA;
        case 5: return COLOR_CYAN;
        default: return COLOR_WHITE;
    }
}

void print_task_log(Task_t *task, const char *status) {
    printf("%s%.4f sn \t%s %s \t(id:%s \töncelik:%d \tkalan süre:%d sn)%s\n",
           task->color,                // 1. Renk
           (double)G_CurrentTime,      // 2. şuanki zaman
           task->id,                   // 3. task adı
           status,                     // 4. Durum
           task->id,                   // 5. Id
           task->priority,             // 6. Öncelik
           task->remaining,            // 7. Kalan zaman
           COLOR_RESET                 // 8. Reset Color
    );
}

static void prvSchedulerTask(void *pvParameters) {
    (void) pvParameters;
    
    for (;;) {
        for (int i = 0; i < G_TaskCount; i++) {
            if (G_TaskList[i].arrival == G_CurrentTime) {
                print_task_log(&G_TaskList[i], "başladı");
                
            }
        }
        vTaskDelay(pdMS_TO_TICKS(1000));
        G_CurrentTime++;
    }
}

void vSchedulerInit(void) {
    FILE *file;
    int arrival, priority, burst;
    int i = 0;
    file = fopen("giris.txt", "r");

    while(fscanf(file, "%d, %d, %d", &arrival, &priority, &burst) == 3) {
        sprintf(G_TaskList[i].id, "%04d", i);

        G_TaskList[i].arrival   = arrival;
        G_TaskList[i].priority  = priority;
        G_TaskList[i].burstTime = burst;
        G_TaskList[i].remaining = burst;
        G_TaskList[i].color     = getRandomColor();
        G_TaskList[i].handle    = NULL; 
        i++;
        G_TaskCount++;
    }

    fclose(file);
}


void vSchedulerStart(void) {
    OncelikZero = xQueueCreate(10, sizeof(Task_t*));
    OncelikOne = xQueueCreate(10, sizeof(Task_t*));
    OncelikTwo = xQueueCreate(10, sizeof(Task_t*));
    OncelikThree = xQueueCreate(10, sizeof(Task_t*));

    xTaskCreate(prvSchedulerTask, "Manager", configMINIMAL_STACK_SIZE, NULL, configMAX_PRIORITIES-1, NULL);

    vTaskStartScheduler();
}