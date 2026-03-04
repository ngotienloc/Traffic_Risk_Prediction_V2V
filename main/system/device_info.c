#include "device_info.h"
#include "esp_system.h"
#include "esp_mac.h"
uint32_t Device_GetVehicleID(void)
{
    uint8_t mac[6];
    esp_efuse_mac_get_default(mac);

    uint32_t vehicle_id =
        ((uint32_t)mac[2] << 24) |
        ((uint32_t)mac[3] << 16) |
        ((uint32_t)mac[4] << 8)  |
         (uint32_t)mac[5];

    return vehicle_id;
}