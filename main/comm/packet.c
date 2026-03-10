#include "packet.h"
#include <stdio.h>
#include <string.h>

void Packet_Init(V2V_Packet_t *packet) {
    if (packet == NULL) return;
    
    // Xóa sạch vùng nhớ
    memset(packet, 0, sizeof(V2V_Packet_t));
    
    // Gán ID mặc định
    packet->vehicle_id = Device_GetVehicleID();
    packet->event_type = EVENT_NORMAL;
    packet->risk_level = RISK_NORMAL;
}
bool Packet_Update(V2V_Packet_t *packet) {
    if (packet == NULL) return false;

    // Xin chìa khóa mở két (chờ tối đa 10ms, không được thì bỏ qua để tránh treo task)
    if (xSemaphoreTake(Mutex_System_State, pdMS_TO_TICKS(10)) == pdTRUE) {
        
        // --- COPY TỪ KÉT SẮT SANG GÓI TIN ---
        
        // Từ GPS
        packet->latitude  = System_State.gps_data.latitude;
        packet->longitude = System_State.gps_data.longitude;
        packet->speed     = System_State.gps_data.speed;
        packet->heading   = System_State.gps_data.heading;
        packet->timestamp = System_State.gps_data.timestamp;
        
        // Từ thuật toán xử lý
        packet->risk_level = System_State.risk_level;
        
        // Từ MPU6050: Bạn có thể thêm logic ở đây. 
        // Ví dụ: Nếu gia tốc trục X âm quá mạnh (đang phanh cháy lốp), set event = PHANH_GẤP
        if (System_State.mpu_data.accel_x < -0.8f) {
            packet->event_type = 1; // Giả sử 1 là EVENT_HARD_BRAKING
        } else {
            packet->event_type = 0; // EVENT_NORMAL
        }

        // Trả lại chìa khóa ngay lập tức!
        xSemaphoreGive(Mutex_System_State);
        
        return true; // Báo cáo cập nhật thành công
    }
    
    return false; // Báo cáo thất bại (Két đang bận)
}

void Packet_Print(const V2V_Packet_t *packet) {
    if (packet == NULL) return;
    
    printf("--- V2V PACKET [ID: %lu] ---\n", (unsigned long)packet->vehicle_id);
    printf("  Event : %d\n", packet->event_type);
    printf("  Risk  : %d\n", packet->risk_level);
    printf("  Lat   : %.6f\n", packet->latitude);
    printf("  Lon   : %.6f\n", packet->longitude);
    printf("  Speed : %.2f m/s\n", packet->speed);
    printf("  Head  : %.2f deg\n", packet->heading);
    printf("  Time  : %lu\n", (unsigned long)packet->timestamp);
    printf("----------------------------\n");
}