#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "FreeRTOS.h"
#include "task.h"
#define COLOR_RESET   "\x1b[0m"
/*Görev tanımlaması*/
typedef struct {
    char id[16];
    int arrival;
    int priority;
    int burstTime;

    int remaining;
    const char* color;

    TaskHandle_t handle;
}Task;

void SchedulerInitializer(const char *filename);
void SchedulerStarter(void);
static void schedulerTasking(void *pvParameters);

#endif