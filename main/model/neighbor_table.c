#include "neighbor_table.h"
#include <string.h>

static Neighbor_Table_t Neighbor_Table;
static SemaphoreHandle_t Mutex_Neighbor_Table;

/* ========================================================== */

void NeighborTable_Init(void)
{
    memset(&Neighbor_Table, 0, sizeof(Neighbor_Table));
    Mutex_Neighbor_Table = xSemaphoreCreateMutex();
}

/* ========================================================== */

Neighbor_Table_t* NeighborTable_GetHandle(void)
{
    return &Neighbor_Table;
}

/* ========================================================== */

void NeighborTable_Update(uint32_t vehicle_id,
                          float lat,
                          float lon,
                          float speed,
                          float heading,
                          uint32_t gps_timestamp)
{
    if (Mutex_Neighbor_Table == NULL)
        return;

    xSemaphoreTake(Mutex_Neighbor_Table, portMAX_DELAY);

    /* 1. Update nếu đã tồn tại */
    for (int i = 0; i < MAX_NEIGHBORS; i++)
    {
        if (Neighbor_Table.list[i].is_active &&
            Neighbor_Table.list[i].vehicle_id == vehicle_id)
        {
            Neighbor_Table.list[i].latitude = lat;
            Neighbor_Table.list[i].longitude = lon;
            Neighbor_Table.list[i].speed = speed;
            Neighbor_Table.list[i].heading = heading;
            Neighbor_Table.list[i].gps_timestamp = gps_timestamp;
            Neighbor_Table.list[i].last_rx_tick = xTaskGetTickCount();

            xSemaphoreGive(Mutex_Neighbor_Table);
            return;
        }
    }

    /* 2. Thêm mới nếu còn slot */
    for (int i = 0; i < MAX_NEIGHBORS; i++)
    {
        if (!Neighbor_Table.list[i].is_active)
        {
            Neighbor_Table.list[i].vehicle_id = vehicle_id;
            Neighbor_Table.list[i].latitude = lat;
            Neighbor_Table.list[i].longitude = lon;
            Neighbor_Table.list[i].speed = speed;
            Neighbor_Table.list[i].heading = heading;
            Neighbor_Table.list[i].gps_timestamp = gps_timestamp;
            Neighbor_Table.list[i].last_rx_tick = xTaskGetTickCount();
            Neighbor_Table.list[i].is_active = true;

            Neighbor_Table.count++;
            break;
        }
    }

    xSemaphoreGive(Mutex_Neighbor_Table);
}

/* ========================================================== */

void NeighborTable_RemoveTimeout(void)
{
    if (Mutex_Neighbor_Table == NULL)
        return;

    uint32_t now = xTaskGetTickCount();

    xSemaphoreTake(Mutex_Neighbor_Table, portMAX_DELAY);

    for (int i = 0; i < MAX_NEIGHBORS; i++)
    {
        if (Neighbor_Table.list[i].is_active)
        {
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