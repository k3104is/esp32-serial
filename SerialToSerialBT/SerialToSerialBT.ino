//This example code is in the Public Domain (or CC0 licensed, at your option.)
//By Evandro Copercini - 2018
//
//This example creates a bridge between Serial and Classical Bluetooth (SPP)
//and also demonstrate that SerialBT have the same functionalities of a normal Serial

#include "BluetoothSerial.h"
#include "private.h"
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;



#ifdef _REFACT_
void initialize(void);
void onRcv(void);
void onSnd(void);

static const T_COM_STS *ptr_com_sts;

static const T_COM_STS *bt_ignore(const T_COM_STS *ptr_this);
static const T_COM_STS *bt_st_rcv(const T_COM_STS *ptr_this);
static const T_COM_STS *bt_st_snd(const T_COM_STS *ptr_this);

const T_COM_STS c_t_com_sts_idle = {    /* アイドル状態 */
  bt_st_rcv,  /* 受信開始 → 受信状態 */
  bt_st_snd   /* 送信開始 → 送信状態 */
};
const T_COM_STS c_t_com_sts_rcving = {  /* 受信状態 */
  bt_ignore,  /* 受信開始 → 無視 */
  bt_st_snd   /* 送信開始 → 送信状態 */
};

const T_COM_STS c_t_com_sts_snding = {  /* 送信状態 */
  bt_st_rcv,  /* 受信開始 → 受信状態 */
  bt_ignore   /* 送信開始 → 無視 */
};
void initialize(void){
  ptr_com_sts = &c_t_com_sts_idle;
};
void vdOnRcv(void){
  ptr_com_sts = ptr_com_sts->vdRcv(ptr_com_sts);
};
void vdOnSnd(void){
  ptr_com_sts = ptr_com_sts->vdSnd(ptr_com_sts);
};
#endif


void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32test"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");
}

void BT_WrtByChar(const char *str)
{
  while('\0' != *str){
    SerialBT.write(*str++);
  }
}

void loop() {
  static unsigned char com_sts = eNONE;
  char str = 0;
  /* 受信 */
  if (Serial.available()) {
    if(eRECEIVING != com_sts){
      com_sts = eRECEIVING;
      BT_WrtByChar(MSG_OF_RCV);
    }
    str = Serial.read();
    SerialBT.write(str);
    Serial.write(str);
  }
  /* 送信 */
  if (SerialBT.available()) {
    if(eSENDING != com_sts){
      com_sts = eSENDING;
      BT_WrtByChar(MSG_OF_SND);
    }
    str = SerialBT.read();
    SerialBT.write(str);
    Serial.write(str);
  }
  delay(20);
}