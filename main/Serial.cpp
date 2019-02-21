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
static void bt_WrtByChar(const char *str);
static void bt_vdRcvStr(void);
static void bt_vdSndStr(void);
static void bt_vdOnInit(void);
static void bt_onRcv(void);
static void onSnd(void);
static const T_COM_STS *bt_ignore(const T_COM_STS *ptr_this);
static const T_COM_STS *bt_rcving(const T_COM_STS *ptr_this);
static const T_COM_STS *bt_st_rcv(const T_COM_STS *ptr_this);
static const T_COM_STS *bt_snding(const T_COM_STS *ptr_this);
static const T_COM_STS *bt_st_snd(const T_COM_STS *ptr_this);

/* インスタンス */
BluetoothSerial SerialBT;
HardwareSerial  Uart2(2); // Uart2(TXD:IO17, RXD:IO16)

static const T_COM_STS *ptr_com_sts;
static const T_COM_STS c_t_com_sts_idle = {    /* アイドル状態 */
  bt_st_rcv,  /* 受信開始 → 受信状態 */
  bt_st_snd   /* 送信開始 → 送信状態 */
};
static const T_COM_STS c_t_com_sts_rcv = {     /* 受信状態 */
  bt_rcving,  /* 受信中   → 受信状態 */
  bt_st_snd   /* 送信開始 → 送信状態 */
};
static const T_COM_STS c_t_com_sts_snd = {     /* 送信状態 */
  bt_st_rcv,  /* 受信開始 → 受信状態 */
  bt_snding   /* 送信中   → 送信状態 */
};

/* ソース */
static void bt_vdOnInit(void){
  ptr_com_sts = &c_t_com_sts_idle;
};
static void bt_vdOnRcv(void){
  ptr_com_sts = ptr_com_sts->vdRcv(ptr_com_sts);
};
static void bt_vdOnSnd(void){
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
  bt_WrtByChar(MSG_OF_RCV);
  bt_vdRcvStr();
  return &c_t_com_sts_rcv;
};
static const T_COM_STS *bt_snding(const T_COM_STS *ptr_this){
  bt_vdSndStr();
  return &c_t_com_sts_snd;
}
static const T_COM_STS *bt_st_snd(const T_COM_STS *ptr_this){
  bt_WrtByChar(MSG_OF_SND);
  bt_vdSndStr();
  return &c_t_com_sts_snd;
}

static void bt_WrtByChar(const char *str){
  while('\0' != *str){
    SerialBT.write(*str++);
  }
}

static void bt_vdRcvStr(void){
  char str = Uart2.read();
  SerialBT.write(str);
  Serial.write(str);
  Uart2.write(str);
};
static void bt_vdSndStr(void){
  char str = SerialBT.read();
  SerialBT.write(str);
  Serial.write(str);
  Uart2.write(str);
};

void BT_vdSetup(void){
  bt_vdOnInit();
  Serial.begin(BR_SERIAL);
  Uart2.begin(BR_UART);
  SerialBT.begin("ESP32test"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");
}

void BT_vdLoop(void){
  /* 受信 */
  if (Uart2.available()) {
    bt_vdOnRcv();
  }
  /* 送信 */
  if (SerialBT.available()) {
    bt_vdOnSnd();
  }
  delay(20);
}