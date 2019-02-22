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

static void bt_vdInfoOfEsp32(void){
  Serial.println("-----------------------------");
 
  uint64_t chipid;
  chipid=ESP.getEfuseMac();//The chip ID is essentially its MAC address(length: 6 bytes).
  Serial.printf("ESP32 Chip ID = %04X\r\n",(uint16_t)(chipid>>32));//print High 2 bytes
 
  Serial.printf("Chip Revision %d\r\n", ESP.getChipRevision());
  esp_chip_info_t chip_info;
  esp_chip_info(&chip_info);
  Serial.printf("Number of Core: %d\r\n", chip_info.cores);
  Serial.printf("CPU Frequency: %d MHz\r\n", ESP.getCpuFreqMHz());  
  Serial.printf("Flash Chip Size = %d byte\r\n", ESP.getFlashChipSize());
  Serial.printf("Flash Frequency = %d Hz\r\n", ESP.getFlashChipSpeed());
  Serial.printf("ESP-IDF version = %s\r\n", esp_get_idf_version());
  Serial.printf("Free Heap Size = %d\r\n", esp_get_free_heap_size());
  Serial.printf("System Free Heap Size = %d\r\n", system_get_free_heap_size());
  Serial.printf("Minimum Free Heap Size = %d\r\n", esp_get_minimum_free_heap_size());
 
  Serial.println();
 
  uint8_t mac0[6];
  esp_efuse_mac_get_default(mac0);
  Serial.printf("Default Mac Address = %02X:%02X:%02X:%02X:%02X:%02X\r\n", mac0[0], mac0[1], mac0[2], mac0[3], mac0[4], mac0[5]);
 
  uint8_t mac1[6];
  esp_efuse_read_mac(mac1);
  Serial.printf("EFuse Mac Address = %02X:%02X:%02X:%02X:%02X:%02X\r\n", mac1[0], mac1[1], mac1[2], mac1[3], mac1[4], mac1[5]);
 
  uint8_t mac2[6];
  system_efuse_read_mac(mac2);
  Serial.printf("System Mac Address = %02X:%02X:%02X:%02X:%02X:%02X\r\n", mac2[0], mac2[1], mac2[2], mac2[3], mac2[4], mac2[5]);
 
  uint8_t mac3[6];
  esp_read_mac(mac3, ESP_MAC_WIFI_STA);
  Serial.printf("[Wi-Fi Station] Mac Address = %02X:%02X:%02X:%02X:%02X:%02X\r\n", mac3[0], mac3[1], mac3[2], mac3[3], mac3[4], mac3[5]);
 
  uint8_t mac4[7];
  esp_read_mac(mac4, ESP_MAC_WIFI_SOFTAP);
  Serial.printf("[Wi-Fi SoftAP] Mac Address = %02X:%02X:%02X:%02X:%02X:%02X\r\n", mac4[0], mac4[1], mac4[2], mac4[3], mac4[4], mac4[5]);
 
  uint8_t mac5[6];
  esp_read_mac(mac5, ESP_MAC_BT);
  Serial.printf("[Bluetooth] Mac Address = %02X:%02X:%02X:%02X:%02X:%02X\r\n", mac5[0], mac5[1], mac5[2], mac5[3], mac5[4], mac5[5]);
 
  uint8_t mac6[6];
  esp_read_mac(mac6, ESP_MAC_ETH);
  Serial.printf("[Ethernet] Mac Address = %02X:%02X:%02X:%02X:%02X:%02X\r\n", mac6[0], mac6[1], mac6[2], mac6[3], mac6[4], mac6[5]);
}


void BT_vdInit(void){
  bt_vdOnInit();
  Serial.begin(BR_SERIAL);
  bt_vdInfoOfEsp32();
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