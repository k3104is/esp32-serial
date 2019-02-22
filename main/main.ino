#include "private.h"

void setup() {
  TIM_vdInit();
  LED_vdInit();
  BT_vdInit();
}

void loop() {
  LED_vdMain();
  BT_vdMain();
}