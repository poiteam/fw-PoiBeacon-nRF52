#include <string.h>
#include <stdint.h>

#include "app_error.h"
#include "nrf_pwr_mgmt.h"

#include "poi_log.h"
#include "poi_beacon.h"
#include "poi_softdevice.h"
#include "poi_gatt_init.h"
#include "poi_advertising.h"
#include "poi_service.h"
#include "poi_char_action.h"
#include "poi_timer.h"
#include "poi_flash_storage.h"
#include "poi_battery.h"
#include "poi_encryption.h"
#include "poi_sdk_config.h"

NRF_BLE_GATT_DEF(m_gatt);
NRF_BLE_QWR_DEF(m_qwr);

uint8_t m_beacon_info_default[POI_BEACON_INFO_LENGTH] = {

    POI_DEVICE_TYPE,
    POI_ADV_DATA_LENGTH,
    POI_BEACON_UUID,
    POI_MAJOR_VALUE,
    POI_MINOR_VALUE,
    POI_RSSI_VALUE

};

poi_beacon_t poi_beacon = {

    .password_fail      = 0,
    .beacon_password   = {0x41, 0x0D, 0x84, 0xFD, 0x4E},
    .poi_beacon_info    = {

        .advertising_interval  = 0x1C2,
        .tx_power              = -4,

    }
};

poi_flags_t poi_flags = {

    .password_query         = false,
    .connected              = false,
    .uuid_changed           = false,
    .beacon_info_changed    = false,
    .config_phase           = false,
    .password_timeout       = false,
    .encryption_timeout     = false,
};

poi_beacon_t * get_poi_beacon(){

    return &poi_beacon;

}

poi_flags_t * get_poi_flags(){

    return &poi_flags;

}

nrf_ble_gatt_t * get_m_gatt(){

    return &m_gatt;

}

nrf_ble_qwr_t * get_m_qwr(){

    return &m_qwr;

}


void idle_state_handle(){

    if(NRF_LOG_PROCESS() == false){

        nrf_pwr_mgmt_run();

    }

    if(poi_flags.config_phase){

        if(poi_flags.uuid_changed){

            memcpy(&poi_beacon.poi_beacon_info.uuid, poi_beacon.uuid_buffer, sizeof(poi_beacon.uuid_buffer));
            poi_flags.uuid_changed = false;

        }
        if(poi_flags.beacon_info_changed){

            memcpy(poi_beacon.poi_beacon_info.major, poi_beacon.beacon_feature, 4);
            poi_beacon.poi_beacon_info.advertising_interval = (((uint16_t)poi_beacon.beacon_feature[4] >> 8 | poi_beacon.beacon_feature[5]));
            poi_beacon.poi_beacon_info.tx_power             = poi_beacon.beacon_feature[6];
            poi_beacon.poi_beacon_info.rssi_value           = poi_beacon.beacon_feature[7];
            poi_flags.beacon_info_changed                   = false;

        }        
        poi_advertising_init(poi_beacon.poi_beacon_info);
        poi_flash_erase_beacon_info();
        poi_flash_write_beacon_info(poi_beacon.poi_beacon_info.uuid,poi_beacon.poi_beacon_info.major);
        poi_flags.config_phase = false;
        poi_advertising_start();
    }
    
    else if(poi_flags.password_timeout == true){

        poi_service_t * p_m_service = get_m_poilabs_service();
        ret_code_t  err_code        = sd_ble_gap_disconnect(p_m_service->conn_handle, BLE_HCI_CONN_INTERVAL_UNACCEPTABLE);
        APP_ERROR_CHECK(err_code);
        poi_flags.password_timeout  = false;

    }

    else if(poi_flags.encryption_timeout && !poi_flags.connected && !poi_flags.config_phase){

        poi_advertising_stop();
        poi_advertising_init(poi_beacon.poi_beacon_info);
        poi_advertising_start();
        poi_flags.encryption_timeout = false;

    }
}

void poi_power_mgmt_init(){

    //Düşük güç tüketimi ve enerji verimliliği sağlamak üzere sistemdeki güç yönetimi özelliklerini başlatı
    ret_code_t err_code = nrf_pwr_mgmt_init();
    APP_ERROR_CHECK(err_code);

}

int main(){
    

    poi_log_init();
    poi_timer_init();
    poi_power_mgmt_init();
    memcpy(&poi_beacon.poi_beacon_info, m_beacon_info_default, sizeof(m_beacon_info_default));
    poi_flash_init();
    poi_flash_read_beacon_info(poi_beacon.poi_beacon_info.uuid, poi_beacon.poi_beacon_info.major);
    poi_softdevice_init();
    poi_gap_params_init();
    poi_gatt_init();
    poi_service_init();
    poi_update_battery_value();
    poi_advertising_init(poi_beacon.poi_beacon_info);
    poi_conn_params_init();
    poi_advertising_start();

    for(;;){

        idle_state_handle();

    }

}