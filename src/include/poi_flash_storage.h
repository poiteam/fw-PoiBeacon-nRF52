#ifndef POI_FLASH_STORAGE_H__
#define POI_FLASH_STORAGE_H__

#include <string.h>
#include "app_error.h"
#include "ble.h"
#include "ble_gatts.h"
#include "poi_beacon.h"


void poi_flash_write_beacon_info(uint8_t * p_uuid, uint8_t * p_beacon_info);
void poi_flash_erase_beacon_info();
uint32_t poi_flash_read_beacon_info(uint8_t * p_uuid, uint8_t * p_beacon_info);
void poi_flash_init();

#endif