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

void NeighborTable_Update(uint32_t vehicle_id[6],
                          float lat,
                          float lon,
                          float speed,
                          float heading,
                          uint32_t timestamp)
{
    if(Mutex_Neighbor_Table == NULL){
        return; 
    }

    xSemaphoreTake(Mutex_Neighbor_Table, portMAX_DELAY);

    

}

