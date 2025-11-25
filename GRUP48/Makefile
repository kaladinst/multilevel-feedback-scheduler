
# Derleme komutu
freertos_sim: src/main.c src/scheduler.c src/tasks.c src/FreeRTOSConfig.h \
              FreeRTOS/source/tasks.c \
              FreeRTOS/source/list.c \
              FreeRTOS/source/queue.c \
              FreeRTOS/source/timers.c \
              FreeRTOS/source/event_groups.c \
              FreeRTOS/source/stream_buffer.c \
              FreeRTOS/portable/ThirdParty/GCC/Posix/port.c \
              FreeRTOS/portable/ThirdParty/GCC/Posix/utils/wait_for_event.c \
              FreeRTOS/portable/MemMang/heap_3.c
#Çalıştırma komutu
	gcc \
	-Wall -Wextra -g -DPROJ_MAC_OS \
	-I./src \
	-I./FreeRTOS/include \
	-I./FreeRTOS/portable/ThirdParty/GCC/Posix \
	-I.