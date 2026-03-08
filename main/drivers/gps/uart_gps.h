#ifndef UART_GPS_H
#define UART_GPS_H
#include "driver/uart.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "app_config.h"
#include <string.h>

// Khai báo công khai Event Queue để Task có thể 'lắng nghe'
extern QueueHandle_t gps_uart_event_queue;
void UART_GPS_Init(void);

#endif