#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "scheduler.h"
#include "tasks.h"

/* Global Değişkenler */
QueueHandle_t OncelikZero;
QueueHandle_t OncelikOne;
QueueHandle_t OncelikTwo;
QueueHandle_t OncelikThree;
Task G_TaskList[MAX_TASKS];
Task* CurrentTask = NULL;
int G_TaskCount = 0;
int G_CurrentTime = 0;
int G_ActiveTasks = 0;

/*Kuyrukların oluşturulması*/
void SchedulerStarter(void) {
    OncelikZero = xQueueCreate(10, sizeof(Task*));
    OncelikOne = xQueueCreate(10, sizeof(Task*));
    OncelikTwo = xQueueCreate(10, sizeof(Task*));
    OncelikThree = xQueueCreate(10, sizeof(Task*));

    xTaskCreate(schedulerTasking, "TaskManager", 2048, NULL, 4, NULL);
    vTaskStartScheduler();
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
                // Eğer birinci öncelikte ise önceliği 2 yapılır ve xQueueSend fonksiyonu ile ikinci kuyruğa gönderilir
                if(CurrentTask->priority == 1) {
                    CurrentTask->priority = 2;
                    xQueueSend(OncelikTwo, &taskToMove, 0);
                }
                // Eğer ikinci öncelikte ise önceliği 3 yapılır ve xQueueSend fonksiyonu ile üçüncü kuyruğa gönderilir 
                else if(CurrentTask->priority == 2) {
                    CurrentTask->priority = 3;
                    xQueueSend(OncelikThree, &taskToMove, 0);
                } 
                // Eğer üçüncü öncelikte ise önceliği değişmez. kendi kuyruğunun sonuna gönderilir.
                else {
                    xQueueSend(OncelikThree, &taskToMove, 0);
                }
                //task 1sn çalıştıktan sonra cpu boşaltılır.
                CurrentTask = NULL;
            }
        }

        /* Sıradaki task seçimi*/
        if (CurrentTask == NULL) {
             Task* temp = NULL;
            // 0.öncelik kuyruğu kontrol edilir. task varsa alınır.
             while(CurrentTask== NULL && xQueueReceive(OncelikZero, &temp, 0) == pdTRUE) {
                if(temp->remaining > 0) {CurrentTask = temp;}
             }
             // 1.öncelik kuyruğu kontrol edilir. task varsa alınır.
             while(CurrentTask== NULL && xQueueReceive(OncelikOne, &temp, 0) == pdTRUE) {
                if(temp->remaining > 0) {CurrentTask = temp;}
             }
             // 2.öncelik kuyruğu kontrol edilir. task varsa alınır.
             while(CurrentTask== NULL && xQueueReceive(OncelikTwo, &temp, 0) == pdTRUE) {
                if(temp->remaining > 0) {CurrentTask = temp;}
             }
             // 3.öncelik kuyruğu kontrol edilir. task varsa alınır.
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

        /*Zaman artımı ve okunabilirlik için gecikme ekleme*/
        G_CurrentTime++;
        vTaskDelay(pdMS_TO_TICKS(100));
    }
    // çıkış
    exit(0);
}