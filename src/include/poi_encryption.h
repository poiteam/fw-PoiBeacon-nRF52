#ifndef POI_ENCRYPTION_H__
#define POI_ENCRYPTION_H__

#include <stdint.h>
#include "nrf_log.h"

void poi_encryption_init(const uint8_t * p_minor, uint8_t * p_enc_minor);

#endif