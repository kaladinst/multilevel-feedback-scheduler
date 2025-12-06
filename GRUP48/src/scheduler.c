#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "scheduler.h"

/* Global Değişkenler */
#define MAX_TASKS 25
QueueHandle_t OncelikZero;
QueueHandle_t OncelikOne;
QueueHandle_t OncelikTwo;
QueueHandle_t OncelikThree;
Task G_TaskList[MAX_TASKS];
Task* CurrentTask = NULL;
int G_TaskCount = 0;
int G_CurrentTime = 0;
int G_ActiveTasks = 0;

/* Renk Seçici */
const char* getRandomColor() {
    static const char* palette[] = {
        "\x1b[38;5;196m", "\x1b[38;5;208m", "\x1b[38;5;226m",
        "\x1b[38;5;46m", "\x1b[38;5;51m", "\x1b[38;5;21m",
        "\x1b[38;5;201m", "\x1b[38;5;129m", "\x1b[38;5;200m",
        "\x1b[38;5;154m", "\x1b[38;5;214m", "\x1b[38;5;39m",
        "\x1b[38;5;93m", "\x1b[38;5;160m", "\x1b[38;5;220m",
        "\x1b[38;5;14m", "\x1b[38;5;190m", "\x1b[38;5;27m",
        "\x1b[38;5;127m", "\x1b[38;5;166m", "\x1b[38;5;82m",
        "\x1b[38;5;205m", "\x1b[38;5;229m", "\x1b[38;5;244m", "\x1b[38;5;105m"
    };
    int r = rand() % 25;
    return palette[r];
}

/*Kuyrukların oluşturulması*/
void SchedulerStarter(void) {
    OncelikZero = xQueueCreate(10, sizeof(Task*));
    OncelikOne = xQueueCreate(10, sizeof(Task*));
    OncelikTwo = xQueueCreate(10, sizeof(Task*));
    OncelikThree = xQueueCreate(10, sizeof(Task*));

    xTaskCreate(schedulerTasking, "TaskManager", 2048, NULL, 4, NULL);
    vTaskStartScheduler();
}

/*Dosya okuması ve değişkenlerin atanması*/
void SchedulerInitializer(const char *filename) {
    FILE *file;
    int arrival, priority, burst;
    int i = 0;
    
    file = fopen(filename, "r");
    if (file == NULL) {
        printf("Hata: giris.txt bulunamadi\n");
        exit(1);
    }
    srand(time(NULL));
    while(fscanf(file, "%d, %d, %d", &arrival, &priority, &burst) == 3 && i < MAX_TASKS) {
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

    G_ActiveTasks = G_TaskCount;
    fclose(file);
}

/* Log Yazdırma */
void print_task_log(Task *task, const char *status) {
    if (task == NULL) return;
    printf("%s%.4f sn \t%s %s \t(id:%s \töncelik:%d \tkalan süre:%d sn)%s\n",
           task->color,                
           (double)G_CurrentTime,      
           task->id,                   
           status,                     
           task->id,                   
           task->priority,             
           task->remaining,            
           COLOR_RESET                 
    );
}

/* Scheduler Görevi */
static void schedulerTasking(void *pvParameters) {
    (void) pvParameters;
    
    while(G_ActiveTasks > 0) {
        /* Zaman aşımları kontrolü*/
        for (int i = 0; i < G_TaskCount; i++) {
            Task* task = &G_TaskList[i];
            
            if (task->remaining > 0 && task->arrival <= G_CurrentTime) {
                
                int timePassed = G_CurrentTime - task->arrival;
                
                if (timePassed >= 21) { 
                    print_task_log(task, "zaman aşımı");               
                    task->remaining = 0;
                    G_ActiveTasks--;

                    if (CurrentTask == task) {
                        CurrentTask = NULL;
                    }

                }
            }
        }
        
        /* Taskları kuyruklara yerleştirme */
        for (int i = 0; i < G_TaskCount; i++) {
            if (G_TaskList[i].arrival == G_CurrentTime) {
                Task* tempTaskPointer = &G_TaskList[i];
                if(G_TaskList[i].remaining > 0) {
                if(G_TaskList[i].priority == 0) {
                    xQueueSend(OncelikZero, &tempTaskPointer, 0);
                } else if(G_TaskList[i].priority == 1) {
                    xQueueSend(OncelikOne, &tempTaskPointer, 0);
                } else if(G_TaskList[i].priority == 2) {
                    xQueueSend(OncelikTwo, &tempTaskPointer, 0);
                } else if(G_TaskList[i].priority == 3) {
                    xQueueSend(OncelikThree, &tempTaskPointer, 0);
                }
                } 
            }
        }

        /* Task durum kontrolü*/
        if (CurrentTask != NULL) {          
            if (CurrentTask->remaining <= 0) {
                print_task_log(CurrentTask, "sonlandi"); 
                G_ActiveTasks--;
                CurrentTask = NULL; 
            }
            /* Taskın 1sn çalıştıktan sonra askıya alınması ve bir alt kuyruğa indirgenmesi*/
            else if (CurrentTask->priority > 0) {
                print_task_log(CurrentTask, "askıda");
                
                Task* taskToMove = CurrentTask;
                if(CurrentTask->priority == 1) {
                    CurrentTask->priority = 2;
                    xQueueSend(OncelikTwo, &taskToMove, 0);
                } else if(CurrentTask->priority == 2) {
                    CurrentTask->priority = 3;
                    xQueueSend(OncelikThree, &taskToMove, 0);
                } else {
                    xQueueSend(OncelikThree, &taskToMove, 0);
                }
                
                CurrentTask = NULL; 
            }
        }

        /* Sıradaki task seçimi*/
        if (CurrentTask == NULL) {
             Task* temp = NULL;
             while(CurrentTask== NULL && xQueueReceive(OncelikZero, &temp, 0) == pdTRUE) {
                if(temp->remaining > 0) {CurrentTask = temp;}
             }
             while(CurrentTask== NULL && xQueueReceive(OncelikOne, &temp, 0) == pdTRUE) {
                if(temp->remaining > 0) {CurrentTask = temp;}
             }
             while(CurrentTask== NULL && xQueueReceive(OncelikTwo, &temp, 0) == pdTRUE) {
                if(temp->remaining > 0) {CurrentTask = temp;}
             }
             while(CurrentTask== NULL && xQueueReceive(OncelikThree, &temp, 0) == pdTRUE) {
                if(temp->remaining > 0) {CurrentTask = temp;}
             }
            
            /*Eğer cpudaki taskımız ilk defa çalıştırılacaksa başladı yazılır*/
             if (CurrentTask != NULL) {
                 if (CurrentTask->burstTime == CurrentTask->remaining) {
                     print_task_log(CurrentTask, "başladı");
                 }
             }
        }

        if(CurrentTask != NULL) {
            /*task yürütülmesi*/
            print_task_log(CurrentTask, "yürütülüyor");
            CurrentTask->remaining--;
        } 
        G_CurrentTime++;
        vTaskDelay(pdMS_TO_TICKS(100));
    }

    exit(0);
}