#ifndef PACKET_H
#define PACKET_H

#include <stdint.h>
#include "system/device_info.h"
#include "model/shared_state.h"

// Định nghĩa các loại Event (Theo Algorithm Flowchart)
typedef enum {
    EVENT_NORMAL     = 0,
    EVENT_HARD_BRAKE = 1, // acc_long < -5 m/s2
    EVENT_CRASH      = 2  // acc_total > 2g
} EventType_t;

// Định nghĩa Risk Level (Theo State Machine Diagram)
typedef enum {
    RISK_NORMAL  = 0,
    RISK_WARNING = 1,
    RISK_DANGER  = 2,
    RISK_CRASH   = 3  // Trạng thái va chạm (Thêm theo mũi tên risk_level 3 trong sơ đồ)
} RiskLevel_t;

// Cấu trúc gói tin BSM Broadcast qua ESP-NOW (Chuẩn hóa kích thước để truyền nhanh)
typedef struct __attribute__((packed)) {
    uint32_t vehicle_id;    // ID định danh của xe (có thể dùng 4 byte cuối của MAC Address)
    uint8_t  event_type;    // EventType_t
    uint8_t  risk_level;    // RiskLevel_t
    float    latitude;
    float    longitude;
    float    speed;         // Vận tốc (m/s)
    float    heading;       // Hướng di chuyển (độ) 
    uint64_t timestamp;     // Thời gian lấy từ GPS (để đồng bộ)
} V2V_Packet_t;

// Gia toc khong can dua vao ban tin broadcast vi no duoc xu ly tai ban than xe va gui qua risk_level hay event_type

// Khởi tạo một gói tin mới với giá trị mặc định
void Packet_Init(V2V_Packet_t *packet);

// In thông tin gói tin ra console (dùng cho debug)
void Packet_Print(const V2V_Packet_t *packet);

bool Packet_Update(V2V_Packet_t *packet);
#endif