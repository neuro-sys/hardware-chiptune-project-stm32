/*
**
**                           Main.c
**
**
**********************************************************************/
/*
   Last committed:     $Revision: 00 $
   Last changed by:    $Author: $
   Last changed date:  $Date:  $
   ID:                 $Id:  $

**********************************************************************/
#include "stm32f4xx_conf.h"

#include "chiptune.h"

#include "codec.h"

#include <stdint.h>
#include <stdio.h>


void initLEDS()
{
    GPIO_InitTypeDef GPIO_TypeStruct;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
    GPIO_StructInit(&GPIO_TypeStruct);
    GPIO_TypeStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_Init(GPIOD, &GPIO_TypeStruct);
}

void TIM2_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
        chiptune_callback();
    }
}

void initTimerInterrupt(void)
{
    NVIC_InitTypeDef NVIC_InitStruct;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

    NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;

    NVIC_Init(&NVIC_InitStruct);

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    TIM_TimeBaseStructure.TIM_Period = 62;
    TIM_TimeBaseStructure.TIM_Prescaler = 167;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

    TIM_Cmd(TIM2, ENABLE);
}

void initAudio(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

	codec_init();
	codec_ctrl_init();

	I2S_Cmd(CODEC_I2S, ENABLE);
}


int main()
{
    SystemInit();
    initLEDS();
    initAudio();

    initTimerInterrupt();

    mainloop();

    return 0;
}
