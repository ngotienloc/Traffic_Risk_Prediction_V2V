#ifndef APP_CONFIG_H
#define APP_CONFIG_H

/* ==========================================
 * 1. CẤU HÌNH CHÂN PHẦN CỨNG (HARDWARE PINS)
 * ========================================== */
// Cảm biến MPU6050 (Giao tiếp I2C)
#define I2C_MASTER_SCL_IO           22      // Chân SCL của ESP32
#define I2C_MASTER_SDA_IO           21      // Chân SDA của ESP32
#define I2C_MASTER_NUM              0       // Dùng cổng I2C số 0
#define I2C_MASTER_FREQ_HZ          400000  // Tần số I2C (400kHz - Tốc độ cao)

// Module GPS NEO-6M (Giao tiếp UART)
#define GPS_UART_NUM                1       // Dùng cổng UART số 1
#define GPS_TX_PIN                  17      // Chân TX của ESP nối với RX của GPS
#define GPS_RX_PIN                  16      // Chân RX của ESP nối với TX của GPS
#define GPS_BAUD_RATE               9600    // Tốc độ Baudrate mặc định của NEO-6M

/* ==========================================
 * 2. CẤU HÌNH FREERTOS TASKS
 * ========================================== */
// Định nghĩa Lõi CPU (ESP32 có 2 lõi: 0 và 1)
#define CORE_0                      0
#define CORE_1                      1

// Kích thước Stack (Kích thước "bàn làm việc" cho từng Task)
#define TASK_STACK_SENSOR           4096    // 4KB cho Task đọc cảm biến
#define TASK_STACK_ALGO             8192    // 8KB cho Task Thuật toán (vì tính toán số thực tốn RAM)
#define TASK_STACK_COMM             4096    // 4KB cho Task mạng ESP-NOW
#define TASK_STACK_DISPLAY          4096    // 4KB cho Task vẽ màn hình TFT

// Mức độ ưu tiên (Priority - Số càng lớn càng được ưu tiên chạy trước)
#define TASK_PRIO_ALGO              5       // Bộ não hệ thống: Ưu tiên Tối cao
#define TASK_PRIO_SENSOR            4       // Mắt/Tai thu thập: Ưu tiên số 2
#define TASK_PRIO_COMM              3       // Mạng WiFi: Ưu tiên số 3
#define TASK_PRIO_DISPLAY           2       // Vẽ màn hình (không quá gấp): Ưu tiên thấp nhất

/* ==========================================
 * 3. CẤU HÌNH THUẬT TOÁN & HỆ THỐNG
 * ========================================== */
#define SENSOR_READ_DELAY_MS        10      // Chu kỳ đọc MPU6050 (10ms = 100Hz)
#define RISK_THRESHOLD_WARNING      1.0f    // Ngưỡng gia tốc báo động Vàng (G)
#define RISK_THRESHOLD_DANGER       2.0f    // Ngưỡng gia tốc báo động Đỏ (G)

#endif // APP_CONFIG_H