//This example code is in the Public Domain (or CC0 licensed, at your option.)
//By Evandro Copercini - 2018
//
//This example creates a bridge between Serial and Classical Bluetooth (SPP)
//and also demonstrate that SerialBT have the same functionalities of a normal Serial

#include "private.h"
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

/* プロトタイプ宣言 */
static void bt_WrtByChar(const char *str);
static char bt_vdRcvStr(void);
static char bt_vdSndStr(void);
static void bt_vdOnInit(void);
static void bt_onRcv(void);
static void onSnd(void);
static const T_COM_STS *bt_ignore(const T_COM_STS *ptr_this);
static const T_COM_STS *bt_rcving(const T_COM_STS *ptr_this);
static const T_COM_STS *bt_rcv_mark(const T_COM_STS *ptr_this);
static const T_COM_STS *bt_snding(const T_COM_STS *ptr_this);
static const T_COM_STS *bt_st_snd(const T_COM_STS *ptr_this);
static const T_COM_STS *bt_sp_rcv(const T_COM_STS *ptr_this);
static const T_COM_STS *bt_sp_snd(const T_COM_STS *ptr_this);

/* インスタンス */
BluetoothSerial SerialBT;
HardwareSerial  Uart2(2); // Uart2(TXD:IO17, RXD:IO16)

static const T_COM_STS *ptr_com_sts;
static const T_COM_STS c_t_com_sts_idle = {    /* アイドル状態 */
  bt_rcv_mark,
  bt_ignore,  
  bt_st_snd   
};
static const T_COM_STS c_t_com_sts_com = {     /* 受信/送信状態 */
  bt_ignore,
  bt_rcving,
  bt_snding
};
static const T_COM_STS c_t_com_sts_bcc = {     /* BCC受信状態 */
  bt_ignore,
  bt_sp_rcv,
  bt_sp_snd
};

/* ソース */
static void bt_vdOnInit(void){
  Uart2.end();
  ptr_com_sts = &c_t_com_sts_idle;
};
static void bt_vdOnMark(void){
  ptr_com_sts = ptr_com_sts->vdMark(ptr_com_sts);
};
static void bt_vdOnRcv(void){
  ptr_com_sts = ptr_com_sts->vdRcv(ptr_com_sts);
};
static void bt_vdOnSnd(void){
  ptr_com_sts = ptr_com_sts->vdSnd(ptr_com_sts);
};

static const T_COM_STS *bt_ignore(const T_COM_STS *ptr_this){
  return ptr_this;
};
static const T_COM_STS *bt_rcv_mark(const T_COM_STS *ptr_this){
  Uart2.begin(BR_UART);
  bt_WrtByChar(MSG_OF_RCV);
  return &c_t_com_sts_com;
};

static const T_COM_STS *bt_rcving(const T_COM_STS *ptr_this){
  if(0x03 == bt_vdRcvStr()) 
    return &c_t_com_sts_bcc;
  return &c_t_com_sts_com;
}
static const T_COM_STS *bt_sp_rcv(const T_COM_STS *ptr_this){
  bt_vdRcvStr();
  Uart2.end();
  return &c_t_com_sts_idle;
}
static const T_COM_STS *bt_st_snd(const T_COM_STS *ptr_this){
  bt_WrtByChar(MSG_OF_SND);
  delay(300);
  Uart2.begin(BR_UART);
  bt_vdSndStr();
  return &c_t_com_sts_com;
}
static const T_COM_STS *bt_snding(const T_COM_STS *ptr_this){
  if(0x03 == bt_vdSndStr()) 
    return &c_t_com_sts_bcc;
  return &c_t_com_sts_com;
}
static const T_COM_STS *bt_sp_snd(const T_COM_STS *ptr_this){
  bt_vdSndStr();
  Uart2.end();
  return &c_t_com_sts_idle;
}

static void bt_WrtByChar(const char *str){
  while('\0' != *str){
    SerialBT.write(*str++);
  }
}

static char bt_vdRcvStr(void){
  char str = Uart2.read();
  SerialBT.write(str);
  Serial.write(str);
  return str;
};
static char bt_vdSndStr(void){
  char str = SerialBT.read();
  SerialBT.write(str);
  Serial.write(str);
  Uart2.write(str);
  return str;
};

static bool bt_bTimeout(unsigned short s2_timeout){
  return (s2_timeout < TIM_s2ElapsedTime()) ? true : false;
}

void BT_vdInit(void){
  bt_vdOnInit();
  Serial.begin(BR_SERIAL);
  pinMode(INPUT_PORT, INPUT); 
  SerialBT.begin("ESP32test"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");
  return;
}

void BT_vdMain(void){
  /* 受信 */
  if(HIGH == digitalRead(INPUT_PORT)){
    bt_vdOnMark();
    TIM_vdSt();
  }
  if(Uart2.available()){
    bt_vdOnRcv();
    TIM_vdSt();
  }
  /* 送信 */
  if(SerialBT.available()){
    bt_vdOnSnd();
    TIM_vdSt();
  }
  /* タイムアウト判定 */
  if(bt_bTimeout(COM_TIMEOUT))
    bt_vdOnInit();
  delay(20);
  return;
}