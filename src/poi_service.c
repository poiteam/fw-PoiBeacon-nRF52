#include "poi_beacon.h"
#include "poi_softdevice.h"
#include "poi_char_action.h"
#include "poi_service.h"

void ble_our_service_on_ble_evt(ble_evt_t const * p_ble_evt, void * p_context){

    poi_service_t * p_our_service = (poi_service_t *)p_context;
    switch(p_ble_evt->header.evt_id){
        case BLE_GAP_EVT_CONNECTED:
            p_our_service->conn_handle = p_ble_evt->evt.gap_evt.conn_handle;
            break;
        case BLE_GAP_EVT_DISCONNECTED:
            p_our_service->conn_handle = BLE_CONN_HANDLE_INVALID;
            break;
        case BLE_GATTS_EVT_WRITE:
            char_write_action((poi_service_t *)p_context, p_ble_evt);
            break;
        default:
            break;
    }

}

uint32_t password_char_add(poi_service_t * p_m_service){

    uint32_t        err_code;
    ble_uuid_t      char_uuid;
    ble_uuid128_t   base_uuid = OUR_BASE_UUID;
    char_uuid.uuid            = PASS_CHARACTERISTC_UUID;
    err_code                  = sd_ble_uuid_vs_add(&base_uuid, &char_uuid.type);
    APP_ERROR_CHECK(err_code);

    ble_gatts_char_md_t char_md;
    memset(&char_md, 0, sizeof(char_md));
	char_md.char_props.write = 1;

    ble_gatts_attr_md_t attr_md;
    memset(&attr_md, 0, sizeof(attr_md));  
    attr_md.vloc        = BLE_GATTS_VLOC_STACK;    
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm); 

    ble_gatts_attr_t    attr_char_value;
    memset(&attr_char_value, 0, sizeof(attr_char_value));
    attr_char_value.p_uuid      = &char_uuid;
	attr_char_value.p_attr_md   = &attr_md; 

	attr_char_value.max_len     = 5;
    attr_char_value.init_offs   = 0;
	attr_char_value.init_len    = 5;
	attr_char_value.p_value     = NULL;

    err_code = sd_ble_gatts_characteristic_add(p_m_service->service_handle, &char_md, &attr_char_value, &p_m_service->password_char_handles);
	APP_ERROR_CHECK(err_code);
    return NRF_SUCCESS;

}

uint32_t uuid_char_add(poi_service_t * p_m_service){

	uint32_t            err_code;
	ble_uuid_t          char_uuid;
	ble_uuid128_t       base_uuid = OUR_BASE_UUID;
	char_uuid.uuid                = UUID_CHARACTERISTC_UUID;
	err_code = sd_ble_uuid_vs_add(&base_uuid, &char_uuid.type);
	APP_ERROR_CHECK(err_code);

    ble_gatts_char_md_t char_md;
    memset(&char_md, 0, sizeof(char_md));
	char_md.char_props.read = 1;
	char_md.char_props.write = 1;

    ble_gatts_attr_md_t cccd_md;
    memset(&cccd_md, 0, sizeof(cccd_md));
	BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
	BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);
	cccd_md.vloc                = BLE_GATTS_VLOC_STACK;    
	char_md.p_cccd_md           = &cccd_md;

    ble_gatts_attr_md_t attr_md;
    memset(&attr_md, 0, sizeof(attr_md));  
	attr_md.vloc        = BLE_GATTS_VLOC_STACK;
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
	BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);

    ble_gatts_attr_t    attr_char_value;
    memset(&attr_char_value, 0, sizeof(attr_char_value));
	attr_char_value.p_uuid      = &char_uuid;
	attr_char_value.p_attr_md   = &attr_md;
	attr_char_value.max_len     = 16;
	attr_char_value.init_len    = 16; 
    poi_beacon_t * p_poi_beacon = get_poi_beacon();
	attr_char_value.p_value     = p_poi_beacon->poi_beacon_info.uuid;
	err_code = sd_ble_gatts_characteristic_add(p_m_service->service_handle, &char_md, &attr_char_value, &p_m_service->uuid_char_handle);
	APP_ERROR_CHECK(err_code);

    return NRF_SUCCESS;
    
}

uint32_t beacon_char_add(poi_service_t * p_m_service){

    uint32_t        err_code;
    ble_uuid_t      char_uuid;
    ble_uuid128_t   base_uuid = OUR_BASE_UUID;
    char_uuid.uuid            = BEACON_CHARACTERISTC_UUID;
    err_code                  = sd_ble_uuid_vs_add(&base_uuid, &char_uuid.type);
    APP_ERROR_CHECK(err_code);

    ble_gatts_char_md_t char_md;
    memset(&char_md, 0, sizeof(char_md));
	char_md.char_props.read = 1;
	char_md.char_props.write = 1;   

    ble_gatts_attr_md_t cccd_md;
    memset(&cccd_md, 0, sizeof(cccd_md));
	BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
	BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);
	cccd_md.vloc                = BLE_GATTS_VLOC_STACK;    
	char_md.p_cccd_md           = &cccd_md;

    ble_gatts_attr_md_t attr_md;
    memset(&attr_md, 0, sizeof(attr_md));  
	attr_md.vloc        = BLE_GATTS_VLOC_STACK;      
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
	BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);

    ble_gatts_attr_t    attr_char_value;
    memset(&attr_char_value, 0, sizeof(attr_char_value));
	attr_char_value.p_uuid      = &char_uuid;
	attr_char_value.p_attr_md   = &attr_md;
	attr_char_value.max_len     = 8;
	attr_char_value.init_len    = 8;
    poi_beacon_t * p_poi_beacon = get_poi_beacon();
	attr_char_value.p_value     = p_poi_beacon->poi_beacon_info.major;

    err_code = sd_ble_gatts_characteristic_add(p_m_service->service_handle, &char_md, &attr_char_value, &p_m_service->beacon_info_handles);
	APP_ERROR_CHECK(err_code);

    return NRF_SUCCESS;

}

void poilabs_service_init(){

    uint32_t err_code;
    poi_service_t * p_m_service = get_m_poilabs_service();

    ble_uuid128_t base_uuid = OUR_BASE_UUID;
    ble_uuid_t    service_uuid;
    service_uuid.uuid       = OUR_SERVICE_UUID;
    err_code                = sd_ble_uuid_vs_add(&base_uuid, &service_uuid.type);
    APP_ERROR_CHECK(err_code);

    p_m_service->conn_handle = BLE_CONN_HANDLE_INVALID;
    err_code                 = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY, &service_uuid, &p_m_service->service_handle);
    APP_ERROR_CHECK(err_code);

    password_char_add(p_m_service);
    uuid_char_add(p_m_service);
    beacon_char_add(p_m_service);

}

void nrf_qwr_error_handler(uint32_t nrf_error)
{

    APP_ERROR_HANDLER(nrf_error);

}

void poi_service_init(){

    ret_code_t err_code;
    nrf_ble_qwr_init_t qwr_init = {0};
    qwr_init.error_handler      = nrf_qwr_error_handler;

    nrf_ble_qwr_t * p_m_qwr = get_m_qwr();
    err_code = nrf_ble_qwr_init(p_m_qwr, &qwr_init);
    APP_ERROR_CHECK(err_code);   
    poilabs_service_init();

}