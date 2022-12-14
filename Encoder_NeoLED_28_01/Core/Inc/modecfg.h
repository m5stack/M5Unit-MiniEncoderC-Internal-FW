#ifndef __MODECFG__H_
#define __MODECFG__H_

#include "main.h"
extern void inputModeInit(uint8_t num);
extern void inputModeDeinit(uint8_t num);
extern void pulseModeInit(uint8_t num);
extern void pulseModeDeinit(uint8_t num);
extern void encodeABModeInit(void);
extern void encodeABModeDeInit(void);
extern void encodePulseModeInit(void);
extern void encodePulseModeDeinit(void);
extern void modeDeinit(uint8_t num, uint8_t mode);
extern void modeInit(uint8_t num, uint8_t mode);
extern void modeConfig(uint8_t currMode,uint8_t lastMode);
extern void advanceModeInit(uint8_t mode);
extern void advanceModeDeinit(uint8_t mode);
extern GPIO_TypeDef *GPIOx_List[4];
extern const uint16_t GPIO_Pin_list[4];
#endif
