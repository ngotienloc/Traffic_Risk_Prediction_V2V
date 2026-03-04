#ifndef NEIGHBOR_TABLE_H
#define NEIGHBOR_TABLE_H

#include <stdint.h>
#include <stdbool.h>
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

/* ================= CONFIG ================= */

#define MAX_NEIGHBORS       5
#define NEIGHBOR_TIMEOUT_MS 2000

/* ================= STRUCT ================= */

typedef struct {
    uint32_t vehicle_id;
    float latitude;
    float longitude;
    float speed;
    float heading;

    uint32_t gps_timestamp;   // timestamp xe gửi
    uint32_t last_rx_tick;    // tick nội bộ khi nhận
    bool is_active;
} Neighbor_t;

typedef struct {
    Neighbor_t list[MAX_NEIGHBORS];
    int count;
} Neighbor_Table_t;

/* ================= API ================= */

void NeighborTable_Init(void);

void NeighborTable_Update(uint32_t vehicle_id,
                          float lat,
                          float lon,
                          float speed,
                          float heading,
                          uint32_t gps_timestamp);

void NeighborTable_RemoveTimeout(void);

Neighbor_Table_t* NeighborTable_GetHandle(void);

#endif