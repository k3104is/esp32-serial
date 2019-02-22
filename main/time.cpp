#include "private.h"

static T_TIM s_t_tim;
hw_timer_t * timer0 = NULL;

static void tim_vdIncOfSec(void){
  if(s_t_tim.flg)
    s_t_tim.now_time++;
  return;
}

bool TIM_vd1s(void){
  return s_t_tim.b_1s;
}
static void tim_vdIntervalTimer500ms(void){
  s_t_tim.b_1s = !s_t_tim.b_1s; /* 500[ms]周期に反転 */
  if(s_t_tim.b_1s)  /* 1s周期 */
    tim_vdIncOfSec();
  return;
}
void tim_vdInitTimer0(void)
{
  timer0 = timerBegin(0, 240, true);  /* タイマ設定 240分周→1[us] */
  timerAttachInterrupt(timer0, tim_vdIntervalTimer500ms, true);  /* 割込関数セット */
  timerAlarmWrite(timer0, 500000, true);  /* 割込タイミング 500000カウントで発火→500[ms] */
  timerAlarmEnable(timer0); /* タイマ開始 */
  return;
}

void TIM_vdInit(void){
  tim_vdInitTimer0();
}
void TIM_vdSt(void){
  s_t_tim.flg = true;
  s_t_tim.st_time = 0;
  s_t_tim.now_time = 0;
  return;
}

void TIM_vdSp(void){
  s_t_tim.flg = false;
  s_t_tim.st_time = 0;
  s_t_tim.now_time = 0;
  return;
}

unsigned short TIM_s2ElapsedTime(void){
  return s_t_tim.now_time - s_t_tim.st_time;
}
