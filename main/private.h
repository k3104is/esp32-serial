#ifndef _PRIVATE_H_
#define _PRIVATE_H_

#include <stdbool.h>
#include <esp32-hal-timer.h>
#include <esp32-hal-gpio.h>
#include "BluetoothSerial.h"

#define _IN_THE_FIRM_
#define _REFACT_


#define P13         13
#define INPUT_PORT  2
#define BR_SERIAL   115200
#define BR_UART     300
#define PORT_NUM    21
#define HTML_PATH "/index.html"
#define BUFFER_SIZE 16384
#define COM_TIMEOUT 10
#define MSG_OF_RCV "\r\n[Received Data]\r\n"
#define MSG_OF_SND "\r\n[Send data]\r\n"
/* 社外 */
#ifndef _IN_THE_FIRM_
    #define SSID      "SPWH_H32_E57283"
    #define PASSWORD  "t90jnhaet0aagbn"
    #define LOCAL_IP  "192.168.100.131"
/* 社内 */
#else
    #define SSID      "test_ap"
    #define PASSWORD  "akisuteno"
    #define LOCAL_IP  "192.168.137.131"
#endif

typedef struct T_COM_STS{
    const struct T_COM_STS *(*const vdMark)(const struct T_COM_STS *ptr_this);
    const struct T_COM_STS *(*const vdRcv)(const struct T_COM_STS *ptr_this);
    const struct T_COM_STS *(*const vdSnd)(const struct T_COM_STS *ptr_this2);
}T_COM_STS;

typedef struct T_TIM{
    bool           b_10ms;
    bool           b_1s;
    bool           flg;
    unsigned short st_time;
    unsigned short now_time;
}T_TIM;

void BT_vdInit(void);
void BT_vdMain(void);
void TIM_vdInit(void);
unsigned short TIM_s2ElapsedTime(void);
void TIM_vdSt(void);
void TIM_vdSp(void);
bool TIM_vd1s(void);
void LED_vdInit(void);
void LED_vdMain(void);
#endif