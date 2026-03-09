#ifndef UART_GPS_H
#define UART_GPS_H
#include "driver/uart.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "app_config.h"
#include <string.h>

extern QueueHandle_t gps_uart_event_queue;
void UART_GPS_Init(void);

#endif