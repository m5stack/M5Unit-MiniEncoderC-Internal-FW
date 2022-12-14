/**
  ******************************************************************************
  * File Name          : encoder.c
  * Description        : This file provides code for the configuration
  *                      of all used GPIO pins.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "encoder.h"
#include "tim.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
*/
// encoder
uint16_t usNowCnt;
uint16_t usLastCnt = TIM_ENCODE_DEFAULT;
__IO int8_t cDirCnt = 1;
__IO int32_t encoder_countAB;
__IO int32_t increment_countAB;
__IO uint16_t encoder_countPulse;
__IO uint8_t ucPressState = 0;
__IO int8_t count_mode = 0;

uint8_t led_num;
uint32_t *color_buf = NULL;
void color_set_single(uint32_t color);

void restart(void) {
  for (uint8_t i = 0; i < 113; i++) {
    delay_600ns();
  }
}

void GPIO_init(void) {
	__HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  GPIO_InitTypeDef GPIO_InitStruct = { 0 };
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void sk6812_init(uint8_t num) {
  GPIO_init();
  color_buf = (uint32_t *)calloc(num, sizeof(uint32_t));
  led_num = num;
}

void neopixel_set_color(uint8_t num, uint32_t color) {
	uint8_t rled = (color >> 16) & 0xff;
	uint8_t gled = (color >> 8) & 0xff;
	uint8_t bled = color  & 0xff;
  color_buf[num] = gled << 16 | rled << 8 | bled;
}

void neopixel_set_all_color(uint32_t color) {
  for (uint8_t i = 0; i < led_num; i++) {
		uint8_t rled = (color >> 16) & 0xff;
		uint8_t gled = (color >> 8) & 0xff;
		uint8_t bled = color  & 0xff;
    color_buf[i] = gled << 16 | rled << 8 | bled;
  }
}

void neopixel_show(void) {
  __disable_irq();
  for (uint8_t i = 0; i < led_num; i++) {
    color_set_single(color_buf[i]);
  }
  __enable_irq();
  restart();
}

void color_set_single(uint32_t color) {
  for (uint8_t i = 0; i < 24; i++) {
    if (color & (1 << (23 - i))) {
      out_bit_high();
    }
    else {
      out_bit_low();
    }
  }
}

void EncoderABModeInit(void)
{
	// this is internal encoder timer3 CH1 & CH2 initial
	cDirCnt = 1;
	usLastCnt = TIM_ENCODE_DEFAULT;
	MX_TIM3_Init();
  HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL);
  __HAL_TIM_ENABLE_IT(&htim3, TIM_IT_UPDATE);
  __HAL_TIM_SET_COUNTER(&htim3, TIM_ENCODE_DEFAULT);
}

void EncoderABModeDeinit(void)
{
	__HAL_TIM_DISABLE_IT(&htim3, TIM_IT_UPDATE);
  HAL_TIM_Encoder_Stop(&htim3, TIM_CHANNEL_ALL);
  HAL_TIM_Encoder_DeInit(&htim3);
}

void pulseCounterInit(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
  memset(&GPIO_InitStruct, 0, sizeof(GPIO_InitTypeDef));
  GPIO_InitStruct.Pin = GPIO_PIN_6;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  
	HAL_NVIC_SetPriority(EXTI4_15_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);
}

void pulseCounterDeinit(void)
{
	HAL_GPIO_DeInit(GPIOA, GPIO_PIN_6);
  HAL_NVIC_DisableIRQ(EXTI4_15_IRQn);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
  if (htim->Instance == TIM3) {
    cDirCnt = (TIM3->CNT > 60535) ? cDirCnt + 1 : cDirCnt - 1;
  }
}

void advanceModeInit(uint8_t mode)
{
	if(mode == 0)
	{
		EncoderABModeInit();
	}
	else if(mode == 1)
	{
		pulseCounterInit();
	}
}

void advanceModeDeinit(uint8_t mode)
{
	if(mode == 0)
	{
		EncoderABModeDeinit();
	}
	else if(mode == 1)
	{
		pulseCounterDeinit();
	}
}

// Encoder AB
void encode_update(void) {
	if(count_mode == 0)
	{
		usNowCnt = __HAL_TIM_GET_COUNTER(&htim3);
		if (cDirCnt < 0) {
			encoder_countAB += 65536 + usNowCnt - usLastCnt;
			increment_countAB += 65536 + usNowCnt - usLastCnt;
		} else if(cDirCnt > 0){
			encoder_countAB += -65536 + usNowCnt - usLastCnt;
			increment_countAB += -65536 + usNowCnt - usLastCnt;
		} else {
			encoder_countAB += usNowCnt - usLastCnt;
			increment_countAB += usNowCnt - usLastCnt;
		}
		usLastCnt = usNowCnt;
		cDirCnt = 0;
	}
	ucPressState = HAL_GPIO_ReadPin(Button_GPIO_Port, Button_Pin);
}

/* USER CODE BEGIN 2 */

/* USER CODE END 2 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
