#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "app_config.h"
#include "model/shared_state.h"
#include "drivers/gps/gps.h"
#include "system/system_tasks.h"

static const char *TAG = "MAIN";

// ---------------------------------------------------------
// TASK TEST: Đọc data từ Shared State và in ra Serial
// ---------------------------------------------------------
static void Task_Debug_Print(void *pvParameters) {
    while (1) {
        if (xSemaphoreTake(Mutex_System_State, pdMS_TO_TICKS(50)) == pdTRUE) {
            
            GPS_Data_t gps = System_State.gps_data;
            xSemaphoreGive(Mutex_System_State);

            if (gps.is_valid) {
                ESP_LOGI("DEBUG", "✔ GPS Valid | Lat: %.6f | Lon: %.6f | Speed: %.2f m/s | Sats: %d | HDOP: %.1f",
                    gps.latitude,
                    gps.longitude,
                    gps.speed,
                    gps.satellites,
                    gps.hdop);
            } else {
                ESP_LOGW("DEBUG", "✘ GPS chưa có fix | Sats: %d | HDOP: %.1f",
                    gps.satellites,
                    gps.hdop);
            }

        } else {
            ESP_LOGW("DEBUG", "Không lấy được Mutex!");
        }

        vTaskDelay(pdMS_TO_TICKS(1000));  // In mỗi 1 giây
    }
}

void app_main(void) {
    ESP_LOGI(TAG, "==== KHỞI ĐỘNG HỆ THỐNG V2V ADAS ====");

    // BƯỚC 1: KHỞI TẠO SHARED STATE
    Shared_State_Init();
    ESP_LOGI(TAG, "Shared State & Mutex khởi tạo thành công.");

    // BƯỚC 2: KHỞI TẠO PHẦN CỨNG & ĐĂNG KÝ CALLBACK
    GPS_Init();
    GPS_RegisterCallback(Callback_Update_GPS_State);
    ESP_LOGI(TAG, "GPS khởi tạo và Callback đã đăng ký.");

    // BƯỚC 3: KHỞI CHẠY CÁC TASK
    ESP_LOGI(TAG, "Đang khởi chạy các Task...");

    xTaskCreatePinnedToCore(
        GPS_Task,
        "GPS_Task",
        TASK_STACK_SENSOR,
        NULL,
        TASK_PRIO_SENSOR,
        NULL,
        CORE_1
    );

    // Task debug: ưu tiên thấp nhất, chạy Core 0 để không tranh Core 1 với GPS
    xTaskCreatePinnedToCore(
        Task_Debug_Print,
        "Debug_Task",
        4096,
        NULL,
        1,              // Ưu tiên thấp nhất
        NULL,
        CORE_0
    );

    // Mở comment khi sẵn sàng:
    // xTaskCreatePinnedToCore(Task_Sensor_MPU, "MPU_Task",  TASK_STACK_SENSOR, NULL, TASK_PRIO_SENSOR, NULL, CORE_1);
    // xTaskCreatePinnedToCore(Task_Algo,       "Algo_Task", TASK_STACK_ALGO,   NULL, TASK_PRIO_ALGO,   NULL, CORE_1);

    ESP_LOGI(TAG, "==== KHỞI TẠO HOÀN TẤT. FREERTOS ĐANG CHẠY! ====");
}