#ifndef GPS_H
#define GPS_H

#include "drivers/gps/gps_types.h"

// Định nghĩa con trỏ hàm Callback
typedef void (*gps_callback_t)(GPS_Data_t *data);

// Khởi tạo GPS (Gọi UART Init và setup UBX)
void GPS_Init(void);

// Đăng ký hàm Callback để nhận dữ liệu khi có tọa độ mới
void GPS_RegisterCallback(gps_callback_t cb);

// Task chạy ngầm để hứng Event Queue từ UART
void GPS_Task(void *pvParameters);

// (Tùy chọn) Lấy dữ liệu tức thời nếu không muốn dùng Callback
GPS_Data_t GPS_GetData(void);

//Phân tách chuỗi GPS
static void parse_nmea(char *sentence);
#endif // GPS_H