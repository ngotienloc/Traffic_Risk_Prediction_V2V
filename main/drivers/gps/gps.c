#include "gps.h"
#include "uart_gps.h"
#include "esp_log.h"
#include "esp_timer.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

static const char *TAG = "GPS";

static GPS_Data_t gps_data;
static gps_callback_t data_ready_cb = NULL;

// Cache dữ liệu từ GGA
static uint8_t last_satellites = 0;
static float last_hdop = 99.9f;
static int last_fix_quality = 0;

static void parse_nmea(char *sentence)
{
    // bỏ qua chuỗi không hợp lệ
    if (strlen(sentence) < 10 || sentence[0] != '$') {
        return;
    }

    /* -------- Parse GGA -------- */
    if (strstr(sentence, "GGA") != NULL)
    {
        char time_str[16], lat_str[16], ns, lon_str[16], ew;
        int fix_quality = 0;
        uint8_t sats = 0;
        float hdop = 99.9f;

        int fields = sscanf(sentence,
                            "$%*[^,],%15[^,],%15[^,],%c,%15[^,],%c,%d,%hhd,%f",
                            time_str, lat_str, &ns, lon_str, &ew,
                            &fix_quality, &sats, &hdop);

        if (fields >= 8) {
            last_satellites  = sats;
            last_hdop        = hdop;
            last_fix_quality = fix_quality;
        }
    }

    /* -------- Parse RMC -------- */
    else if (strstr(sentence, "RMC") != NULL)
    {
        char time_str[16];
        char status;
        char lat_str[16], ns, lon_str[16], ew;
        float speed_knots = 0.0f;
        float course = 0.0f;
        char date_str[16];

        int fields = sscanf(sentence,
                            "$%*[^,],%15[^,],%c,%15[^,],%c,%15[^,],%c,%f,%f,%15[^,]",
                            time_str,
                            &status,
                            lat_str, &ns,
                            lon_str, &ew,
                            &speed_knots, &course,
                            date_str);

        if (fields >= 8 && status == 'A')
        {
            /* -------- Convert Latitude -------- */

            float latitude = 0.0f;

            if (strlen(lat_str) > 4) {
                char deg_buf[4] = {0};
                strncpy(deg_buf, lat_str, 2);

                float minutes = atof(lat_str + 2);
                latitude = atoi(deg_buf) + minutes / 60.0f;

                if (ns == 'S') latitude = -latitude;
            }

            /* -------- Convert Longitude -------- */

            float longitude = 0.0f;

            if (strlen(lon_str) > 5) {
                char deg_buf[5] = {0};
                strncpy(deg_buf, lon_str, 3);

                float minutes = atof(lon_str + 3);
                longitude = atoi(deg_buf) + minutes / 60.0f;

                if (ew == 'W') longitude = -longitude;
            }

            gps_data.latitude  = latitude;
            gps_data.longitude = longitude;
            gps_data.speed     = speed_knots * 0.514444f;   // knots -> m/s
            gps_data.heading   = course;

            /* -------- Parse GPS Time -------- */

            struct tm gps_time = {0};

            if (strlen(time_str) >= 6 && strlen(date_str) == 6)
            {
                char buf[3] = {0};

                // HH
                strncpy(buf, time_str, 2);
                gps_time.tm_hour = atoi(buf);

                // MM
                strncpy(buf, time_str + 2, 2);
                gps_time.tm_min = atoi(buf);

                // SS
                strncpy(buf, time_str + 4, 2);
                gps_time.tm_sec = atoi(buf);

                // DD
                strncpy(buf, date_str, 2);
                gps_time.tm_mday = atoi(buf);

                // MM
                strncpy(buf, date_str + 2, 2);
                gps_time.tm_mon = atoi(buf) - 1;

                // YY
                strncpy(buf, date_str + 4, 2);
                gps_time.tm_year = atoi(buf) + 100;   // since 1900 → 2000+

               gps_data.timestamp = ((uint64_t)mktime(&gps_time) * 1000);
            }

            gps_data.satellites = last_satellites;
            gps_data.hdop       = last_hdop;

            /* -------- Check Fix Valid -------- */

            bool valid = (status == 'A' &&
                          last_fix_quality >= 1 &&
                          last_satellites >= 4 &&
                          last_hdop < 5.0f &&
                          last_hdop > 0.0f);

            gps_data.is_valid = valid;

            if (valid && data_ready_cb != NULL) {
                data_ready_cb(&gps_data);
            }
        }
        else
        {
            gps_data.is_valid = false;
        }
    }
}

// ================= PUBLIC API =================

void GPS_Init(void) {
    memset(&gps_data, 0, sizeof(GPS_Data_t));
    gps_data.is_valid = false;

    UART_GPS_Init();
    ESP_LOGI(TAG, "GPS Logic Layer Initialized");
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
        ESP_LOGE(TAG, "GPS buffer alloc failed");
        vTaskDelete(NULL);
    }

    while (1) {

        if (xQueueReceive(gps_uart_event_queue, &event, portMAX_DELAY)) {

            if (event.type == UART_PATTERN_DET) {

                int pos = uart_pattern_pop_pos(GPS_UART_NUM);

                if (pos != -1) {
                    int read_len = uart_read_bytes(
                        GPS_UART_NUM,
                        dtmp,
                        pos + 1,
                        pdMS_TO_TICKS(100)
                    );

                    if (read_len > 0) {
                        dtmp[read_len] = '\0';
                        parse_nmea((char *)dtmp);
                    }
                }
            }

            else if (event.type == UART_FIFO_OVF ||
                     event.type == UART_BUFFER_FULL) {

                uart_flush_input(GPS_UART_NUM);
                xQueueReset(gps_uart_event_queue);

                ESP_LOGW(TAG, "UART overflow, buffer flushed");
            }
        }
    }

    free(dtmp);
    vTaskDelete(NULL);
}