#include "private.h"




STATIC T_TIM s_t_tim;
hw_timer_t * timer0 = NULL;
hw_timer_t * timer1 = NULL;

static void tim_vdIncOfSec(void)
{
    if(s_t_tim.flg)
        s_t_tim.now_time++;
    return;
}

static void tim_vdIntervalTimer500ms(void)
{
    s_t_tim.b_500ms = true;
    return;
}
static void tim_vdIntervalTimer1ms(void)
{
    s_t_tim.b_1ms = true;
    return;
}

bool TIM_vRtnFlgOf500ms(void)
{
    if(s_t_tim.b_500ms)
    {
        s_t_tim.b_500ms = false;
        return true;
    }
    return false;
}

bool TIM_vRtnFlgOf1ms(void)
{
    if(s_t_tim.b_1ms)
    {
        s_t_tim.b_1ms = false;
        return true;
    }
    return false;
}

void tim_vdInitTimer0(void)
{
    timer0 = timerBegin(0, 80, true);    /* タイマ設定 240分周→1[us] */
    timerAttachInterrupt(timer0, tim_vdIntervalTimer500ms, true);    /* 割込関数セット */
    timerAlarmWrite(timer0, 500000, true);    /* 割込タイミング 500000カウントで発火→500[ms] */
    timerAlarmEnable(timer0); /* タイマ開始 */
    return;
}
void tim_vdInitTimer1(void)
{
    timer1 = timerBegin(1, 80, true);    /* タイマ設定 240分周→1[us] */
    timerAttachInterrupt(timer1, tim_vdIntervalTimer1ms, true);    /* 割込関数セット */
    timerAlarmWrite(timer1, 1000, true);    /* 割込タイミング 1000カウントで発火→1[ms] */
    timerAlarmEnable(timer1); /* タイマ開始 */
    return;
}

void TIM_vdInit(void)
{
    tim_vdInitTimer0();
    tim_vdInitTimer1();
    return;
}


void TIM_vdMain(void)
{
    return;
}


void TIM_vdSt(void)
{
    s_t_tim.flg = true;
    s_t_tim.st_time = 0;
    s_t_tim.now_time = 0;
    return;
}

void TIM_vdSp(void)
{
    s_t_tim.flg = false;
    s_t_tim.st_time = 0;
    s_t_tim.now_time = 0;
    return;
}

unsigned short TIM_s2ElapsedTime(void)
{
    return s_t_tim.now_time - s_t_tim.st_time;
}


uint16_t TIM_u2NowTime(void){return s_t_tim.now_time;}
