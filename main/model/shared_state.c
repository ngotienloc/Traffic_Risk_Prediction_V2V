#include "shared_state.h"

/* Queues */
QueueHandle_t Queue_MPU_Data = NULL;
QueueHandle_t Queue_GPS_Data = NULL;

/* System State */
System_State_t System_State = {0};
SemaphoreHandle_t Mutex_System_State = NULL;