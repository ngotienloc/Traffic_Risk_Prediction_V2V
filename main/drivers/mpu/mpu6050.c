#include "mpu6050.h"
#include "i2c_mpu.h"
#include "esp_log.h"

static const char *TAG = "MPU6050";

// Các địa chỉ thanh ghi quan trọng của MPU6050
#define MPU6050_ADDR          0x68  // Địa chỉ I2C mặc định
#define MPU6050_PWR_MGMT_1    0x6B  // Thanh ghi quản lý nguồn
#define MPU6050_ACCEL_XOUT_H  0x3B  // Thanh ghi chứa data X, Y, Z (6 byte liên tiếp)

// Hệ số chuyển đổi để ra đơn vị (g) với dải đo mặc định +-2g
#define ACCEL_LSB_SENSITIVITY 16384.0f 

bool MPU6050_Init(void) {
    // 1. Khởi tạo bus I2C trước
    if (i2c_master_init() != ESP_OK) {
        ESP_LOGE(TAG, "Lỗi khởi tạo cổng I2C!");
        return false;
    }

    // 2. Viết số 0 vào thanh ghi nguồn để "đánh thức" MPU6050
    esp_err_t err = i2c_write_register(MPU6050_ADDR, MPU6050_PWR_MGMT_1, 0x00);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Không tìm thấy MPU6050! Vui lòng kiểm tra dây nối SDA/SCL.");
        return false;
    }

    ESP_LOGI(TAG, "Khởi tạo và kết nối MPU6050 thành công!");
    return true;
}

bool MPU6050_Read_Accel(MPU_Data_t *data) {
    uint8_t buffer[6]; // Tạo giỏ hứng 6 byte từ 3 trục (X, Y, Z mỗi trục 2 byte)

    // Đọc một mạch 6 thanh ghi liên tiếp bắt đầu từ 0x3B
    if (i2c_read_registers(MPU6050_ADDR, MPU6050_ACCEL_XOUT_H, buffer, 6) == ESP_OK) {
        
        // Ghép 2 byte lại thành số nguyên 16-bit
        int16_t raw_x = (buffer[0] << 8) | buffer[1];
        int16_t raw_y = (buffer[2] << 8) | buffer[3];
        int16_t raw_z = (buffer[4] << 8) | buffer[5];

        // Chia cho hệ số để ra gia tốc trọng trường (G)
        data->accel_x = (float)raw_x / ACCEL_LSB_SENSITIVITY;
        data->accel_y = (float)raw_y / ACCEL_LSB_SENSITIVITY;
        data->accel_z = (float)raw_z / ACCEL_LSB_SENSITIVITY;
        
        return true;
    }
    return false;
}