#ifndef POI_ENCRYPTION_H__
#define POI_ENCRYPTION_H__

#include <stdint.h>
#include "nrf_log.h"
#include "nrf_drv_rng.h"

void poi_encryption_init(const uint8_t * p_minor, uint8_t * p_enc_minor);
void poi_rng_init();

#endif