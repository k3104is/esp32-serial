#include "private.h"


void LED_vdInit(void){
  pinMode(P13, OUTPUT);      // set the LED pin mode
  return;
}

void LED_vdMain(void){
    digitalWrite(P13, TIM_vd1s());
  return;
}





