#include "nrf_fstorage.h"
#include "app_error.h"
#include "nrf_soc.h"
#include "nrf_delay.h"
#include "nrf_fstorage.h"
#include "poi_flash_storage.h"

#ifdef SOFTDEVICE_PRESENT
#include "nrf_sdh.h"
#include "nrf_sdh_ble.h"
#include "nrf_fstorage_sd.h"
#else
#include "nrf_drv_clock.h"
#include "nrf_fstorage_nvmc.h"
#endif

#define POI_MEMORY_BLOCK_SIZE 16,
#if defined (BOARD_PCA10040)
    #define POI_MEMERY_BLOCK_START_ADDRES 0X3e000
    #define POI_MEMORY_BLOCK_END_ADDRES 0X3f000
#endif 
#if defined (BOARD_IGA_BEACON)
    #define POI_MEMERY_BLOCK_START_ADDRES 0X25000
    #define POI_MEMORY_BLOCK_END_ADDRES 0X26000
#endif
#if defined (BOARD_TRACINDO )
    #define POI_MEMERY_BLOCK_START_ADDRES 0X3e000
    #define POI_MEMORY_BLOCK_END_ADDRES 0X3f000
#endif 


uint8_t poi_flash_key[8] = {0xee,0x5f,0x86,0x26,0x31,0xd4,0xf5,0xda};

NRF_FSTORAGE_DEF(nrf_fstorage_t fstorage) =
{
    /* Set a handler for fstorage events. */
    .evt_handler = NULL,

    /* These below are the boundaries of the flash space assigned to this instance of fstorage.
     * You must set these manually, even at runtime, before nrf_fstorage_init() is called.
     * The function nrf5_flash_end_addr_get() can be used to retrieve the last address on the
     * last page of flash available to write data. */
    .start_addr = POI_MEMERY_BLOCK_START_ADDRES,
    .end_addr   = POI_MEMORY_BLOCK_END_ADDRES,
};

void wait_for_flash_ready(nrf_fstorage_t const * p_fstorage)
{
 
    while (nrf_fstorage_is_busy(p_fstorage))
    {
        #ifdef SOFTDEVICE_PRESENT
            (void) sd_app_evt_wait();
        #else
            __WFE();
        #endif
    }

}

/*

Bu fonksiyon, nRF5 SDK'nın flash depolama (fstorage) modülünü başlatan bir fonksiyondur. 
nRF5 SDK, dahili flash veya harici SD kart gibi farklı depolama cihazları üzerinde veri depolamak için bu tür bir modül sunar.

*/
void poi_flash_init(){

// Bu kısım, kullanılacak depolama API'sının seçimini yapar. Eğer bir SoftDevice (BLE yığını) mevcutsa, nrf_fstorage_sd kullanılır; aksi halde, nrf_fstorage_nvmc kullanılır.

    nrf_fstorage_api_t * p_fs_api;
#ifdef SOFTDEVICE_PRESENT
    p_fs_api = &nrf_fstorage_sd;
#else
    p_fs_api = &nrf_fstorage_nvmc;
#endif

    uint32_t rc = nrf_fstorage_init(&fstorage, p_fs_api, NULL);
    APP_ERROR_CHECK(rc);

}

void poi_flash_write_beacon_info(uint8_t * p_uuid, uint8_t * p_beacon_info){

    uint32_t rc;
    uint8_t memory_buffer[32];

    memcpy(memory_buffer,poi_flash_key, sizeof(poi_flash_key));
    memcpy(memory_buffer+sizeof(poi_flash_key),p_uuid,16);
    memcpy(memory_buffer+(sizeof(poi_flash_key)+16),p_beacon_info,8);

    rc = nrf_fstorage_write(&fstorage, POI_MEMERY_BLOCK_START_ADDRES, memory_buffer, sizeof(memory_buffer), NULL);
    APP_ERROR_CHECK(rc);
    wait_for_flash_ready(&fstorage);

}

uint32_t poi_flash_read_beacon_info(uint8_t * p_uuid, uint8_t * p_beacon_info){

    uint8_t  memory_buffer[32];
    uint32_t err_code;
    uint32_t rc = nrf_fstorage_read(&fstorage, POI_MEMERY_BLOCK_START_ADDRES, memory_buffer, sizeof(memory_buffer));
    APP_ERROR_CHECK(rc);

    if(memcmp(memory_buffer, poi_flash_key, sizeof(poi_flash_key)) == 0){

        memcpy(p_uuid, memory_buffer+sizeof(poi_flash_key), 16);
        memcpy(p_beacon_info, memory_buffer+sizeof(poi_flash_key)+16, 8);
        
    }
    else{

        err_code = 1;
        
    }

    return err_code;

}

void poi_flash_erase_beacon_info(){

    ret_code_t rc = nrf_fstorage_erase(&fstorage, POI_MEMERY_BLOCK_START_ADDRES, 1, NULL);
    APP_ERROR_CHECK(rc);
    wait_for_flash_ready(&fstorage);
    nrf_delay_ms(10);

}