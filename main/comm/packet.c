#include "packet.h"
#include <stdio.h>
#include <string.h>

void Packet_Init(V2V_Packet_t *packet, uint32_t my_id) {
    if (packet == NULL) return;
    
    // Xóa sạch vùng nhớ
    memset(packet, 0, sizeof(V2V_Packet_t));
    
    // Gán ID mặc định
    packet->vehicle_id = my_id;
    packet->event_type = EVENT_NORMAL;
    packet->risk_level = RISK_NORMAL;
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