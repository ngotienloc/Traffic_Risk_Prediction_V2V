#ifndef MPU_TYPES_H
#define MPU_TYPES_H

// Cấu trúc dữ liệu MPU
typedef struct {
    float accel_x;  // gia tốc trục X (g)
    float accel_y;  // gia tốc trục Y (g)
    float accel_z;  // gia tốc trục Z (g)
} MPU_Data_t;

#endif // MPU_TYPES_H