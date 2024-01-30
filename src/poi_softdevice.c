#include "poi_softdevice.h"
#include "poi_beacon.h"
#include "poi_service.h"
#include "poi_timer.h"
#include "poi_advertising.h"

#define POI_CONN_CFG_TAG             1 
#define MAX_CONN_PARAMS_UPDATE_COUNT 3

poi_service_t m_poilabs;
uint16_t m_conn_handle = BLE_CONN_HANDLE_INVALID; 

#define CONN_SUP_TIMEOUT  MSEC_TO_UNITS(4000, UNIT_10_MS)
#define MAX_CONN_INTERVAL MSEC_TO_UNITS(200, UNIT_1_25_MS)
#define MIN_CONN_INTERVAL MSEC_TO_UNITS(100, UNIT_1_25_MS)
#define SLAVE_LATENCY     0
#define FIRST_CONN_PARAMS_UPDATE_DELAY  APP_TIMER_TICKS(5000)                   
#define NEXT_CONN_PARAMS_UPDATE_DELAY   APP_TIMER_TICKS(30000)

void on_conn_params_evt(ble_conn_params_evt_t * p_evt)
{
    ret_code_t err_code;

    if (p_evt->evt_type == BLE_CONN_PARAMS_EVT_FAILED)
    {
        err_code = sd_ble_gap_disconnect(m_conn_handle, BLE_HCI_CONN_INTERVAL_UNACCEPTABLE);
        APP_ERROR_CHECK(err_code);
    }
}

void conn_params_error_handler(uint32_t nrf_error)
{
    APP_ERROR_HANDLER(nrf_error);
}

void poi_conn_params_init(){

    ret_code_t             err_code;
    ble_conn_params_init_t cp_init;
    memset(&cp_init, 0, sizeof(cp_init));
    cp_init.p_conn_params                  = NULL;
    cp_init.first_conn_params_update_delay = FIRST_CONN_PARAMS_UPDATE_DELAY;
    cp_init.next_conn_params_update_delay  = NEXT_CONN_PARAMS_UPDATE_DELAY;
    cp_init.max_conn_params_update_count   = MAX_CONN_PARAMS_UPDATE_COUNT;
    cp_init.start_on_notify_cccd_handle    = BLE_GATT_HANDLE_INVALID;
    cp_init.disconnect_on_fail             = false;
    cp_init.evt_handler                    = on_conn_params_evt;
    cp_init.error_handler                  = conn_params_error_handler;

    err_code = ble_conn_params_init(&cp_init);
    APP_ERROR_CHECK(err_code);
}

void poi_gap_params_init(){

    ret_code_t err_code;
    ble_gap_conn_params_t   gap_parameters;
    ble_gap_conn_sec_mode_t sec_mode;

    //Set sec_mode pointed to by ptr to require no protection, open link.
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&sec_mode);

    err_code = sd_ble_gap_device_name_set(&sec_mode, (const uint8_t *)POI_DEVICE_NAME, strlen(POI_DEVICE_NAME));
    APP_ERROR_CHECK(err_code);

    memset(&gap_parameters, 0, sizeof(gap_parameters));
    gap_parameters.conn_sup_timeout  = CONN_SUP_TIMEOUT;
    gap_parameters.max_conn_interval = MAX_CONN_INTERVAL;
    gap_parameters.min_conn_interval = MIN_CONN_INTERVAL;
    gap_parameters.slave_latency     = SLAVE_LATENCY;

    err_code = sd_ble_gap_ppcp_set(&gap_parameters);
    APP_ERROR_CHECK(err_code);
    
}

/**@brief Function for handling BLE events.
 
 p_ble_evt   Bluetooth stack event.
 p_context   Unused.

 */

void ble_evt_handler(ble_evt_t const * p_ble_evt, void * p_context){

    ret_code_t err_code = NRF_SUCCESS;
    poi_flags_t  * p_poi_flags  = get_poi_flags();
    poi_beacon_t * p_poi_beacon = get_poi_beacon();

    switch(p_ble_evt->header.evt_id){

        case BLE_GAP_EVT_DISCONNECTED:
            p_poi_beacon->password_fail = 0;
            p_poi_flags->password_query = false;
            p_poi_flags->connected      = false;
            password_timeout_timer_stop();
            if(p_poi_flags->uuid_changed || p_poi_flags->beacon_info_changed){

                p_poi_flags->config_phase = true;

            }
            else{

                poi_advertising_start();

            }

            break;

        case BLE_GAP_EVT_CONNECTED:
            password_timeout_timer_start();
            encryption_timeout_timer_stop();
            p_poi_flags->connected = true;
            m_conn_handle          = p_ble_evt->evt.gap_evt.conn_handle;
            err_code               = nrf_ble_qwr_conn_handle_assign(get_m_qwr(), m_conn_handle);
            APP_ERROR_CHECK(err_code);
            break;

        case BLE_GAP_EVT_PHY_UPDATE_REQUEST:
        {
            ble_gap_phys_t const phys = {

                .rx_phys = BLE_GAP_PHY_AUTO,
                .tx_phys = BLE_GAP_PHY_AUTO,

            };
            err_code = sd_ble_gap_phy_update(p_ble_evt->evt.gap_evt.conn_handle, &phys);
            APP_ERROR_CHECK(err_code);
        }break;

        case BLE_GATTC_EVT_TIMEOUT:
            err_code = sd_ble_gap_disconnect(p_ble_evt->evt.gattc_evt.conn_handle, BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
            APP_ERROR_CHECK(err_code);
            break;

        case BLE_GATTS_EVT_TIMEOUT:
            err_code = sd_ble_gap_disconnect(p_ble_evt->evt.gatts_evt.conn_handle, BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
            APP_ERROR_CHECK(err_code);
            break;
        
        default:
            break;

    }
}

void poi_softdevice_init(){

    ret_code_t err_code;
    //Function for requesting to enable the SoftDevice
    err_code           = nrf_sdh_enable_request();
    APP_ERROR_CHECK(err_code);

    //Set the default BLE stack configuration. 
    uint32_t ram_start = 0;
    err_code           = nrf_sdh_ble_default_cfg_set(POI_CONN_CFG_TAG, &ram_start);
    APP_ERROR_CHECK(err_code);

    //Function for configuring and enabling the BLE stack.
    err_code           = nrf_sdh_ble_enable(&ram_start);
    APP_ERROR_CHECK(err_code);  

    NRF_SDH_BLE_OBSERVER(m_ble_observer, MAX_CONN_PARAMS_UPDATE_COUNT, ble_evt_handler, NULL);
    NRF_SDH_BLE_OBSERVER(m_poilabs_service_observer, MAX_CONN_PARAMS_UPDATE_COUNT, ble_our_service_on_ble_evt, &m_poilabs);

}

poi_service_t * get_m_poilabs_service(){

    return &m_poilabs;

}