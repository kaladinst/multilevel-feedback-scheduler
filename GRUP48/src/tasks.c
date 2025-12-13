
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "FreeRTOS.h"
#include "task.h"
#include "scheduler.h"
#include "tasks.h"

extern Task TaskList[MAX_TASKS];
extern int TaskCount;
extern int CurrentTime;
extern int ActiveTasks;

/* Renk Seçici */
const char* getRandomColor() {
    static const char* colors[] = {
        "\033[1;31m",
        "\033[1;32m",
        "\033[1;33m", 
        "\033[1;34m",
        "\033[1;35m",
        "\033[1;36m",
        "\033[38;5;208m",
        "\033[38;5;198m",
        "\033[38;5;118m",
        "\033[38;5;39m",
        "\033[38;5;93m",
        "\033[38;5;220m",
        "\033[38;5;51m",
        "\033[38;5;160m",
    };    
    return colors[rand() % 14];
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
        sprintf(TaskList[i].id, "%04d", i);
        TaskList[i].arrival   = arrival;
        TaskList[i].priority  = priority;
        TaskList[i].burstTime = burst;
        TaskList[i].remaining = burst;
        TaskList[i].color     = getRandomColor();
        TaskList[i].handle    = NULL;
        TaskList[i].lastWorkedTime = arrival;
        TaskList[i].startTime = 0;
        i++;
        TaskCount++;
    }

    ActiveTasks = TaskCount;
    fclose(file);
}

/* Log Yazdırma */
void print_task_log(Task *task, const char *status) {
    if (task == NULL) return;
    printf("%s%.4f sn \t%s %s \t(id:%s \töncelik:%d \tkalan süre:%d sn)%s\n",
           task->color,                
           (double)CurrentTime,      
           task->id,                   
           status,                     
           task->id,                   
           task->priority,             
           task->remaining,            
           COLOR_RESET                 
    );
}