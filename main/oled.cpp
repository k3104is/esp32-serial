#include "private.h"
/* 16文字8行 */
/* HSPI */
const static uint8_t c_s_s1_oled_mosi   = 13; /* MOSI(Master Output Slave Input) */
const static uint8_t c_s_s1_oled_sclk   = 14; /* SCLK */
const static uint8_t c_s_s1_oled_cs1    = 15; /* Chip Select */
// const static uint8_t c_s_s1_oled_miso = 12; /* MISO(Master Input Slave Output) */
const static uint8_t c_s_s1_oled_dc     = 26; /* Data/Command */
const static uint8_t c_s_s1_oled_rst    = 27;    /* Reset */

/* インスタンス */
STATIC T_OLED s_t_oled;
U8X8_SSD1306_128X64_NONAME_4W_SW_SPI u8x8( 
    c_s_s1_oled_sclk,   /* clock */ 
    c_s_s1_oled_mosi,   /* data */ 
    c_s_s1_oled_cs1,    /* cs */ 
    c_s_s1_oled_dc,     /* dc */ 
    c_s_s1_oled_rst);   /* reset */

void OLED_vdInit(void)
{
    u8x8.begin();
    u8x8.setFont(u8x8_font_chroma48medium8_r);
    return;
}

void OLED_vdSwReqMain(bool b_sw)
{
    s_t_oled.b_req_main = b_sw;
    return;
}

void OLED_vdMain(void)
{
    char s1_str_buf[17];
    if(s_t_oled.b_req_main)
    {
        OLED_vdSwReqMain(false);
        u8x8.clear();
        u8x8.drawString(0, 0, "ESP32");

        sprintf(s1_str_buf, "wait cnt:%d", BT_u1Wait20msCnt());
        u8x8.drawString(0, 1, s1_str_buf);

        sprintf(s1_str_buf, "timeout:%d", BT_u1Timeout500msCnt());
        u8x8.drawString(0, 2, s1_str_buf);

        sprintf(s1_str_buf, "sts:%d", BT_u1Sts());
        u8x8.drawString(0, 3, s1_str_buf);
    }
    return;
}



