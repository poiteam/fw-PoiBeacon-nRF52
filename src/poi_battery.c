#include "poi_battery.h"
#include "poi_beacon.h"

#define ADC_REF_VOLTAGE_MV                600
#define ADC_RES_8_BIT                     255
#define ADC_PRE_SCALING_COMPENSATION      6 
#define VBAT_MIN_MV                       1700
#define VBAT_MAX_MV                       3100
#define PERCENTAGE                        100
#define ADC_RESULT_MV(ADC_VALUE)          ((((ADC_VALUE) * ADC_REF_VOLTAGE_MV) / ADC_RES_8_BIT) * ADC_PRE_SCALING_COMPENSATION)
#define ADC_RESULT_PERCENTAGE(VOLT_MV)    (((VOLT_MV - VBAT_MIN_MV) * PERCENTAGE) / (VBAT_MAX_MV - VBAT_MIN_MV))

void poi_battery_service_init(){

    ret_code_t err_code;
    nrf_drv_saadc_config_t saadc_config;
    saadc_config.low_power_mode      = true;
    saadc_config.resolution          = NRF_SAADC_RESOLUTION_8BIT;
    saadc_config.oversample          = NRF_SAADC_OVERSAMPLE_DISABLED;
    saadc_config.interrupt_priority  = APP_IRQ_PRIORITY_LOW;
    nrf_saadc_channel_config_t channel_config = NRF_DRV_SAADC_DEFAULT_CHANNEL_CONFIG_SE(NRF_SAADC_INPUT_VDD);

    err_code            = nrf_drv_saadc_channel_init(0, &channel_config);
    APP_ERROR_CHECK(err_code);

    err_code            = nrf_drv_saadc_init(&saadc_config, NULL);
    APP_ERROR_CHECK(err_code);
    
}

uint8_t get_battery_percentage(){

    ret_code_t err_code;
    nrf_saadc_value_t sample;
    uint16_t          battery_mv;

    err_code    = nrfx_saadc_sample_convert(0, &sample);
    APP_ERROR_CHECK(err_code);

    battery_mv            = ADC_RESULT_MV(sample);
    uint8_t battery_level = ADC_RESULT_PERCENTAGE(battery_mv);

    return battery_level;
    
}

void battery_service_uninit(){

    nrf_drv_saadc_uninit();

}

void poi_update_battery_value(){

    poi_battery_service_init();
    poi_beacon_t * p_poi_beacon         = get_poi_beacon();
    p_poi_beacon->battery_percentage    = get_battery_percentage();
    battery_service_uninit();

}