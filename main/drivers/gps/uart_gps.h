#ifndef UART_GPS_H
#define UART_GPS_H

#include <stdbool.h>
#include "model/shared_state.h"

// Khởi tạo UART cho GPS
void UART_GPS_Init(void);

// Đọc và bóc tách dữ liệu NMEA trực tiếp từ buffer của UART
bool UART_GPS_Read_Data(GPS_Data_t *gps_data);

#endif // UART_GPS_H