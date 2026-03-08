#include "system/system_tasks.h"
#include "model/shared_state.h" // Cầm chìa khóa Mutex
#include "esp_log.h"

// ---------------------------------------------------------
// 1. DÀNH CHO GPS: HÀM CALLBACK (Cơ chế ĐẨY - Push)
// ---------------------------------------------------------
void Callback_Update_GPS_State(GPS_Data_t *new_data) {
    // 1. Dùng tên ổ khóa mới: Mutex_System_State
    if (xSemaphoreTake(Mutex_System_State, portMAX_DELAY) == pdTRUE) {
        
        // 2. Dùng tên két sắt mới: System_State.gps_data
        System_State.gps_data = *new_data; 
        
        // 3. Trả ổ khóa và đã XÓA CHỮ
        xSemaphoreGive(Mutex_System_State); 
    }
}