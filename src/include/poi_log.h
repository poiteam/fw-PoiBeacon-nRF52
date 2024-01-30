#ifndef POI_LOG_H__
#define POI_LOG_H__

#include <stdint.h>
#include <string.h>

#include "app_error.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

void poi_log_init();

#endif