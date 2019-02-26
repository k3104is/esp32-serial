#include "private.h"

static void mng_vdProcessOf1s(void);


void MNG_vdInit(void)
{

    return;
}
static void mng_vdMake1s(void)
{
    static bool s_b_1s = false;
    s_b_1s = !s_b_1s;
    if(s_b_1s)
    {
        mng_vdProcessOf1s();    /* 1[s]周期処理 */
    }
    return;
}

/* ******************************************************************* */
/* タイマ別処理関数 */
/* ******************************************************************* */
static void mng_vdProcessOf500ms(void)
{
    mng_vdMake1s();
    OLED_vdSwReqMain(true);
    LED_vdSwReqMain(true);
    BT_vdDecrementTimeout();
    return;
}
static void mng_vdProcessOf1ms(void)
{   
    BT_vdDecrementWaitTime();
    return;
}
static void mng_vdProcessOf1s(void)
{
    return;
}
/* ******************************************************************* */

static void mng_vdTime(void)
{
    if(TIM_vRtnFlgOf500ms())    /* 500[ms]周期の場合 */
    {
        mng_vdProcessOf500ms(); /* 500[ms]周期処理 */
    }
    if(TIM_vRtnFlgOf1ms())    /* 10[us]周期の場合 */
    {
        mng_vdProcessOf1ms(); /* 10[us]周期処理 */
    }
    return;
}

void MNG_vdMain(void)
{
    mng_vdTime();
    return;
}