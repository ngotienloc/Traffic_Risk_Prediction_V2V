#include "model/shared_state.h"
#include "esp_log.h"

static const char *TAG = "SharedState";

// Định nghĩa thật, cấp phát RAM
System_State_t    System_State       = {0};
SemaphoreHandle_t Mutex_System_State = NULL;

void Shared_State_Init(void) {
    Mutex_System_State = xSemaphoreCreateMutex();
    if (Mutex_System_State == NULL) {
        ESP_LOGE(TAG, "Không tạo được Mutex!");
        esp_restart();
    }
}