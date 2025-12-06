#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../FreeRTOS/include/FreeRTOS.h"
#include "task.h"
#include "scheduler.h" 

int main(int argc, char *argv[]) {
    if(argc < 2) {
        printf("lütfen dosya adını giriniz.");
        return 1;
    }

    SchedulerInitializer(argv[1]);
    SchedulerStarter();
    return 0;
}


void vApplicationIdleHook(void){

}

 

void vApplicationTickHook(void) {

}