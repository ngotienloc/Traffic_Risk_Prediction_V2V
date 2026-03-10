#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "app_config.h"
#include "model/shared_state.h"
#include "drivers/gps/gps.h"
#include "drivers/mpu/mpu6050.h"
#include "system/system_tasks.h"

static const char *TAG = "MAIN";

/* ---------------------------------------------------------
   TASK DEBUG: Đọc dữ liệu từ Shared State và in ra Serial
--------------------------------------------------------- */
static void Task_Debug_Print(void *pvParameters)
{
    while (1)
    {
        if (xSemaphoreTake(Mutex_System_State, pdMS_TO_TICKS(50)) == pdTRUE)
        {
            GPS_Data_t gps = System_State.gps_data;
            MPU_Data_t imu = System_State.mpu_data;

            xSemaphoreGive(Mutex_System_State);

            /* ---------- GPS ---------- */

            if (gps.is_valid)
            {
                ESP_LOGI("DEBUG",
                         "GPS ✔ | Lat: %.6f | Lon: %.6f | Speed: %.2f m/s | Sats: %d | HDOP: %.1f",
                         gps.latitude,
                         gps.longitude,
                         gps.speed,
                         gps.satellites,
                         gps.hdop);
            }
            else
            {
                ESP_LOGW("DEBUG",
                         "GPS ✘ chưa fix | Sats: %d | HDOP: %.1f",
                         gps.satellites,
                         gps.hdop);
            }

            /* ---------- IMU ---------- */

            ESP_LOGI("DEBUG",
                     "IMU | Ax: %.2f g | Ay: %.2f g | Az: %.2f g",
                     imu.accel_x,
                     imu.accel_y,
                     imu.accel_z);
        }
        else
        {
            ESP_LOGW("DEBUG", "Không lấy được Mutex!");
        }

        vTaskDelay(pdMS_TO_TICKS(1000));   // debug mỗi 1s
    }
}


/* ---------------------------------------------------------
   MAIN
--------------------------------------------------------- */
void app_main(void)
{
    ESP_LOGI(TAG, "==== KHỞI ĐỘNG HỆ THỐNG V2V ADAS ====");

    /* ---------- Shared State ---------- */

    Shared_State_Init();
    ESP_LOGI(TAG, "Shared State & Mutex khởi tạo thành công.");

    /* ---------- GPS ---------- */

    GPS_Init();
    MPU6050_Init();

    
    GPS_RegisterCallback(Callback_Update_GPS_State);

    ESP_LOGI(TAG, "GPS khởi tạo và Callback đã đăng ký.");

    /* ---------- START TASKS ---------- */

    ESP_LOGI(TAG, "Đang khởi chạy các Task...");

    /* GPS Task */

    xTaskCreatePinnedToCore(
        GPS_Task,
        "GPS_Task",
        TASK_STACK_SENSOR,
        NULL,
        TASK_PRIO_SENSOR,
        NULL,
        CORE_1
    );

    /* MPU Sensor Task */

    xTaskCreatePinnedToCore(
        Task_Sensor_MPU,
        "MPU_Task",
        TASK_STACK_SENSOR,
        NULL,
        TASK_PRIO_SENSOR,
        NULL,
        CORE_1
    );

    /* Debug Task */

    xTaskCreatePinnedToCore(
        Task_Debug_Print,
        "Debug_Task",
        4096,
        NULL,
        1,
        NULL,
        CORE_0
    );

    ESP_LOGI(TAG, "==== KHỞI TẠO HOÀN TẤT. FREERTOS ĐANG CHẠY! ====");
}