#include "poi_char_action.h"
#include "poi_timer.h"

void pass_control(poi_service_t * p_service, const uint8_t * p_data, poi_beacon_t * p_poi_beacon, poi_flags_t * p_poi_flags){

    if(memcmp(p_poi_beacon->beacon_password, p_data, 5) == 0){

        password_timeout_timer_stop();
        p_poi_flags->password_query = true;

    }
    else{

        p_poi_beacon->password_fail++;

    }

}

void change_uuid(poi_service_t * p_service, const uint8_t * p_data, poi_beacon_t * p_poi_beacon, poi_flags_t * p_poi_flags){

    memcpy(p_poi_beacon->uuid_buffer, p_data, sizeof(p_poi_beacon->uuid_buffer));
    p_poi_flags->uuid_changed = true;

}

void change_beacon_info(poi_service_t * p_service, const uint8_t * p_data, poi_beacon_t * p_poi_beacon, poi_flags_t * p_poi_flags){

    memcpy(p_poi_beacon->beacon_feature, p_data, sizeof(p_poi_beacon->beacon_feature));
    p_poi_flags->beacon_info_changed = true;

}

void char_write_action(poi_service_t * p_service, ble_evt_t const * p_ble_evt){

    uint32_t err_code;
    ble_gatts_evt_write_t const * p_evt_write = &p_ble_evt->evt.gatts_evt.params.write;
    poi_beacon_t * p_poi_beacon = get_poi_beacon();
    poi_flags_t  * p_poi_flags  = get_poi_flags();

    if(p_poi_beacon->password_fail > 2){

        err_code = sd_ble_gap_disconnect(p_ble_evt->evt.gattc_evt.conn_handle, BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
        APP_ERROR_CHECK(err_code);

    }

    else if((p_evt_write->handle == p_service->password_char_handles.value_handle) && (p_evt_write->len == 5)){

        pass_control(p_service, p_evt_write->data, p_poi_beacon, p_poi_flags);

    }

    else if((p_evt_write->handle == p_service->password_char_handles.value_handle) && (p_evt_write->len != 5)){

        p_poi_beacon->password_fail++;

    }

    else if((p_evt_write->handle == p_service->uuid_char_handle.value_handle) && (p_evt_write->len == 16) && (p_poi_flags->password_query == true)){

        change_uuid(p_service, p_evt_write->data, p_poi_beacon, p_poi_flags);

    }

    else if((p_evt_write->handle == p_service->beacon_info_handles.value_handle) && (p_evt_write->len == 8) && (p_poi_flags->password_query == true)){

        change_beacon_info(p_service, p_evt_write->data, p_poi_beacon, p_poi_flags);

    }

    else{

        p_poi_beacon->password_fail++;

    }
}