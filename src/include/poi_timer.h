#ifndef POI_TIMER_SERVICE_H__
#define POI_TIMER_SERVICE_H__

#include "app_error.h"
#include "app_timer.h"
#include "poi_beacon.h"

void poi_timer_init();
void password_timeout_timer_start(void);
void password_timeout_timer_stop(void);
void encryption_timeout_timer_start(void);
void encryption_timeout_timer_stop(void);

#endif