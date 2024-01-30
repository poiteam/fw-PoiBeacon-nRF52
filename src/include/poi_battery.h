#ifndef POI_BATTERY_H__
#define POI_BATTERY_H__

#include <stdint.h>
#include <string.h>
#include "nrf_drv_saadc.h"
#include "ble.h"
#include "ble_gatts.h"

void poi_update_battery_value();

#endif