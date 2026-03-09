#include "system/system_tasks.h"
#include "model/shared_state.h"
#include "esp_log.h"
#include "drivers/mpu/mpu6050.h"

static const char *TAG = "SENSOR_TASK";

// ---------------- GPS CALLBACK ----------------
void Callback_Update_GPS_State(GPS_Data_t *new_data) {

    if (xSemaphoreTake(Mutex_System_State, portMAX_DELAY) == pdTRUE) {

        // cập nhật dữ liệu GPS
        System_State.gps_data = *new_data;

        xSemaphoreGive(Mutex_System_State);
    }
}


// ---------------- MPU SENSOR TASK ----------------
// đọc gia tốc MPU6050 định kỳ (~100 Hz)
void Task_Sensor_MPU(void *pvParameters) {

    ESP_LOGI(TAG, "MPU sensor task started");

    MPU_Data_t raw_imu;

    while (1) {

        // đọc accel từ MPU6050
        if (MPU6050_Read_Accel(&raw_imu) == true) {

            // ghi vào shared state
            if (xSemaphoreTake(Mutex_System_State, pdMS_TO_TICKS(10)) == pdTRUE) {

                System_State.mpu_data = raw_imu;

                xSemaphoreGive(Mutex_System_State);
            }
        }

        // chu kỳ 10 ms (~100 Hz)
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}