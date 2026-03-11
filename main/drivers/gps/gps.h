#ifndef GPS_H
#define GPS_H

#include "drivers/gps/gps_types.h"
#include <time.h>

typedef void (*gps_callback_t)(GPS_Data_t *data);

void GPS_Init(void);
void GPS_RegisterCallback(gps_callback_t cb);
void GPS_Task(void *pvParameters);
GPS_Data_t GPS_GetData(void);

#endif