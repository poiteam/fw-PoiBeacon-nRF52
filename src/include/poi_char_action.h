#ifndef POI_CHAR_ACTION_H__
#define POI_CHAR_ACTION_H__

#include <stdint.h>
#include <string.h>

#include "app_error.h"
#include "ble.h"
#include "ble_gatts.h"
#include "poi_beacon.h"
#include "poi_service.h"

void char_write_action(poi_service_t * p_service, ble_evt_t const * p_ble_evt);

#endif