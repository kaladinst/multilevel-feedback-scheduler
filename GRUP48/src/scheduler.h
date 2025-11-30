#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "FreeRTOS.h"
#include "task.h"

/* ANSI Color Codes (Hex Format - Safer for macOS/VS Code) */
#define COLOR_RESET   "\x1b[0m"
#define COLOR_RED     "\x1b[31m"
#define COLOR_GREEN   "\x1b[32m"
#define COLOR_YELLOW  "\x1b[33m"
#define COLOR_BLUE    "\x1b[34m"
#define COLOR_MAGENTA "\x1b[35m"
#define COLOR_CYAN    "\x1b[36m"
#define COLOR_WHITE   "\x1b[37m"

typedef struct {
    char id[16];
    int arrival;
    int priority;
    int burstTime;

    int remaining;
    const char* color;

    TaskHandle_t handle;
} Task_t;

void vSchedulerInit(void);
void vSchedulerStart(void);

#endif