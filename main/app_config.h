#ifndef APP_CONFIG_H
#define APP_CONFIG_H

/* =========================================================
   Hardware configuration (ESP32-S3)
   ========================================================= */

/* ================= I2C - MPU6050 ================= */

#define I2C_MASTER_NUM              0
#define I2C_MASTER_SCL_IO           22
#define I2C_MASTER_SDA_IO           21
#define I2C_MASTER_FREQ_HZ          400000
#define I2C_MASTER_TX_BUF_DISABLE   0
#define I2C_MASTER_RX_BUF_DISABLE   0


/* ================= MPU6050 Sensor ================= */

#define MPU6050_I2C_ADDR            0x68

// accelerometer range
#define MPU6050_ACCEL_RANGE         0x00    // ±2g

// gyro range
#define MPU6050_GYRO_RANGE          0x00    // ±250 deg/s

// digital low pass filter
#define MPU6050_DLPF_CONFIG         0x03    // ~44Hz

// sample rate
#define MPU6050_SAMPLE_DIV          9       // 100Hz

// scale factors
#define MPU6050_ACCEL_SCALE         16384.0f
#define MPU6050_GYRO_SCALE          131.0f


/* ================= GPS NEO-6M (UART) ================= */

#define GPS_UART_NUM                1
#define GPS_TX_PIN                  17
#define GPS_RX_PIN                  18
#define GPS_BAUD_RATE               9600
#define GPS_BUF_SIZE                1024

// GPS update rate
#define GPS_UPDATE_RATE_HZ          10


/* =========================================================
   FreeRTOS task configuration
   ========================================================= */

#define CORE_0                      0
#define CORE_1                      1

#define TASK_STACK_SENSOR           4096
#define TASK_STACK_ALGO             8192
#define TASK_STACK_COMM             4096
#define TASK_STACK_DISPLAY          4096

#define TASK_PRIO_ALGO              5
#define TASK_PRIO_SENSOR            4
#define TASK_PRIO_COMM              3
#define TASK_PRIO_DISPLAY           2


/* =========================================================
   Sensor configuration
   ========================================================= */

// sensor sampling
#define SENSOR_READ_DELAY_MS        10      // 100Hz

// accelerometer filter
#define ACCEL_FILTER_ALPHA          0.2f

// GPS + MPU fusion
#define SENSOR_FUSION_ALPHA         0.7f


/* =========================================================
   Vehicle behavior detection
   ========================================================= */

// hard brake detection
#define ACCEL_HARD_BRAKE            0.6f

// crash detection
#define ACCEL_CRASH_DETECTED        3.0f


/* =========================================================
   Collision prediction (TTC)
   ========================================================= */

#define TTC_WARNING_SEC             4.0f
#define TTC_DANGER_SEC              2.0f
#define SAFE_DISTANCE_MIN_M         5.0f


/* =========================================================
   ESP-NOW distance estimation
   ========================================================= */

#define RSSI_REF_1M                 -40
#define RSSI_PATH_LOSS_EXP          2.5f
#define RSSI_FILTER_ALPHA           0.1f


#endif