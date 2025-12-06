
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "FreeRTOS.h"
#include "task.h"
#include "scheduler.h"
#include "tasks.h"

extern Task G_TaskList[MAX_TASKS];
extern int G_TaskCount;
extern int G_CurrentTime;
extern int G_ActiveTasks;

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

/* Dosya okuması ve değişkenlerin atanması */
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