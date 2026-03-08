#include "uart_gps.h"
#include "app_config.h"

QueueHandle_t gps_uart_event_queue;

// Các lệnh UBX (Như bản trước)
static const uint8_t UBX_RATE_5HZ[] = {0xB5,0x62,0x06,0x08,0x06,0x00,0xC8,0x00,0x01,0x00,0x01,0x00,0xDE,0x6A};
static const uint8_t UBX_DISABLE_GSA[] = {0xB5,0x62,0x06,0x01,0x08,0x00,0xF0,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x31};
static const uint8_t UBX_DISABLE_GSV[] = {0xB5,0x62,0x06,0x01,0x08,0x00,0xF0,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x38};
static const uint8_t UBX_DISABLE_VTG[] = {0xB5,0x62,0x06,0x01,0x08,0x00,0xF0,0x05,0x00,0x00,0x00,0x00,0x00,0x00,0x04,0x46};

static void send_ubx(const uint8_t *cmd, uint16_t len) {
    uart_write_bytes(GPS_UART_NUM, (const char*)cmd, len);
    vTaskDelay(pdMS_TO_TICKS(100));
}

void UART_GPS_Init(void) {
    uart_config_t uart_config = {
        .baud_rate = GPS_BAUD_RATE,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };

    // Khởi tạo UART VỚI Event Queue (Kích thước hàng đợi = 20 sự kiện)
    uart_driver_install(GPS_UART_NUM, GPS_BUF_SIZE * 2, GPS_BUF_SIZE * 2, 20, &gps_uart_event_queue, 0);
    uart_param_config(GPS_UART_NUM, &uart_config);
    uart_set_pin(GPS_UART_NUM, GPS_TX_PIN, GPS_RX_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);

    // DẠY PHẦN CỨNG NHẬN DIỆN KÝ TỰ '\n' (Line Feed)
    uart_enable_pattern_det_baud_intr(GPS_UART_NUM, '\n', 1, 9, 0, 0);
    uart_pattern_queue_reset(GPS_UART_NUM, 20);

    vTaskDelay(pdMS_TO_TICKS(500));

    // Gửi lệnh UBX
    send_ubx(UBX_RATE_5HZ, sizeof(UBX_RATE_5HZ));
    send_ubx(UBX_DISABLE_GSA, sizeof(UBX_DISABLE_GSA));
    send_ubx(UBX_DISABLE_GSV, sizeof(UBX_DISABLE_GSV));
    send_ubx(UBX_DISABLE_VTG, sizeof(UBX_DISABLE_VTG));

    uart_flush(GPS_UART_NUM);
}