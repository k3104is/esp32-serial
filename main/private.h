#ifndef _PRIVATE_H_
#define _PRIVATE_H_

#define _IN_THE_FIRM_
#define _REFACT_


#define P13         13
#define BR_SERIAL   115200
#define BR_UART     115200
#define PORT_NUM    21
#define HTML_PATH "/index.html"
#define BUFFER_SIZE 16384
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
    const struct T_COM_STS *(*const vdRcv)(const struct T_COM_STS *ptr_this);
    const struct T_COM_STS *(*const vdSnd)(const struct T_COM_STS *ptr_this2);
}T_COM_STS;

void BT_vdSetup(void);
void BT_vdLoop(void);

#endif