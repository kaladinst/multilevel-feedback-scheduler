#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "FreeRTOS.h"
#include "task.h"
#include "scheduler.h" 

int main(int argc, char *argv[]) {
    if(argc < 2) {
        printf("lütfen dosya adını giriniz.");
        return 1;
    }
    /*TXT okuması ve scheduleri başlatan fonksiyonun çağrımı*/
    SchedulerInitializer(argv[1]);
    /*Schedulerin başlatılması*/
    SchedulerStarter();
    return 0;
}