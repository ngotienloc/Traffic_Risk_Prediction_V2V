#ifndef GPS_TYPES_H
#define GPS_TYPES_H
#include <stdint.h>
#include <stdbool.h>

typedef struct {
    float latitude;
    float longitude;
    float speed;
    float heading;
    uint32_t timestamp;
    uint8_t satellites;
    float hdop;
    bool is_valid;
} GPS_Data_t;

#endif