#include "mpu6050.h"
#include "i2c_mpu.h"
#include "esp_log.h"

static const char *TAG = "MPU6050";

// địa chỉ và register chính
#define MPU6050_ADDR         0x68
#define MPU6050_PWR_MGMT_1   0x6B
#define MPU6050_ACCEL_XOUT_H 0x3B

// hệ số chuyển đổi sang đơn vị g (±2g)
#define ACCEL_LSB_SENSITIVITY 16384.0f 

bool MPU6050_Init(void) {

    // init I2C bus
    if (i2c_master_init() != ESP_OK) {
        ESP_LOGE(TAG, "I2C init failed");
        return false;
    }

    // wake up MPU6050
    esp_err_t err = i2c_write_register(MPU6050_ADDR, MPU6050_PWR_MGMT_1, 0x00);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "MPU6050 not detected (check SDA/SCL)");
        return false;
    }

    ESP_LOGI(TAG, "MPU6050 initialized");
    return true;
}

bool MPU6050_Read_Accel(MPU_Data_t *data) {

    uint8_t buffer[6];

    // đọc 6 byte accel X/Y/Z
    if (i2c_read_registers(MPU6050_ADDR, MPU6050_ACCEL_XOUT_H, buffer, 6) == ESP_OK) {

        // ghép MSB + LSB
        int16_t raw_x = (buffer[0] << 8) | buffer[1];
        int16_t raw_y = (buffer[2] << 8) | buffer[3];
        int16_t raw_z = (buffer[4] << 8) | buffer[5];

        // convert sang g
        data->accel_x = (float)raw_x / ACCEL_LSB_SENSITIVITY;
        data->accel_y = (float)raw_y / ACCEL_LSB_SENSITIVITY;
        data->accel_z = (float)raw_z / ACCEL_LSB_SENSITIVITY;

        return true;
    }

    return false;
}