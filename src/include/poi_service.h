#ifndef POI_SERVICE_H__
#define POI_SERVICE_H__

#include <stdint.h>
#include "ble.h"
#include "ble_srv_common.h"
#include "nrf_ble_qwr.h"
#include "app_error.h"

#define OUR_BASE_UUID              {{0xB7, 0xDB, 0x3d, 0x6A, 0x96, 0x5a, 0x4e, 0xd9, 0x5f, 0x11, 0x46, 0x0a, 0x00, 0x00, 0xa1, 0x8a}} 
#define OUR_SERVICE_UUID           0x1000 

#define PASS_CHARACTERISTC_UUID     0x1001 
#define UUID_CHARACTERISTC_UUID     0x1004 
#define BEACON_CHARACTERISTC_UUID   0x1005 

typedef struct{

    uint16_t conn_handle;
    uint16_t service_handle;

    ble_gatts_char_handles_t password_char_handles;
    ble_gatts_char_handles_t uuid_char_handle;
    ble_gatts_char_handles_t beacon_info_handles;

}poi_service_t;

void ble_our_service_on_ble_evt(ble_evt_t const * p_ble_evt, void * p_context);

nrf_ble_qwr_t * get_m_qwr();

void poi_service_init();

#endif