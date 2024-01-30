#include "poi_gatt_init.h"

void poi_gatt_init(){

    nrf_ble_gatt_t * p_m_gatt = get_m_gatt();
    ret_code_t err_code = nrf_ble_gatt_init(p_m_gatt, NULL);
    APP_ERROR_CHECK(err_code);

}