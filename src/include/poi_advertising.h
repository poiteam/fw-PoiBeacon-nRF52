#ifndef POI_ADVERTISING_H__
#define POI_ADVERTISING_H__

#include <stdint.h>
#include <string.h>

#include "app_error.h"
#include "ble_advdata.h"
#include "ble_advertising.h"
#include "ble.h"
#include "ble_conn_state.h"
#include "ble_srv_common.h"
#include "nrf_delay.h"

#include "poi_beacon.h"

void poi_advertising_init(poi_beacon_info_t a_poi_beacon_config);
void poi_advertising_start();
void poi_advertising_stop();

#endif