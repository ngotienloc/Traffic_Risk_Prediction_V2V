#ifndef SHARED_STATE_H
#define SHARED_STATE_H

#include <stdint.h>
#include <stdbool.h>

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"

/* ==========================================================
 * CONFIG
 * ========================================================== */

#define MAX_NEIGHBORS 5
#define NEIGHBOR_TIMEOUT_MS 2000

/* ==========================================================
 * 1. SENSOR DATA
 * ========================================================== */

typedef struct {
    float accel_x;
    float accel_y;
    float accel_z;
    float heading;      // IMU heading (0–360 deg)
} MPU_Data_t;

typedef struct {
    float latitude;
    float longitude;
    float speed;        // m/s
    float heading;      // GPS course over ground
    uint32_t timestamp; // GPS time (ms)
    bool is_valid;
} GPS_Data_t;

/* ==========================================================
 * 2. NEIGHBOR STRUCTURES
 * ========================================================== */

typedef struct {
    uint32_t vehicle_id;     
    float latitude;
    float longitude;
    float speed;
    float heading;
    uint32_t timestamp; // timestamp xe gửi
    bool is_active;
} Neighbor_t;

typedef struct {
    Neighbor_t list[MAX_NEIGHBORS];
    int count;
} Neighbor_Table_t;

/* ==========================================================
 * 3. SYSTEM STATE (Decision Layer)
 * ========================================================== */

typedef struct {
    int risk_level;         // 0 safe, 1 warning, 2 danger
    bool is_system_ready;
    uint32_t last_update_ms;
} System_State_t;

/* ==========================================================
 * 4. GLOBAL OBJECTS
 * ========================================================== */
 
/* Sensor queues (size = 1) */
extern QueueHandle_t Queue_MPU_Data;
extern QueueHandle_t Queue_GPS_Data;

/* Neighbor table */
extern Neighbor_Table_t Neighbor_Table;
extern SemaphoreHandle_t Mutex_Neighbor_Table;

/* System state */
extern System_State_t System_State;
extern SemaphoreHandle_t Mutex_System_State;

/* ==========================================================
 * 5. FUNCTION PROTOTYPES
 * ========================================================== */

void NeighborTable_Init(void);
void NeighborTable_Update(uint32_t vehicle_id,
                          float lat,
                          float lon,
                          float speed,
                          float heading,
                          uint32_t timestamp);

void NeighborTable_RemoveTimeout(uint32_t current_time);

#endif