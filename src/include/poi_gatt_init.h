#ifndef POI_GATT_INIT_H__
#define POI_GATT_INIT_H__

#include <stdint.h>
#include <string.h>

#include "app_error.h"
#include "nrf_ble_gatt.h"

nrf_ble_gatt_t * get_m_gatt();
void poi_gatt_init();

#endif