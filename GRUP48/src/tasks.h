#ifndef TASKS_H
#define TASKS_H

#include "FreeRTOS.h"
#include "task.h"

/*Task yapısını oluşturma*/
typedef struct {
    char id[16];
    int arrival;
    int priority;
    int burstTime;
    int remaining;
    int lastWorkedTime;
    int startTime;
    const char* color;
    TaskHandle_t handle;
} Task;

/*Fonksiyon bildirimleri*/
const char* getRandomColor(void);
void print_task_log(Task *task, const char *status);
void SchedulerInitializer(const char *filename);

#endif