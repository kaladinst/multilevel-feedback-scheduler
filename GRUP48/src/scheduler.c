#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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
Task_t* CurrentTask = NULL;
int G_TaskCount = 0;
int G_CurrentTime = 0;

/* Random renk oluşturma fonksiyonu*/
const char* getRandomColor() {
    static const char* palette[] = {
        "\x1b[38;5;196m",
        "\x1b[38;5;208m",
        "\x1b[38;5;226m",
        "\x1b[38;5;46m",
        "\x1b[38;5;51m",
        "\x1b[38;5;21m",
        "\x1b[38;5;201m",
        "\x1b[38;5;129m",
        "\x1b[38;5;200m",
        "\x1b[38;5;154m",
        "\x1b[38;5;214m",
        "\x1b[38;5;39m",
        "\x1b[38;5;93m",
        "\x1b[38;5;160m",
        "\x1b[38;5;220m",
        "\x1b[38;5;14m",
        "\x1b[38;5;190m",
        "\x1b[38;5;27m",
        "\x1b[38;5;127m",
        "\x1b[38;5;166m",
        "\x1b[38;5;82m",
        "\x1b[38;5;205m",
        "\x1b[38;5;229m",
        "\x1b[38;5;244m",
        "\x1b[38;5;105m"
    };
    int r = rand() % 25;
    return palette[r];
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
                Task_t* tempTaskPointer = &G_TaskList[i];
                if(G_TaskList[i].priority == 0) {
                    xQueueSend(OncelikZero, &tempTaskPointer, 0);
                } else if(G_TaskList[i].priority == 1) {
                    xQueueSend(OncelikOne, &tempTaskPointer, 0);
                }else if(G_TaskList[i].priority == 2) {
                    xQueueSend(OncelikTwo, &tempTaskPointer, 0);
                }else if(G_TaskList[i].priority == 3) {
                    xQueueSend(OncelikThree, &tempTaskPointer, 0);
                }
                
            }
        }
        Task_t* nextTask = NULL;
        Task_t* temp = NULL;
        if(CurrentTask != NULL &&  CurrentTask->priority == 0 && CurrentTask->remaining > 0) {
            nextTask = CurrentTask;
        } else {
            if(xQueueReceive(OncelikZero, &temp, 0) == pdTRUE) {
                nextTask = temp;
            } else if(xQueueReceive(OncelikOne , &temp,0) == pdTRUE) {
                nextTask = temp;
            } else if(xQueueReceive(OncelikTwo , &temp,0) == pdTRUE) {
                nextTask = temp;
            } else if(xQueueReceive(OncelikThree , &temp,0) == pdTRUE) {
                nextTask = temp;
            } 
        }


        if(nextTask != NULL) {
            CurrentTask = nextTask;

            print_task_log(CurrentTask, "yürütülüyor");
            CurrentTask->remaining--;
            if(CurrentTask->remaining == 0) {
                print_task_log(CurrentTask, "sonlandi");
                CurrentTask = NULL;
            } else {
                if(CurrentTask->priority == 0) {   
                } else {
                    if(CurrentTask->priority == 1) {
                        CurrentTask->priority = 2;
                        xQueueSend(OncelikTwo,&CurrentTask,0);
                    } else if(CurrentTask->priority == 2) {
                        CurrentTask->priority = 3;
                        xQueueSend(OncelikThree,&CurrentTask,0);
                    } else {
                        xQueueSend(OncelikThree,&CurrentTask,0);
                    }

                    CurrentTask = NULL;
                }
            }
        }
         else {
            CurrentTask = NULL;
         }

        G_CurrentTime++;
    }
}

void vSchedulerInit(void) {
    FILE *file;
    int arrival, priority, burst;
    int i = 0;
    file = fopen("giris.txt", "r");
    srand(time(NULL));
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

    xTaskCreate(prvSchedulerTask, "TaskManager", configMINIMAL_STACK_SIZE, NULL, configMAX_PRIORITIES-1, NULL);
    vTaskStartScheduler();
}