#ifndef MPU_TYPES_H
#define MPU_TYPES_H

// ĐÂY LÀ NƠI DUY NHẤT ĐỊNH NGHĨA MPU_Data_t
typedef struct {
    float accel_x;  // Đo phanh gấp / tăng tốc (g)
    float accel_y;  // Đo lực văng ngang (g)
    float accel_z;  // Đo xóc nảy (g)
    
    // Đã bỏ biến heading đi theo như chốt phương án dùng GPS cho heading
} MPU_Data_t;

#endif // MPU_TYPES_H