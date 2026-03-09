#ifndef SYSTEM_TASK_H
#define SYSTEM_TASK_H

#include "drivers/gps/gps_types.h"
#include "drivers/mpu/mpu_type.h"

// callback cập nhật dữ liệu GPS vào shared state
void Callback_Update_GPS_State(GPS_Data_t *new_data);

// task đọc dữ liệu MPU
void Task_Sensor_MPU(void *pvParameters);

// task xử lý thuật toán (TTC)
void Task_Algo(void *pvParameters);

#endif