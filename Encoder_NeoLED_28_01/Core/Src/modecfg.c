#include "modecfg.h"
#include "tim.h"
#include "encoder.h"
#include "string.h"

// ----------------------------     IN0         IN1         IN2         IN3
GPIO_TypeDef *GPIOx_List[4] =     { GPIOA,      GPIOA,      GPIOB,      GPIOA };
const uint16_t GPIO_Pin_list[4] = { GPIO_PIN_6, GPIO_PIN_7, GPIO_PIN_1, GPIO_PIN_4 };
extern __IO int8_t cPulseDir;

//void inputModeInit(uint8_t num) {
//  GPIO_InitTypeDef GPIO_InitStruct = { 0 };

//  GPIO_InitStruct.Pin = GPIO_Pin_list[num];
//  GPIO_InitStruct.Mode = GPIO_MODE_INPUT; 
//  GPIO_InitStruct.Pull = GPIO_PULLUP;
//  HAL_GPIO_Init(GPIOx_List[num], &GPIO_InitStruct);
//}

//void inputModeDeinit(uint8_t num) {
//  HAL_GPIO_DeInit(GPIOx_List[num], GPIO_Pin_list[num]);
//}

//void pulseModeInit(uint8_t num) {  
//  GPIO_InitTypeDef GPIO_InitStruct;
//  memset(&GPIO_InitStruct, 0, sizeof(GPIO_InitTypeDef));
//  GPIO_InitStruct.Pin = GPIO_Pin_list[num];
//  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
//  GPIO_InitStruct.Pull = GPIO_PULLUP;
//  HAL_GPIO_Init(GPIOx_List[num], &GPIO_InitStruct);
//  
//  if (num == 2) {
//    HAL_NVIC_SetPriority(EXTI0_1_IRQn, 0, 0);
//    HAL_NVIC_EnableIRQ(EXTI0_1_IRQn);
//  } else {
//    HAL_NVIC_SetPriority(EXTI4_15_IRQn, 0, 0);
//    HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);
//  }
//}

//void pulseModeDeinit(uint8_t num) {
//  HAL_GPIO_DeInit(GPIOx_List[num], GPIO_Pin_list[num]);
//  if (num == 2) {
//    HAL_NVIC_DisableIRQ(EXTI0_1_IRQn);
//  } else {
//    // HAL_NVIC_DisableIRQ(EXTI4_15_IRQn);
//  }
//}

//void encodeABModeInit() {
//  MX_TIM3_Init();
//  HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL);
//  __HAL_TIM_ENABLE_IT(&htim3, TIM_IT_UPDATE);
//  __HAL_TIM_SET_COUNTER(&htim3, TIM_ENCODE_DEFAULT);
//}

//void encodeABModeDeinit() {
//  __HAL_TIM_DISABLE_IT(&htim3, TIM_IT_UPDATE);
//  HAL_TIM_Encoder_Stop(&htim3, TIM_CHANNEL_ALL);
//  HAL_TIM_Encoder_DeInit(&htim3);
//}

//void encodePulseModeInit() {
//  GPIO_InitTypeDef GPIO_InitStruct = { 0 };

//  GPIO_InitStruct.Pin = GPIO_Pin_list[2];
//  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
//  GPIO_InitStruct.Pull = GPIO_PULLUP;
//  HAL_GPIO_Init(GPIOx_List[2], &GPIO_InitStruct);

//  GPIO_InitStruct.Pin = GPIO_Pin_list[3];
//  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
//  HAL_GPIO_Init(GPIOx_List[3], &GPIO_InitStruct);

//  if ((GPIOB->IDR & GPIO_PIN_1) != GPIO_PIN_RESET) {
//    cPulseDir = 1;
//  }
//  else {
//    cPulseDir = -1;
//  }

//  HAL_NVIC_SetPriority(EXTI0_1_IRQn, 0, 0);
//  HAL_NVIC_EnableIRQ(EXTI0_1_IRQn);
//  HAL_NVIC_SetPriority(EXTI4_15_IRQn, 0, 0);
//  HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);
//}

//void encodePulseModeDeinit() {
//  HAL_NVIC_DisableIRQ(EXTI0_1_IRQn);
//  HAL_NVIC_DisableIRQ(EXTI4_15_IRQn);
//  HAL_GPIO_DeInit(GPIOx_List[3], GPIO_Pin_list[3]);
//  HAL_GPIO_DeInit(GPIOx_List[2], GPIO_Pin_list[2]);
//}

//void modeDeinit(uint8_t num, uint8_t mode) {
//  uint8_t state = (mode >> (num * 2)) & 0x03;
//  switch (state) {
//    case 0x00:
//      inputModeDeinit(num);
//      break;
//    case 0x01:
//      pulseModeDeinit(num);
//      break;
//    case 0x02:
//      encodePulseModeDeinit();
//      break;
//    case 0x03:
//      encodeABModeDeinit();
//      break;
//    default:
//      break;
//  }
//}

//void modeInit(uint8_t num, uint8_t mode) {
//  uint8_t state = (mode >> (num * 2)) & 0x03;
//  switch (state) {
//    case 0x00:
//      inputModeInit(num);
//      break;
//    case 0x01:
//      pulseModeInit(num);
//      break;
//    case 0x02:
//      encodePulseModeInit();
//      break;
//    case 0x03:
//      encodeABModeInit();
//      break;
//    default:
//      break;
//  }
//}

void zeroPulseCounterInit(uint8_t num)
{
	GPIO_InitTypeDef GPIO_InitStruct;
  memset(&GPIO_InitStruct, 0, sizeof(GPIO_InitTypeDef));
  GPIO_InitStruct.Pin = GPIO_Pin_list[num];
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOx_List[num], &GPIO_InitStruct);
  
  if (num == 2) {
    HAL_NVIC_SetPriority(EXTI0_1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI0_1_IRQn);
  } else {
    HAL_NVIC_SetPriority(EXTI4_15_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);
  }
}

void zeroPulseCounterDeinit(uint8_t num)
{
	HAL_GPIO_DeInit(GPIOx_List[num], GPIO_Pin_list[num]);
  if (num == 2) {
    HAL_NVIC_DisableIRQ(EXTI0_1_IRQn);
  } else {
    HAL_NVIC_DisableIRQ(EXTI4_15_IRQn);
  }
}

void dirPulseCounterInit(uint8_t num, uint8_t pulseEdge, uint8_t dirNum, uint8_t dirEdge)
{
	if(num != dirNum)
	{
		GPIO_InitTypeDef GPIO_InitStruct;
		memset(&GPIO_InitStruct, 0, sizeof(GPIO_InitTypeDef));
		GPIO_InitStruct.Pin = GPIO_Pin_list[num];
		if(pulseEdge == High)
			GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
		else if(pulseEdge == Low)
			GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
		HAL_GPIO_Init(GPIOx_List[num], &GPIO_InitStruct);
		
		GPIO_InitStruct.Pin = GPIO_Pin_list[dirNum];
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		if(dirEdge == High)
			GPIO_InitStruct.Pull = GPIO_PULLUP;
		else if(dirEdge == Low)
			GPIO_InitStruct.Pull = GPIO_PULLDOWN;
		HAL_GPIO_Init(GPIOx_List[dirNum], &GPIO_InitStruct);
		
		if (num == 2) {
			HAL_NVIC_SetPriority(EXTI0_1_IRQn, 0, 0);
			HAL_NVIC_EnableIRQ(EXTI0_1_IRQn);
		} else {
			HAL_NVIC_SetPriority(EXTI4_15_IRQn, 0, 0);
			HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);
		}
	}
}

void dirPulseCounterDeinit(uint8_t num, uint8_t dirNum)
{
	HAL_GPIO_DeInit(GPIOx_List[num], GPIO_Pin_list[num]);
	HAL_GPIO_DeInit(GPIOx_List[dirNum], GPIO_Pin_list[dirNum]);
  if (num == 2) {
    HAL_NVIC_DisableIRQ(EXTI0_1_IRQn);
  } else {
    HAL_NVIC_DisableIRQ(EXTI4_15_IRQn);
  }
}


void pulseCounterInit(uint8_t num, uint8_t pulseEdge)
{
	GPIO_InitTypeDef GPIO_InitStruct;
  memset(&GPIO_InitStruct, 0, sizeof(GPIO_InitTypeDef));
  GPIO_InitStruct.Pin = GPIO_Pin_list[num];
	if(pulseEdge == High)
		GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
	else if(pulseEdge == Low)
		GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOx_List[num], &GPIO_InitStruct);
  
  if (num == 2) {
    HAL_NVIC_SetPriority(EXTI0_1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI0_1_IRQn);
  } else {
    HAL_NVIC_SetPriority(EXTI4_15_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);
  }
}

void pulseCounterDeinit(uint8_t num)
{
	HAL_GPIO_DeInit(GPIOx_List[num], GPIO_Pin_list[num]);
  if (num == 2) {
    HAL_NVIC_DisableIRQ(EXTI0_1_IRQn);
  } else {
    HAL_NVIC_DisableIRQ(EXTI4_15_IRQn);
  }
}

void oneCH_EncoderABModeInit(void)
{
	// this is internal encoder timer3 CH1 & CH2 initial
	cDirCnt = 1;
	usLastCnt = TIM_ENCODE_DEFAULT;
	MX_TIM3_Init();
  HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL);
  __HAL_TIM_ENABLE_IT(&htim3, TIM_IT_UPDATE);
  __HAL_TIM_SET_COUNTER(&htim3, TIM_ENCODE_DEFAULT);
}

void oneCH_EncoderABModeDeinit(void)
{
	__HAL_TIM_DISABLE_IT(&htim3, TIM_IT_UPDATE);
  HAL_TIM_Encoder_Stop(&htim3, TIM_CHANNEL_ALL);
  HAL_TIM_Encoder_DeInit(&htim3);
}

void twoCH_EncoderABModeInit(void)
{
	// this is internal encoder timer3 CH1 & CH2 initial
	cDirCnt = 1;
	usLastCnt = TIM_ENCODE_DEFAULT;
	MX_TIM3_Init();
  HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL);
  __HAL_TIM_ENABLE_IT(&htim3, TIM_IT_UPDATE);
  __HAL_TIM_SET_COUNTER(&htim3, TIM_ENCODE_DEFAULT);
	
	// this is external interrupt 1 & 4 use encoder initial
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };

  GPIO_InitStruct.Pin = GPIO_Pin_list[2];
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOx_List[2], &GPIO_InitStruct);

  GPIO_InitStruct.Pin = GPIO_Pin_list[3];
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  HAL_GPIO_Init(GPIOx_List[3], &GPIO_InitStruct);

//  if ((GPIOB->IDR & GPIO_PIN_1) != GPIO_PIN_RESET) {
//    cPulseDir = 1;
//  }
//  else {
//    cPulseDir = -1;
//  }

  HAL_NVIC_SetPriority(EXTI0_1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI0_1_IRQn);
  HAL_NVIC_SetPriority(EXTI4_15_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);
}

void twoCH_EncoderABModeDeinit(void)
{
	__HAL_TIM_DISABLE_IT(&htim3, TIM_IT_UPDATE);
  HAL_TIM_Encoder_Stop(&htim3, TIM_CHANNEL_ALL);
  HAL_TIM_Encoder_DeInit(&htim3);
	
	HAL_NVIC_DisableIRQ(EXTI0_1_IRQn);
  HAL_NVIC_DisableIRQ(EXTI4_15_IRQn);
  HAL_GPIO_DeInit(GPIOx_List[3], GPIO_Pin_list[3]);
  HAL_GPIO_DeInit(GPIOx_List[2], GPIO_Pin_list[2]);
}

void advanceModeInit(uint8_t mode)
{
	if(mode >= 0 && mode <= 19)
	{
		switch (mode) {
				case 0:
						encoder_countAB1 = 0;
						encoder_countAB2 = 0;
//						encoder_countZero = 0;
						for (uint8_t i = 0; i < 4; i++)
							uiINPulseCount[i] = 0;
						for (uint8_t i = 0; i < 2; i++)
							uiDIRPulseCount[i] = 0;
					break;
				case 1:
						twoCH_EncoderABModeInit();
					break;
				case 2:
						oneCH_EncoderABModeInit();
						pulseCounterInit(2, High);
						pulseCounterInit(3, High);
					break;
				case 3:
						oneCH_EncoderABModeInit();
						pulseCounterInit(2, Low);
						pulseCounterInit(3, Low);
					break;
				case 4:
						oneCH_EncoderABModeInit();
						pulseCounterInit(2, High);
						pulseCounterInit(3, Low);
					break;
				case 5:
						// Deinit function add
						oneCH_EncoderABModeInit();
						zeroPulseCounterInit(2);
						pulseCounterInit(3, High);
					break;
				case 6:
						// Deinit function add
						oneCH_EncoderABModeInit();
						zeroPulseCounterInit(2);
						pulseCounterInit(3, Low);
					break;
				case 7:
						// Deinit function add
						oneCH_EncoderABModeInit();
						dirPulseCounterInit(3, High, 2, High);	
					break;
				case 8:
						// Deinit function add
						oneCH_EncoderABModeInit();
						dirPulseCounterInit(3, Low, 2, High);
					break;
				case 9:
						// Deinit function add
						oneCH_EncoderABModeInit();
						dirPulseCounterInit(3, High, 2, Low);
					break;
				case 10:
						// Deinit function add
						oneCH_EncoderABModeInit();
						dirPulseCounterInit(3, Low, 2, Low);
					break;
				case 11:
						// Deinit function add
						dirPulseCounterInit(1, High, 0, High);
						dirPulseCounterInit(3, High, 2, High);
					break;
				case 12:
						// Deinit function add
						dirPulseCounterInit(1, Low, 0, High);
						dirPulseCounterInit(3, Low, 2, High);
					break;
				case 13:
						// Deinit function add
						dirPulseCounterInit(1, High, 0, Low);
						dirPulseCounterInit(3, High, 2, Low);
					break;
				case 14:
						// Deinit function add
						dirPulseCounterInit(1, Low, 0, Low);
						dirPulseCounterInit(3, Low, 2, Low);
					break;
				case 15:
						pulseCounterInit(0, High);
						pulseCounterInit(1, High);
						pulseCounterInit(2, High);
						pulseCounterInit(3, High); 
					break;
				case 16:
						pulseCounterInit(0, Low);
						pulseCounterInit(1, High);
						pulseCounterInit(2, High);
						pulseCounterInit(3, High); 
					break;
				case 17:
						pulseCounterInit(0, Low);
						pulseCounterInit(1, Low);
						pulseCounterInit(2, High);
						pulseCounterInit(3, High);
					break;
				case 18:
						pulseCounterInit(0, Low);
						pulseCounterInit(1, Low);
						pulseCounterInit(2, Low);
						pulseCounterInit(3, High);
					break;
				case 19:
						pulseCounterInit(0, Low);
						pulseCounterInit(1, Low);
						pulseCounterInit(2, Low);
						pulseCounterInit(3, Low);
					break;
				default:
					break;
			}
	}
}

void advanceModeDeinit(uint8_t mode)
{
	switch (mode) {
			case 1:
				// Deinit function add
					twoCH_EncoderABModeDeinit();
				break;
			case 2:
			case 3:
			case 4:
					// Deinit function add
					oneCH_EncoderABModeDeinit();
					pulseCounterDeinit(2);
					pulseCounterDeinit(3);
				break;
			case 5:
			case 6:
			 // Deinit function add
					oneCH_EncoderABModeInit();
					zeroPulseCounterDeinit(2);
					pulseCounterDeinit(3);
				break;
			case 7:
			case 8:
			case 9:
			case 10:
					// Deinit function add
					oneCH_EncoderABModeInit();
					dirPulseCounterDeinit(3,2);		
				break;
			case 11:
			case 12:
			case 13:
			case 14:
					dirPulseCounterDeinit(1,0);
					dirPulseCounterDeinit(3,2);
				break;
			
			case 15:
			case 16:
			case 17:
			case 18:
			case 19:
				// Deinit function add
					pulseCounterDeinit(0);
					pulseCounterDeinit(1);
					pulseCounterDeinit(2);
					pulseCounterDeinit(3);
				break;
			default:
				break;
		}
}



//void modeConfig(uint8_t currMode,uint8_t lastMode)
//{
//	uint8_t configLoop;
//	uint8_t mode = lastMode;
//	for(configLoop = 0;configLoop <= 1;configLoop++)
//	{
//		switch (mode) {
//			case 0x00:
//				if(configLoop == 0)
//				// Deinit function add
//					twoCH_EncoderABModeDeinit();
//				else if(configLoop == 1)
//					twoCH_EncoderABModeInit();
//				break;
//			case 0x01:
//				if(configLoop == 0)
//				{
//				// Deinit function add
//					oneCH_EncoderABModeDeinit();
//					pulseCounterDeinit(2);
//					pulseCounterDeinit(3);
//				}
//				else if(configLoop == 1)
//				{
//					oneCH_EncoderABModeInit();
//					pulseCounterInit(2, High);
//					pulseCounterInit(3, High);
//				}
//				break;
//			case 0x02:
//				if(configLoop == 0)
//				{
//					// Deinit function add
//					oneCH_EncoderABModeDeinit();
//					pulseCounterDeinit(2);
//					pulseCounterDeinit(3);
//				}
//				else if(configLoop == 1)
//				{
//					oneCH_EncoderABModeInit();
//					pulseCounterInit(2, Low);
//					pulseCounterInit(3, Low);
//				}
//				break;
//			case 0x03:
//				if(configLoop == 0)
//				{
//					// Deinit function add
//					oneCH_EncoderABModeDeinit();
//					pulseCounterDeinit(2);
//					pulseCounterDeinit(3);
//				}
//				else if(configLoop == 1)
//				{
//					oneCH_EncoderABModeInit();
//					pulseCounterInit(2, High);
//					pulseCounterInit(3, Low);
//				}
//				break;
//			case 0x04:
//				// Deinit function add
//				oneCH_EncoderABModeInit();
//	//		
//	//		
//	//		
//				break;
//			case 0x05:
//			 // Deinit function add
//				oneCH_EncoderABModeInit();
//	//		
//	//		
//	//		
//				break;
//			case 0x06:
//				// Deinit function add
//				oneCH_EncoderABModeInit();
//	//		
//	//		
//	//		
//				break;
//			case 0x07:
//			 // Deinit function add
//				oneCH_EncoderABModeInit();
//	//		
//	//		
//	//		
//				break;
//			case 0x08:
//				// Deinit function add
//				oneCH_EncoderABModeInit();
//	//		
//	//		
//	//		
//				break;
//			case 0x09:
//			 
//				break;
//			case 0x0A:
//				
//				break;
//			case 0x0B:
//			 
//				break;
//			case 0x0C:
//				
//				break;
//			case 0x0D:
//				if(configLoop == 0)
//				{
//				// Deinit function add
//					pulseCounterDeinit(0);
//					pulseCounterDeinit(1);
//					pulseCounterDeinit(2);
//					pulseCounterDeinit(3);
//				}
//				else if(configLoop == 1)
//				{
//					pulseCounterInit(0, High);
//					pulseCounterInit(1, High);
//					pulseCounterInit(2, High);
//					pulseCounterInit(3, High); 
//				}
//				break;
//			case 0x0E:
//				if(configLoop == 0)
//				{
//				// Deinit function add
//					pulseCounterDeinit(0);
//					pulseCounterDeinit(1);
//					pulseCounterDeinit(2);
//					pulseCounterDeinit(3);
//				}
//				else if(configLoop == 1)
//				{
//					pulseCounterInit(0, Low);
//					pulseCounterInit(1, High);
//					pulseCounterInit(2, High);
//					pulseCounterInit(3, High); 
//				}
//				break;
//			case 0x0F:
//				if(configLoop == 0)
//				{
//				// Deinit function add
//					pulseCounterDeinit(0);
//					pulseCounterDeinit(1);
//					pulseCounterDeinit(2);
//					pulseCounterDeinit(3);
//				}
//				else if(configLoop == 1)
//				{
//					pulseCounterInit(0, Low);
//					pulseCounterInit(1, Low);
//					pulseCounterInit(2, High);
//					pulseCounterInit(3, High); 
//				}
//				break;
//			case 0x10:
//				if(configLoop == 0)
//				{
//				// Deinit function add
//					pulseCounterDeinit(0);
//					pulseCounterDeinit(1);
//					pulseCounterDeinit(2);
//					pulseCounterDeinit(3);
//				}
//				else if(configLoop == 1)
//				{
//					pulseCounterInit(0, Low);
//					pulseCounterInit(1, Low);
//					pulseCounterInit(2, Low);
//					pulseCounterInit(3, High);
//				}
//				break;
//			case 0x11:
//				if(configLoop == 0)
//				{
//				// Deinit function add
//					pulseCounterDeinit(0);
//					pulseCounterDeinit(1);
//					pulseCounterDeinit(2);
//					pulseCounterDeinit(3);
//				}
//				else if(configLoop == 1)
//				{
//					pulseCounterInit(0, Low);
//					pulseCounterInit(1, Low);
//					pulseCounterInit(2, Low);
//					pulseCounterInit(3, Low);
//				}
//				break;
//			default:
//				break;
//		}
//		mode = currMode;
//  }
//}
