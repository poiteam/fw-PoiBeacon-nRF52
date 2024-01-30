#include "poi_timer.h"

//Bir zaman ölçer tanımı oluşturmak için kullanılır ve genellikle uygulama içinde zamanlama işlemlerini yönetmek amacıyla kullanılır.
APP_TIMER_DEF(password_timeout_timer_id);
APP_TIMER_DEF(encryption_timeout_timer_id);

//Milisaniye cinsinden bir süre miktarını zaman ölçer zaman birimine dönüştürmek için kullanılır.
#define PASS_TIMEOUT         APP_TIMER_TICKS(5000)
#define ENCRYPTION_TIMEOUT   APP_TIMER_TICKS(60000000) 

//Bu işlev, bir zaman ölçer (timer) olayının (event) tetiklendiğinde çağrılan bir işlevi temsil eder.
void password_timeout_timer_handler(void * p_context) 
{
    poi_flags_t * p_poi_flags = get_poi_flags();
    p_poi_flags->password_timeout = true;
}

void password_timeout_timer_init(void) 
{

    //app_timer_create() fonksiyonunu kullanarak bir zaman ölçer oluşturur.
    ret_code_t err_code;
    err_code = app_timer_create(&password_timeout_timer_id, APP_TIMER_MODE_SINGLE_SHOT, password_timeout_timer_handler);
    APP_ERROR_CHECK(err_code);

}

void password_timeout_timer_start(void)
{  
    //app_timer_start() fonksiyonunu kullanarak zaman ölçer başlatma işlemini gerçekleştirir
    ret_code_t err_code;
    err_code = app_timer_start(password_timeout_timer_id, PASS_TIMEOUT, NULL); 
    APP_ERROR_CHECK(err_code);
}

void password_timeout_timer_stop(void)
{
    (void) app_timer_stop(password_timeout_timer_id);
}

void encryption_timeout_timer_handler(void * p_context) 
{

    poi_flags_t * p_poi_flags = get_poi_flags();
    p_poi_flags->encryption_timeout = true;

}

void encryption_timeout_timer_init(void)
{

    ret_code_t err_code;
    err_code = app_timer_create(&encryption_timeout_timer_id, APP_TIMER_MODE_SINGLE_SHOT, encryption_timeout_timer_handler);
    APP_ERROR_CHECK(err_code);

}

void encryption_timeout_timer_start(void)
{  
   
    ret_code_t err_code;
    err_code = app_timer_start(encryption_timeout_timer_id, ENCRYPTION_TIMEOUT, NULL); // starting the timer for meas interval it is include one allready defined timer and interval value for the timer!
    APP_ERROR_CHECK(err_code);

}

void encryption_timeout_timer_stop(void)
{
   
    (void) app_timer_stop(encryption_timeout_timer_id);

}

//Bu fonksiyon, uygulamanın zaman ölçer (timer) özelliklerini başlatan bir inşa fonksiyonudur
void poi_timer_init(void)
{

    ret_code_t err_code = app_timer_init();
    APP_ERROR_CHECK(err_code);
    password_timeout_timer_init();
    encryption_timeout_timer_init();
    
}