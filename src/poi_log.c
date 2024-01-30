#include "poi_log.h"

//Function for initializing the nrf log module.

void poi_log_init(){

    ret_code_t err_code = NRF_LOG_INIT(NULL);     // İlk parametre olarak bir yapı (config struct) alabilir, ancak burada NULL geçilmiş, yani varsayılan ayarlar kullanılmış.
    APP_ERROR_CHECK(err_code);
    NRF_LOG_DEFAULT_BACKENDS_INIT();              // NRF_LOG modülünün varsayılan log backend'lerini başlatır. NRF_LOG modülü, logları farklı çıkışlara yönlendirmek için backend'ler sunar.

}