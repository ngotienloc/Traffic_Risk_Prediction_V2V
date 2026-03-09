#ifndef MPU6050_H
#define MPU6050_H

#include <stdbool.h>
#include "drivers/mpu/mpu_type.h" 

// Khởi tạo và đánh thức MPU6050
bool MPU6050_Init(void);

// Đọc 3 trục gia tốc
bool MPU6050_Read_Accel(MPU_Data_t *data);

#endif // MPU6050_H