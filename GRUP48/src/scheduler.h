#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "FreeRTOS.h"
#include "task.h"
#define COLOR_RESET   "\x1b[0m"
#define MAX_TASKS 25
/*Fonksiyon bildirimleri*/
void SchedulerInitializer(const char *filename);
void SchedulerStarter(void);
#endif