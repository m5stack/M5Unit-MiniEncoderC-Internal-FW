/**
  ******************************************************************************
  * File Name          : gpio.h
  * Description        : This file contains all the functions prototypes for 
  *                      the gpio  
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __encoder_H
#define __encoder_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* USER CODE BEGIN Private defines */
#define gpio_low() GPIOA->BRR = GPIO_PIN_0
#define gpio_high() GPIOA->BSRR = GPIO_PIN_0
#define delay_150ns() __asm("NOP"); __asm("NOP"); __asm("NOP"); __asm("NOP"); __asm("NOP"); __asm("NOP")
#define delay_300ns() delay_150ns(); __asm("NOP"); __asm("NOP"); __asm("NOP"); __asm("NOP"); __asm("NOP"); __asm("NOP")
#define delay_600ns() delay_300ns(); delay_300ns()
#define delay_900ns() delay_600ns(); delay_300ns()

#define out_bit_low() \
  gpio_high(); \
  delay_300ns(); \
  gpio_low(); \
  delay_600ns();

#define out_bit_high() \
  gpio_high(); \
  delay_900ns(); \
  gpio_low(); \
  delay_150ns();
	
#define Button_Pin GPIO_PIN_5
#define Button_GPIO_Port GPIOA

/* USER CODE END Private defines */
extern __IO int8_t cDirCnt;
extern uint16_t usLastCnt;

extern __IO int32_t encoder_countAB;
extern __IO int32_t increment_countAB;
extern __IO uint16_t encoder_countPulse;
extern __IO int8_t count_mode;

void encode_update(void);

void pulseCounterInit(void);
void pulseCounterDeinit(void);
void EncodeABModeInit(void);
void EncoderABModeDeinit(void);
void advanceModeInit(uint8_t mode);
void advanceModeDeinit(uint8_t mode);

void restart(void);
void GPIO_init(void);
void sk6812_init(uint8_t num);
void neopixel_set_color(uint8_t num, uint32_t color);
void neopixel_set_all_color(uint32_t color);
void neopixel_show(void);
void color_set_single(uint32_t color);


/* USER CODE BEGIN Prototypes */

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ pinoutConfig_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
