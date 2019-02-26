#include "private.h"
STATIC T_LED s_t_led;

void LED_vdInit(void){
    pinMode(P21, OUTPUT);   // set the LED pin mode
    return;
}

void led_vdPortBlink(uint8_t u1_port)
{
    static bool s_b_flg = false;
    s_b_flg = !s_b_flg;
    digitalWrite(u1_port, s_b_flg);
    return;
}

void LED_vdSwReqMain(bool b_sw)
{
    s_t_led.b_req_main = b_sw;
    return;
}

void LED_vdMain(void)
{
    if(s_t_led.b_req_main)
    {
        LED_vdSwReqMain(false);
        led_vdPortBlink(P21);
    }
    return;
}





