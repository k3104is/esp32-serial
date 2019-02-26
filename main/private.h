#ifndef _PRIVATE_H_
#define _PRIVATE_H_

#include <stdint.h>
#include <stdbool.h>
#include <esp32-hal-timer.h>
#include <esp32-hal-gpio.h>
#include <U8g2lib.h>
#include "standard.h"
#include "BluetoothSerial.h"



#define _IN_THE_FIRM_
#define _REFACT_
#define STATIC 

typedef enum
{
    P0 = 0, P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11, P12, P13, P14, P15, P16, P17, P18, P19, P20, P21, P22, P23, P24, P25, P26, P27, P28, P29, P30, P31, P32, P33, P34, P35, P36, P37, P38, P39,
}PORT_NO;
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
    const struct T_COM_STS *(*const vdTo)(const struct T_COM_STS *ptr_this3);
}T_COM_STS;

typedef struct T_BT{
    UCHAR   u1_wait_1ms_pef_cnt;
    UCHAR   u1_timeout_500ms_per_cnt;
    uint8_t u1_sts;
}T_BT;

typedef struct T_TIM{
    bool           b_1ms;
    bool           b_500ms;
    bool           b_10ms;
    bool           b_1s;
    bool           flg;
    unsigned short st_time;
    unsigned short now_time;
}T_TIM;

typedef struct T_LED{
    bool            b_req_main;
}T_LED;
typedef struct T_OLED{
    bool            b_req_main;
}T_OLED;

void BT_vdInit(void);
void BT_vdMain(void);
void TIM_vdInit(void);
void TIM_vdMain(void);
unsigned short TIM_s2ElapsedTime(void);
void TIM_vdSt(void);
void TIM_vdSp(void);
bool TIM_vd1s(void);
void LED_vdInit(void);
void LED_vdMain(void);
void OLED_vdInit(void);
void OLED_vdMain(void);
void led_vdPortBlink(uint8_t u1_port);
void led_vdPortBlink(uint8_t u1_port);
void BT_vdDecrementWaitTime(void);
void MNG_vdProcessOf1ms(void);
UCHAR BT_u1Wait20msCnt(void);
uint16_t TIM_u2NowTime(void);
uint8_t BT_u1Sts(void);
bool TIM_vRtnFlgOf500ms(void);
void OLED_vdSwReqMain(bool b_sw);
void MNG_vdInit(void);
bool TIM_vRtnFlgOf1ms(void);
void BT_vdDecrementTimeout(void);
void MNG_vdMain(void);
void LED_vdSwReqMain(bool b_sw);
UCHAR BT_u1Timeout500msCnt(void);
UCHAR BT_u1Sts(void);
#endif