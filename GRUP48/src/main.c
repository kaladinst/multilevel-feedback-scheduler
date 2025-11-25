#include <stdio.h>
#include <stdlib.h>

/* FreeRTOS Includes */
#include "FreeRTOS.h"
#include "task.h"

int main(void) {
    printf("----------------------\n");
    printf("MAC OS BUILD TEST\n");
    printf("----------------------\n");

    return 0;
}

/* FreeRTOS Hooks */
void vApplicationIdleHook(void) 
{
} 

void vApplicationTickHook(void) 
{
}