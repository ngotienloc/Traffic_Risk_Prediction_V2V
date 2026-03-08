#include "gps.h"
#include "uart_gps.h"
#include "esp_log.h"
#include "esp_timer.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>   // cho atoi, atof

static const char *TAG = "GPS";

static GPS_Data_t gps_data;
static gps_callback_t data_ready_cb = NULL;

// Lưu tạm từ GPGGA (cập nhật trước/sau GPRMC đều được, nhưng ta ưu tiên dùng giá trị mới nhất)
static uint8_t last_satellites = 0;
static float last_hdop = 99.9f;
static int last_fix_quality = 0;  // 0=invalid, 1=GPS, 2=DGPS,...

// ================= NMEA PARSER =================
static void parse_nmea(char *sentence) {
    // Bỏ qua câu quá ngắn hoặc không bắt đầu bằng $
    if (strlen(sentence) < 10 || sentence[0] != '$') {
        return;
    }

    // 1. Parse $GPGGA (hoặc $GNGGA) để lấy satellites, HDOP, fix quality
    if (strstr(sentence, "GGA") != NULL) {   // khớp GPGGA, GNGGA,...
        char time_str[16], lat_str[16], ns, lon_str[16], ew;
        int fix_quality = 0;
        uint8_t sats = 0;
        float hdop = 99.9f;

        // Format chuẩn GGA: $xxGGA,time,lat,NS,lon,EW,quality,sats,hdop,...
        int fields = sscanf(sentence, "$%*[^,],%15[^,],%15[^,],%c,%15[^,],%c,%d,%hhd,%f",
                            time_str, lat_str, &ns, lon_str, &ew, &fix_quality, &sats, &hdop);

        if (fields >= 8) {
            last_satellites   = sats;
            last_hdop         = hdop;
            last_fix_quality  = fix_quality;
            // ESP_LOGD(TAG, "GGA: sats=%d, hdop=%.1f, quality=%d", sats, hdop, fix_quality);
        }
    }

    // 2. Parse $GPRMC (hoặc $GNRMC) để lấy vị trí, tốc độ, hướng
    else if (strstr(sentence, "RMC") != NULL) {
        char status;
        char lat_str[16], ns, lon_str[16], ew;
        float speed_knots = 0.0f;
        float course = 0.0f;
        char date_str[16];

        // Format chuẩn RMC: $xxRMC,time,status,lat,NS,lon,EW,speed,course,date,...
        int fields = sscanf(sentence, "$%*[^,],%*[^,],%c,%15[^,],%c,%15[^,],%c,%f,%f,%15[^,]",
                            &status, lat_str, &ns, lon_str, &ew, &speed_knots, &course, date_str);

        if (fields >= 7 && status == 'A') {
            // ================= Convert latitude =================
            float latitude = 0.0f;
            if (strlen(lat_str) > 4) {
                char deg_buf[4] = {0};
                strncpy(deg_buf, lat_str, 2);           // 2 chữ số đầu = degrees
                float minutes = atof(lat_str + 2);      // phần còn lại là mm.mmmm
                latitude = atoi(deg_buf) + (minutes / 60.0f);
                if (ns == 'S') latitude = -latitude;
            }

            // ================= Convert longitude =================
            float longitude = 0.0f;
            if (strlen(lon_str) > 5) {
                char deg_buf[5] = {0};
                strncpy(deg_buf, lon_str, 3);           // 3 chữ số đầu = degrees
                float minutes = atof(lon_str + 3);      // phần còn lại là mm.mmmm
                longitude = atoi(deg_buf) + (minutes / 60.0f);
                if (ew == 'W') longitude = -longitude;
            }

            // Cập nhật dữ liệu
            gps_data.latitude   = latitude;
            gps_data.longitude  = longitude;
            gps_data.speed      = speed_knots * 0.514444f;  // knots → m/s (chuẩn hơn 0.51444)
            gps_data.heading    = course;
            gps_data.timestamp  = (uint32_t)(esp_timer_get_time() / 1000ULL);  // ms since boot
            gps_data.satellites = last_satellites;
            gps_data.hdop       = last_hdop;

            // Tiêu chí valid: fix tốt + đủ vệ tinh + HDOP hợp lý
            bool valid = (status == 'A' &&
                          last_fix_quality >= 1 &&
                          last_satellites >= 4 &&
                          last_hdop < 5.0f &&
                          last_hdop > 0.0f);   //tránh hdop =0.0 giả

            gps_data.is_valid = valid;

            if (valid) {
                if (data_ready_cb != NULL) {
                    data_ready_cb(&gps_data);
                }
                // ESP_LOGI(TAG, "Valid fix: %.6f, %.6f, speed=%.1f m/s, sats=%d, hdop=%.1f",
                //          latitude, longitude, gps_data.speed, last_satellites, last_hdop);
            } else {
                // ESP_LOGW(TAG, "Invalid fix: quality=%d, sats=%d, hdop=%.1f",
                //          last_fix_quality, last_satellites, last_hdop);
            }
        } else {
            gps_data.is_valid = false;
        }
    }
}

// ================= PUBLIC FUNCTIONS =================
void GPS_Init(void) {
    memset(&gps_data, 0, sizeof(GPS_Data_t));
    gps_data.is_valid = false;
    
    UART_GPS_Init();
    ESP_LOGI(TAG, "GPS Logic Layer Initialized.");
}

void GPS_RegisterCallback(gps_callback_t cb) {
    data_ready_cb = cb;
}

GPS_Data_t GPS_GetData(void) {
    return gps_data;
}

// ================= RTOS TASK =================
void GPS_Task(void *pvParameters) {
    uart_event_t event;
    uint8_t *dtmp = (uint8_t *)malloc(GPS_BUF_SIZE + 1);
    if (dtmp == NULL) {
        ESP_LOGE(TAG, "Failed to allocate GPS buffer!");
        vTaskDelete(NULL);
    }

    while (1) {
        if (xQueueReceive(gps_uart_event_queue, &event, portMAX_DELAY)) {
            if (event.type == UART_PATTERN_DET) {
                int pos = uart_pattern_pop_pos(GPS_UART_NUM);
                if (pos != -1) {
                    int read_len = uart_read_bytes(GPS_UART_NUM, dtmp, pos + 1, pdMS_TO_TICKS(100));
                    if (read_len > 0) {
                        dtmp[read_len] = '\0';
                        parse_nmea((char *)dtmp);
                    }
                }
            }
            else if (event.type == UART_FIFO_OVF || event.type == UART_BUFFER_FULL) {
                uart_flush_input(GPS_UART_NUM);
                xQueueReset(gps_uart_event_queue);
                ESP_LOGW(TAG, "UART Buffer Overflow, flushed.");
            }
        }
    }

    free(dtmp);
    vTaskDelete(NULL);
}