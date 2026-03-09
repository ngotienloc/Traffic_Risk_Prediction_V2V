#ifndef I2C_MPU_H
#define I2C_MPU_H

#include <stdint.h>
#include "esp_err.h"
#include "app_config.h"

esp_err_t i2c_master_init(void);
esp_err_t i2c_write_register(uint8_t dev_addr, uint8_t reg_addr, uint8_t data);
esp_err_t i2c_read_registers(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, size_t len);

#endif // I2C_MPU_H