#ifndef _PRIVATE_H_
#define _PRIVATE_H_

#define _IN_THE_FIRM_
#define P13         13
#define BAUD_RATE   115200
#define PORT_NUM    21
#define HTML_PATH "/index.html"
#define BUFFER_SIZE 16384
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

#endif