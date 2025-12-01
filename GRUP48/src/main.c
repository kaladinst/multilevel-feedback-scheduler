#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../FreeRTOS/include/FreeRTOS.h"
#include "task.h"
#include "scheduler.h" 

int main(void) {
    printf("----------------------\n");
    printf("MAC OS BUILD TEST\n");
    printf("----------------------\n");

    vSchedulerInit();
    vSchedulerStart();
    return 0;
}


void vApplicationIdleHook(void){

}

 

void vApplicationTickHook(void) {

}