#ifndef POI_SOFTDEVICE_H__
#define POI_SOFTDEVICE_H__

#include <string.h>
#include <stdint.h>

#include "app_error.h"
#include "nrf_sdh.h"
#include "nrf_sdh_ble.h"
#include "nrf_sdh_soc.h"
#include "nrf_ble_gatt.h"
#include "ble_srv_common.h"
#include "ble_advertising.h"
#include "ble_conn_params.h"
#include "ble_conn_state.h"
#include "ble.h"
#include "ble_gap.h"

#include "poi_service.h"

void poi_conn_params_init();
void poi_gap_params_init();
void poi_softdevice_init();
poi_service_t * get_m_poilabs_service();

#endif