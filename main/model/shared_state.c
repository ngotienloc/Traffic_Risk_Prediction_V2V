#include "shared_state.h"
#include <string.h>

/* ==========================================================
 * GLOBAL VARIABLES
 * ========================================================== */

QueueHandle_t Queue_MPU_Data = NULL; 
QueueHandle_t Queue_GPS_Data = NULL; 

Neighbor_Table_t Neighbor_Table; 
SemaphoreHandle_t Mutex_Neighbor_Table = NULL;

System_State_t System_State = {0}; 
SemaphoreHandle_t Mutex_System_State = NULL;

/* ==========================================================
 * INITIALIZATION
 * ========================================================== */

void NeighborTable_Init(void)
{
    memset(&Neighbor_Table, 0, sizeof(Neighbor_Table));
}

/* ==========================================================
 * UPDATE OR INSERT NEIGHBOR
 * ========================================================== */

void NeighborTable_Update(uint32_t vehicle_id,
                          float lat,
                          float lon,
                          float speed,
                          float heading,
                          uint32_t gps_timestamp)
{
    if (Mutex_Neighbor_Table == NULL)
        return;

    uint32_t now_tick = xTaskGetTickCount();

    xSemaphoreTake(Mutex_Neighbor_Table, portMAX_DELAY);

    for (int i = 0; i < MAX_NEIGHBORS; i++) {

        if (Neighbor_Table.list[i].is_active &&
            Neighbor_Table.list[i].vehicle_id == vehicle_id)
        {
            Neighbor_Table.list[i].latitude      = lat;
            Neighbor_Table.list[i].longitude     = lon;
            Neighbor_Table.list[i].speed         = speed;
            Neighbor_Table.list[i].heading       = heading;
            Neighbor_Table.list[i].timestamp = gps_timestamp;
            Neighbor_Table.list[i].last_rx_tick  = now_tick;

            xSemaphoreGive(Mutex_Neighbor_Table);
            return;
        }
    }

    for (int i = 0; i < MAX_NEIGHBORS; i++) {

        if (!Neighbor_Table.list[i].is_active) {

            Neighbor_Table.list[i].vehicle_id    = vehicle_id;
            Neighbor_Table.list[i].latitude      = lat;
            Neighbor_Table.list[i].longitude     = lon;
            Neighbor_Table.list[i].speed         = speed;
            Neighbor_Table.list[i].heading       = heading;
            Neighbor_Table.list[i].timestamp     = gps_timestamp;
            Neighbor_Table.list[i].last_rx_tick  = now_tick;
            Neighbor_Table.list[i].is_active     = true;

            Neighbor_Table.count++;
            break;
        }
    }

    xSemaphoreGive(Mutex_Neighbor_Table);
}

/* ==========================================================
 * REMOVE TIMEOUT NEIGHBORS
 * ========================================================== */

void Neighbor_Table_RemoveTimeout(void)
{
    if (Mutex_Neighbor_Table == NULL)
        return;

    uint32_t now = xTaskGetTickCount();

    xSemaphoreTake(Mutex_Neighbor_Table, portMAX_DELAY);

    for (int i = 0; i < MAX_NEIGHBORS; i++) {

        if (Neighbor_Table.list[i].is_active) {

            if ((now - Neighbor_Table.list[i].last_rx_tick) >
                pdMS_TO_TICKS(NEIGHBOR_TIMEOUT_MS))
            {
                Neighbor_Table.list[i].is_active = false;
                Neighbor_Table.count--;
            }
        }
    }

    xSemaphoreGive(Mutex_Neighbor_Table);
}