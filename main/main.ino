#include "private.h"

void setup() {
  TIM_vdInit();
  MNG_vdInit();
  BT_vdInit();
  LED_vdInit();
  OLED_vdInit();
}

void loop() {
  TIM_vdMain();
  MNG_vdMain();
  BT_vdMain();
  LED_vdMain();
  OLED_vdMain();
}