#include "poi_encryption.h"
#include "poi_sdk_config.h"

#if MINOR_ENCRYPTION_STATUS

void random_vector_generate(uint8_t * p_buff, uint8_t size){

    uint32_t err_code;
    uint8_t  available;

    nrf_drv_rng_bytes_available(&available);
    uint8_t length = MIN(size, available);
    err_code = nrf_drv_rng_rand(p_buff, length);
    APP_ERROR_CHECK(err_code);

}

void poi_rng_init(){

    uint32_t err_code = nrf_drv_rng_init(NULL);
    APP_ERROR_CHECK(err_code);

}

void poi_encryption_init(const uint8_t * p_minor, uint8_t * p_enc_minor){

    uint8_t random_number;
    random_vector_generate(&random_number, 1);  
    
    memcpy(p_enc_minor, p_minor, 2);
    uint32_t order = 0;
	if(p_enc_minor[1] % 2 == 0){
				
        p_enc_minor[0] = ((p_enc_minor[0] & 2)<<4) | ((p_enc_minor[0] & 1)<<3)|((p_enc_minor[1] & 128)>>5) |((p_enc_minor[1] & 64)>>6);
		p_enc_minor[1] = ((p_enc_minor[1] & 48)<<2) |(p_enc_minor[1] & 15);
		
    }else {
				
        p_enc_minor[0] = ((p_enc_minor[0] & 2)<<6) | (((p_enc_minor[0] & 1)<<5)) |(((p_enc_minor[1] & 128)>>4)) |(((p_enc_minor[1] & 64)>>6));
		p_enc_minor[1] = ((p_enc_minor[1] & 56)<<1) | (p_enc_minor[1] & 7) ;

	}

	if(p_enc_minor[1] % 2 == 0){
			
        order = random_number % 63;
		order = (((order& 48)<<2)|((order & 8)<<1)|((order & 4)>>1));
		p_enc_minor[0] = p_enc_minor[0] & 45 ;
		p_enc_minor[0] = p_enc_minor[0] | order;
		order = random_number % 63;
		order = (order & 3)<<4;
		p_enc_minor[1] = p_enc_minor[1] & 207 ;
		p_enc_minor[1] = p_enc_minor[1] | order;
			
	}
    
    else{

		order = random_number % 63;
		order = ((order& 32)<<1)|(order & 16)|((order & 12)>>1);
		p_enc_minor[0] = p_enc_minor[0] & 169 ;
		p_enc_minor[0] = p_enc_minor[0] | order;
		order = random_number % 63;
		order = ((order & 2)<<6)|((order & 1)<<3);
		p_enc_minor[1] = p_enc_minor[1] & 119 ;
		p_enc_minor[1] = p_enc_minor[1] | order;
	
	}
		
}
#endif

