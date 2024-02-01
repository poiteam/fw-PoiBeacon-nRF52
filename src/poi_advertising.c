#include "poi_advertising.h"
#include "poi_battery.h"
#include "poi_flash_storage.h"
#include "poi_encryption.h"
#include "poi_timer.h"
#include "poi_sdk_config.h"

uint8_t m_adv_handle  =  BLE_GAP_ADV_SET_HANDLE_NOT_SET;
ble_gap_adv_params_t  m_adv_params;
uint8_t m_enc_advdata[BLE_GAP_ADV_SET_DATA_SIZE_MAX];
uint8_t m_enc_srdata[BLE_GAP_ADV_SET_DATA_SIZE_MAX];

ble_gap_adv_data_t m_adv_data = {

    .adv_data = {

        .p_data = m_enc_advdata,
        .len    = BLE_GAP_ADV_SET_DATA_SIZE_MAX

    },
    .scan_rsp_data = {

        .p_data = m_enc_srdata,
        .len    = BLE_GAP_ADV_SET_DATA_SIZE_MAX

    }
};

void poi_advertising_init(poi_beacon_info_t a_poi_beacon_config){

    ret_code_t      err_code;
    ble_advdata_t   advdata, scanrsp;
    uint8_t flags = BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE;
    poi_beacon_t * p_poi_beacon               = get_poi_beacon(); 
#if MINOR_ENCRYPTION_STATUS 
    poi_encryption_init(a_poi_beacon_config.minor,a_poi_beacon_config.minor);
    encryption_timeout_timer_start();;
#endif    
   
    ble_advdata_manuf_data_t manuf_specific_data;
    manuf_specific_data.company_identifier    = POI_COMPANY_IDENTIFIER;
    manuf_specific_data.data.p_data           = (uint8_t *)&a_poi_beacon_config;
    manuf_specific_data.data.size             = POI_BEACON_INFO_LENGTH;

    memset(&advdata, 0, sizeof(advdata));
    advdata.name_type                         = BLE_ADVDATA_NO_NAME;
    advdata.flags                             = flags;
    advdata.p_manuf_specific_data             = &manuf_specific_data; 

    uint8_t telemetry_default_service_info[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    telemetry_default_service_info[0]         = POI_FIRMWARE_VERSION;
    telemetry_default_service_info[1]         = p_poi_beacon->battery_percentage;
    telemetry_default_service_info[6]         = (a_poi_beacon_config.advertising_interval & 0xFF00) >> 8;
    telemetry_default_service_info[7]         = (a_poi_beacon_config.advertising_interval & 0x00FF);

    ble_advdata_service_data_t service_data;
    service_data.service_uuid                 = 0x180A;
    service_data.data.p_data                  = telemetry_default_service_info;
    service_data.data.size                    = 0x08;

    memset(&scanrsp, 0, sizeof(scanrsp));
    scanrsp.name_type                         = BLE_ADVDATA_FULL_NAME;
    scanrsp.p_tx_power_level                  = &a_poi_beacon_config.tx_power;
    scanrsp.p_service_data_array              = &service_data;
    scanrsp.service_data_count                = 1;

    memset(&m_adv_params, 0, sizeof(m_adv_params));
    m_adv_params.primary_phy                  = BLE_GAP_PHY_1MBPS;
    m_adv_params.properties.type              = BLE_GAP_ADV_TYPE_CONNECTABLE_SCANNABLE_UNDIRECTED;
    m_adv_params.p_peer_addr                  = NULL;
    m_adv_params.filter_policy                = BLE_GAP_ADV_FP_ANY;
    m_adv_params.interval                     = NON_CONNECTABLE_ADV_INTERVAL(a_poi_beacon_config.advertising_interval);
    m_adv_params.duration                     = 0;

    err_code                                  = ble_advdata_encode(&advdata, m_adv_data.adv_data.p_data, &m_adv_data.adv_data.len);
    APP_ERROR_CHECK(err_code);

    err_code                                  = ble_advdata_encode(&scanrsp, m_adv_data.scan_rsp_data.p_data, &m_adv_data.scan_rsp_data.len);
    APP_ERROR_CHECK(err_code);

    err_code                                  = sd_ble_gap_adv_set_configure(&m_adv_handle, &m_adv_data, &m_adv_params);
    if (err_code != NRF_SUCCESS) 
    {
       
        poi_flash_erase_beacon_info();
        nrf_delay_ms(200);
        NVIC_SystemReset();

    }
}

void poi_advertising_start(){

    ret_code_t err_code                     = sd_ble_gap_adv_start(m_adv_handle, BLE_ADV_MODE_DIRECTED_HIGH_DUTY);
    APP_ERROR_CHECK(err_code);

}

void poi_advertising_stop(void)
{
    ret_code_t err_code = sd_ble_gap_adv_stop(m_adv_handle);
    APP_ERROR_CHECK(err_code);

}