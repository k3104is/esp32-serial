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

/* プロトタイプ宣言 */
void BT_WrtByChar(const char *str);
void bt_vdRcvStr(void);
void bt_vdSndStr(void);
void vdOnInit(void);
void onRcv(void);
void onSnd(void);
static const T_COM_STS *bt_ignore(const T_COM_STS *ptr_this);
static const T_COM_STS *bt_rcving(const T_COM_STS *ptr_this);
static const T_COM_STS *bt_st_rcv(const T_COM_STS *ptr_this);
static const T_COM_STS *bt_snding(const T_COM_STS *ptr_this);
static const T_COM_STS *bt_st_snd(const T_COM_STS *ptr_this);

/* インスタンス */
BluetoothSerial SerialBT;
static const T_COM_STS *ptr_com_sts;
const T_COM_STS c_t_com_sts_idle = {    /* アイドル状態 */
  bt_st_rcv,  /* 受信開始 → 受信状態 */
  bt_st_snd   /* 送信開始 → 送信状態 */
};
const T_COM_STS c_t_com_sts_rcv = {     /* 受信状態 */
  bt_rcving,  /* 受信中   → 受信状態 */
  bt_st_snd   /* 送信開始 → 送信状態 */
};
const T_COM_STS c_t_com_sts_snd = {     /* 送信状態 */
  bt_st_rcv,  /* 受信開始 → 受信状態 */
  bt_snding   /* 送信中   → 送信状態 */
};

/* ソース */
void vdOnInit(void){
  ptr_com_sts = &c_t_com_sts_idle;
};
void vdOnRcv(void){
  ptr_com_sts = ptr_com_sts->vdRcv(ptr_com_sts);
};
void vdOnSnd(void){
  ptr_com_sts = ptr_com_sts->vdSnd(ptr_com_sts);
};

static const T_COM_STS *bt_ignore(const T_COM_STS *ptr_this){
  return &c_t_com_sts_idle;
};
static const T_COM_STS *bt_rcving(const T_COM_STS *ptr_this){
  bt_vdRcvStr();
  return &c_t_com_sts_rcv;
};
static const T_COM_STS *bt_st_rcv(const T_COM_STS *ptr_this){
  BT_WrtByChar(MSG_OF_RCV);
  bt_vdRcvStr();
  return &c_t_com_sts_rcv;
};
static const T_COM_STS *bt_snding(const T_COM_STS *ptr_this){
  bt_vdSndStr();
  return &c_t_com_sts_snd;
}
static const T_COM_STS *bt_st_snd(const T_COM_STS *ptr_this){
  BT_WrtByChar(MSG_OF_SND);
  bt_vdSndStr();
  return &c_t_com_sts_snd;
}

void BT_WrtByChar(const char *str){
  while('\0' != *str){
    SerialBT.write(*str++);
  }
}

void bt_vdRcvStr(void){
  char str = Serial.read();
  SerialBT.write(str);
  Serial.write(str);
};
void bt_vdSndStr(void){
  char str = SerialBT.read();
  SerialBT.write(str);
  Serial.write(str);
};

/* Arduinoフォーマット */
void setup() {
  vdOnInit();
  Serial.begin(115200);
  SerialBT.begin("ESP32test"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");
}

void loop() {
  /* 受信 */
  if (Serial.available()) {
    vdOnRcv();
  }
  /* 送信 */
  if (SerialBT.available()) {
    vdOnSnd();
  }
  delay(20);
}