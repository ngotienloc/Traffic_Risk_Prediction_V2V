#ifndef SHARED_STATE_H
#define SHARED_STATE_H

#include <stdint.h>
#include <stdbool.h>
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

// import kiểu dữ liệu từ driver
#include "drivers/gps/gps_types.h"
#include "drivers/mpu/mpu_type.h"


// trạng thái hệ thống dùng chung giữa các task
typedef struct {

    // dữ liệu cảm biến
    GPS_Data_t gps_data;
    MPU_Data_t mpu_data;

    // kết quả xử lý
    int risk_level;            // 0: safe, 1: warning, 2: danger
    bool is_system_ready;

    uint32_t last_update_tick;

} System_State_t;


// shared state + mutex bảo vệ
extern System_State_t System_State;
extern SemaphoreHandle_t Mutex_System_State;

void Shared_State_Init(void);

#endif // SHARED_STATE_H