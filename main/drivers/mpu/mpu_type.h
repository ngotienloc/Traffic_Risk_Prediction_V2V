#ifndef MPU_TYPES_H
#define MPU_TYPES_H

/* =====================================================
   MPU6050 data structure
   ===================================================== */

typedef struct
{
    float accel_x;     // acceleration X (g)
    float accel_y;     // acceleration Y (g)
    float accel_z;     // acceleration Z (g)

    float gyro_x;      // angular velocity X (deg/s)
    float gyro_y;      // angular velocity Y (deg/s)
    float gyro_z;      // angular velocity Z (deg/s)

} MPU_Data_t;

#endif // MPU_TYPES_H