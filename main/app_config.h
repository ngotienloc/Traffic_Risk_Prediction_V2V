#ifndef APP_CONFIG_H
#define APP_CONFIG_H

/* ================= Hardware configuration (ESP32-S3) ================= */

// MPU6050 - I2C
#define I2C_MASTER_SCL_IO           22
#define I2C_MASTER_SDA_IO           21
#define I2C_MASTER_NUM              0
#define I2C_MASTER_FREQ_HZ          400000     // 400 kHz
#define I2C_MASTER_TX_BUF_DISABLE   0
#define I2C_MASTER_RX_BUF_DISABLE   0

// GPS NEO-6M - UART
#define GPS_UART_NUM                1
#define GPS_TX_PIN                  17
#define GPS_RX_PIN                  18
#define GPS_BAUD_RATE               9600
#define GPS_BUF_SIZE                1024


/* ================= FreeRTOS task configuration ================= */

// CPU cores
#define CORE_0                      0
#define CORE_1                      1

// task stack size (bytes)
#define TASK_STACK_SENSOR           4096
#define TASK_STACK_ALGO             8192
#define TASK_STACK_COMM             4096
#define TASK_STACK_DISPLAY          4096

// task priority
#define TASK_PRIO_ALGO              5
#define TASK_PRIO_SENSOR            4
#define TASK_PRIO_COMM              3
#define TASK_PRIO_DISPLAY           2


/* ================= Algorithm parameters ================= */

// sensor sampling
#define SENSOR_READ_DELAY_MS        10      // 100 Hz

// self-behavior detection (MPU6050)
#define ACCEL_HARD_BRAKE            0.6f
#define ACCEL_CRASH_DETECTED        3.0f

// collision prediction (TTC)
#define TTC_WARNING_SEC             4.0f
#define TTC_DANGER_SEC              2.0f
#define SAFE_DISTANCE_MIN_M         5.0f

// ESP-NOW RSSI model
#define RSSI_REF_1M                 -40
#define RSSI_PATH_LOSS_EXP          2.5f
#define RSSI_FILTER_ALPHA           0.1f

#endif // APP_CONFIG_H