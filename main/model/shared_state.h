#ifndef SHARED_STATE_H
#define SHARED_STATE_H

#include <stdint.h>
#include <stdbool.h>
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"

/* ================= SENSOR DATA ================= */

typedef struct {
    float accel_x;
    float accel_y;
    float accel_z;
    float heading;
} MPU_Data_t;

typedef struct {
    float latitude;
    float longitude;
    float speed;
    float heading;
    uint32_t timestamp;
    bool is_valid;
} GPS_Data_t;

/* ================= SYSTEM STATE ================= */

typedef struct {
    int risk_level;        // 0 safe, 1 warning, 2 danger
    bool is_system_ready;
    uint32_t last_update_tick;
} System_State_t;

/* ================= GLOBAL OBJECTS ================= */

extern QueueHandle_t Queue_MPU_Data;
extern QueueHandle_t Queue_GPS_Data;

extern System_State_t System_State;
extern SemaphoreHandle_t Mutex_System_State;

#endif