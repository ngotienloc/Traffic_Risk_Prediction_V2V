#ifndef SYSTEM_TASK_H
#define SYSTEM_TASK_H

#include "drivers/gps/gps_types.h"

// Hàm Callback chuyên dùng để cập nhật GPS vào Shared_State
void Callback_Update_GPS_State(GPS_Data_t *new_data);

// Task quét dữ liệu cảm biến (sẽ dùng cho MPU6050)
void Task_Sensor_MPU(void *pvParameters);       // Chưa dùng đến + Để tạm đây.

// Task chạy thuật toán TTC
void Task_Algo(void *pvParameters); // Chưa dùng, để tạm ở đây.

#endif 