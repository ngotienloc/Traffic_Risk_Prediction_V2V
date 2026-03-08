#ifndef APP_CONFIG_H
#define APP_CONFIG_H

/* ==========================================
 * 1. CẤU HÌNH PHẦN CỨNG (HARDWARE PINS - ESP32-S3)
 * ========================================== */
// Cảm biến MPU6050 (Giao tiếp I2C)
#define I2C_MASTER_SCL_IO           22      // Chân SCL
#define I2C_MASTER_SDA_IO           21      // Chân SDA
#define I2C_MASTER_NUM              0       // I2C Port 0
#define I2C_MASTER_FREQ_HZ          400000  // 400kHz (Fast Mode)

// Module GPS NEO-6M (Giao tiếp UART)
#define GPS_UART_NUM                1       // UART Port 1
#define GPS_TX_PIN                  17      // ESP TX nối GPS RX
#define GPS_RX_PIN                  18      // ESP RX nối GPS TX
#define GPS_BAUD_RATE               9600    // Baudrate NMEA chuẩn
#define GPS_BUF_SIZE                1024
/* ==========================================
 * 2. CẤU HÌNH FREERTOS TASKS
 * ========================================== */
// Định nghĩa Lõi CPU
#define CORE_0                      0       // Dùng cho IPC và Output
#define CORE_1                      1       // Dùng cho Input và Xử lý lõi

// Kích thước Stack (Tính bằng Byte)
#define TASK_STACK_SENSOR           4096
#define TASK_STACK_ALGO             8192    // Cần nhiều RAM để tính toán vector/float
#define TASK_STACK_COMM             4096
#define TASK_STACK_DISPLAY          4096

// Mức độ ưu tiên (Số càng lớn -> Ưu tiên càng cao)
#define TASK_PRIO_ALGO              5       // Lõi tính toán rủi ro (Cao nhất)
#define TASK_PRIO_SENSOR            4       // Thu thập dữ liệu liên tục
#define TASK_PRIO_COMM              3       // Xử lý mạng V2V
#define TASK_PRIO_DISPLAY           2       // Vẽ UI (Thấp nhất)

/* ==========================================
 * 3. THÔNG SỐ THUẬT TOÁN ĐỘNG HỌC & MẠNG
 * ========================================== */
// Tần số lấy mẫu
#define SENSOR_READ_DELAY_MS        10      // Chu kỳ đọc MPU6050: 10ms (100Hz)

// Ngưỡng phát hiện hành vi bản thân (Layer 1 - MPU6050)
#define ACCEL_HARD_BRAKE            0.6f    // Gia tốc > 0.6G: Phanh gấp (Sự kiện)
#define ACCEL_CRASH_DETECTED        3.0f    // Gia tốc > 3.0G: Va chạm thực tế (Sự kiện khẩn)

// Ngưỡng dự báo va chạm (Layer 3 - TTC: Time To Collision)
#define TTC_WARNING_SEC             4.0f    // Còn <= 4 giây: Cảnh báo Vàng (Warning)
#define TTC_DANGER_SEC              2.0f    // Còn <= 2 giây: Cảnh báo Đỏ (Danger)
#define SAFE_DISTANCE_MIN_M         5.0f    // Khoảng cách an toàn tối thiểu (Bù trừ sai số GPS)

// Thông số suy hao tín hiệu ESP-NOW (Layer 2 - Mesh/Distance)
#define RSSI_REF_1M                 -40     // dBm đo được ở 1 mét
#define RSSI_PATH_LOSS_EXP          2.5f    // Hệ số suy hao môi trường đô thị
#define RSSI_FILTER_ALPHA           0.1f    // Hệ số lọc thông thấp IIR cho RSSI

#endif // APP_CONFIG_H 