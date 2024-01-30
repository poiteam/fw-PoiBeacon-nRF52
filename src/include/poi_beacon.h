#ifndef POI_BEACON_H__
#define POI_BEACON_H__

#include <stdint.h>

#define POI_DEVICE_NAME                            "nRF52832Test"
#define POI_COMPANY_IDENTIFIER                     0x004C
#define POI_FIRMWARE_VERSION                       0x05
#define POI_BEACON_INFO_LENGTH                     0x17
#define POI_DEVICE_TYPE                            0x02
#define POI_ADV_DATA_LENGTH                        0x15
#define POI_BEACON_UUID                            0x8A, 0xA1, 0x00, 0x00, \
                                                   0x0A, 0x46, 0x11, 0x5F, \
                                                   0xD9, 0x4E, 0x5A, 0x96, \
                                                   0x6A, 0x3D, 0xDB, 0xB7
#define POI_MAJOR_VALUE                            0x00, 0x02
#define POI_MINOR_VALUE                            0x00, 0x08
#define POI_RSSI_VALUE                             0xBD
#define NON_CONNECTABLE_ADV_INTERVAL(INTERVAL)     MSEC_TO_UNITS(INTERVAL, UNIT_0_625_MS)

typedef struct {

    uint8_t     device_type;
    uint8_t     adv_data_len;
    uint8_t     uuid[16];
    uint8_t     major[2];
    uint8_t     minor[2];
    uint8_t     rssi_value;
    int8_t      tx_power;
    uint16_t    advertising_interval;

}poi_beacon_info_t;

typedef struct {

    uint8_t password_fail;
    uint8_t beacon_password[5];
    uint8_t uuid_buffer[16];
    uint8_t beacon_feature[8];
    uint8_t battery_percentage;
    poi_beacon_info_t poi_beacon_info;

}poi_beacon_t;

typedef struct {

    volatile uint8_t password_query:1;
    volatile uint8_t connected:1;
    volatile uint8_t uuid_changed:1;
    volatile uint8_t beacon_info_changed:1;
    volatile uint8_t config_phase:1;
    volatile uint8_t password_timeout:1;
    volatile uint8_t encryption_timeout:1;


}poi_flags_t;

poi_flags_t  * get_poi_flags();
poi_beacon_t * get_poi_beacon();

#endif

