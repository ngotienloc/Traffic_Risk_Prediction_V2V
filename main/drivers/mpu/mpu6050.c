#include "mpu6050.h"
#include "i2c_mpu.h"
#include "esp_log.h"

static const char *TAG = "MPU6050";


/* ================= Init ================= */

bool MPU6050_Init(void)
{
    if (i2c_master_init() != ESP_OK)
    {
        ESP_LOGE(TAG, "I2C init failed");
        return false;
    }

    // wake sensor
    esp_err_t err = i2c_write_register(
        MPU6050_I2C_ADDR,
        MPU6050_PWR_MGMT_1,
        0x00
    );

    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "MPU6050 not detected (check wiring)");
        return false;
    }

    // set sample rate 100Hz
    i2c_write_register(MPU6050_I2C_ADDR, MPU6050_SMPLRT_DIV, 9);

    // enable DLPF 44Hz
    i2c_write_register(MPU6050_I2C_ADDR, MPU6050_CONFIG_REG, 3);

    // set accelerometer range (±2g)
    i2c_write_register(
        MPU6050_I2C_ADDR,
        MPU6050_ACCEL_CONFIG,
        MPU6050_ACCEL_RANGE
    );

    // set gyro range (±250 deg/s)
    i2c_write_register(
        MPU6050_I2C_ADDR,
        MPU6050_GYRO_CONFIG,
        MPU6050_GYRO_RANGE
    );

    ESP_LOGI(TAG, "MPU6050 initialized");

    return true;
}


/* ================= Read Acceleration ================= */

bool MPU6050_Read_Accel(MPU_Data_t *data)
{
    if (data == NULL)
        return false;

    uint8_t buffer[6];

    if (i2c_read_registers(
            MPU6050_I2C_ADDR,
            MPU6050_ACCEL_XOUT_H,
            buffer,
            6) != ESP_OK)
    {
        ESP_LOGE(TAG, "I2C read failed");
        return false;
    }

    int16_t raw_x = (buffer[0] << 8) | buffer[1];
    int16_t raw_y = (buffer[2] << 8) | buffer[3];
    int16_t raw_z = (buffer[4] << 8) | buffer[5];

    data->accel_x = (float)raw_x / MPU6050_ACCEL_SCALE;
    data->accel_y = (float)raw_y / MPU6050_ACCEL_SCALE;
    data->accel_z = (float)raw_z / MPU6050_ACCEL_SCALE;

    return true;
}