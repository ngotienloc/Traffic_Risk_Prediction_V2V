#ifndef MPU6050_H
#define MPU6050_H

#include <stdbool.h>
#include "drivers/mpu/mpu_type.h" 

bool MPU6050_Init(void);
bool MPU6050_Read_Accel(MPU_Data_t *data);

#endif // MPU6050_H