#ifndef SHARED_STATE_H
#define SHARED_STATE_H

#include <stdint.h>
#include <stdbool.h>
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

/* ================= 1. GỌI CÁC BẢN VẼ TỪ TẦNG ĐÁY ================= */
// TUYỆT ĐỐI KHÔNG định nghĩa lại struct GPS_Data_t ở đây nữa!
#include "drivers/gps/gps_types.h" 
#include "drivers/mpu/mpu_type.h"



/* ================= 2. CẤU TRÚC KÉT SẮT TỔNG ================= */
// Đây là nơi chứa toàn bộ "linh hồn" của hệ thống V2V
typedef struct {
    // --- Khối Dữ liệu Đầu vào (Cập nhật bởi Task_Sensor qua Callback) ---
    GPS_Data_t gps_data;
    MPU_Data_t mpu_data;
    
    // --- Khối Dữ liệu Đầu ra (Cập nhật bởi Task_Algo) ---
    int risk_level;        // 0: Safe, 1: Warning, 2: Danger
    bool is_system_ready;
    uint32_t last_update_tick;
} System_State_t;


/* ================= 3. KHAI BÁO BIẾN TOÀN CỤC ================= */

// Khai báo Két sắt và Chìa khóa để các file khác gọi được
extern System_State_t System_State;
extern SemaphoreHandle_t Mutex_System_State;

void Shared_State_Init(void);
#endif // SHARED_STATE_H