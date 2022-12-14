/* Includes ------------------------------------------------------------------*/
#include "i2c.h"
#include "encoder.h"
#include <string.h>
#include <stdlib.h>
#include "stdint.h"

#define I2C_WRITE_OPERATION		0
#define I2C_READ_OPERATION		1
#define	I2C_RECEIVE_BUFFER_LEN	20

volatile uint8_t rx_buffer[I2C_RECEIVE_BUFFER_LEN];
volatile uint8_t tx_buffer[I2C_RECEIVE_BUFFER_LEN];
volatile uint16_t tx_len = 0;
volatile uint8_t tx_state = 0;

volatile uint8_t fm_version = FIRMWARE_VERSION;
uint8_t i2c_address[1] = {0};

void i2c1_set_send_data(uint8_t *tx_ptr, uint16_t len);

extern __IO int32_t encoder_countAB;
extern __IO int32_t increment_countAB;
extern __IO uint16_t encoder_countPulse;
extern __IO uint8_t ucPressState;
extern __IO int8_t count_mode;

enum i2c_reg {
	ENCODER = 0x00,
	INCREMENT = 0x10,
	BUTTON = 0x20,
	NEO_LED = 0x30,
	RESET_COUNT = 0x40,
	FIRMWARE = 0xFE,
	I2C_ADDR = 0xFF,
} I2C_Register;

void user_i2c_init(void)
{
  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x2000090E;
  hi2c1.Init.OwnAddress1 = i2c_address[0]<<1;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    Error_Handler();
  }

}

void i2c_address_write_to_flash(void) 
{   
  writeMessageToFlash(i2c_address , 1);   
}

void i2c_address_read_from_flash(void) 
{   
  if (!(readPackedMessageFromFlash(i2c_address, 1))) {
    i2c_address[0] = I2C_ADDRESS;
    i2c_address_write_to_flash();
  }
}

__weak void i2c1_receive_callback(uint8_t *rx_data, uint16_t len) {
	/* Prevent unused argument(s) compilation warning */
	// if(rx_data[0] == MODE) {
	// 	if(rx_data[1] == 0 | rx_data[1] == 1 )
	// 	{
	// 		if(count_mode != rx_data[1])
	// 		{
	// 			encoder_countAB = 0;
	// 			encoder_countPulse = 0;
	// 			advanceModeDeinit(count_mode);
	// 			count_mode = rx_data[1];
	// 			advanceModeInit(count_mode);
	// 		}
	// 	}
	// }
	if(rx_data[0] == ENCODER) {
		if(len == 1)
		{
			if (count_mode == 0){
				i2c1_set_send_data((uint8_t *)&encoder_countAB, 4);
			}
		}
		else if(len > 1)
		{
			if (count_mode == 0 && len == 5){
				encoder_countAB = (rx_data[1] | (rx_data[2] << 8) | (rx_data[3] << 16) | (rx_data[4] << 24));
			}
		}
	} 
	else if(len == 1 && rx_data[0] == INCREMENT) {
		if (count_mode == 0){
			i2c1_set_send_data((uint8_t *)&increment_countAB, 4);
			increment_countAB = 0;
		}
	} 
	else if(len == 1 && rx_data[0] == BUTTON) {
		i2c1_set_send_data((uint8_t *)&ucPressState, 1);
	} 
	else if (rx_data[0] == NEO_LED) {
		if (len == 4) {
			neopixel_set_all_color(rx_data[1] << 16 | rx_data[2] << 8 | rx_data[3]);
			neopixel_show();
		}
  	}
	else if (rx_data[0] == RESET_COUNT) {
			if(rx_data[1]){
				if (count_mode == 0){
					encoder_countAB = 0;
				}
				else if (count_mode == 1){
					encoder_countPulse = 0;
				}
			}
	}
	else if (rx_data[0] == FIRMWARE) {
		if(len == 1) {
			i2c1_set_send_data((uint8_t *)&fm_version, 1);
		}
	}
	else if (len > 1 && (rx_data[0] == 0xFF)) {
		if (len == 2) {
			if (rx_data[1] < 128) {
				i2c_address[0] = rx_data[1];
				i2c_address_write_to_flash();
				user_i2c_init();
			}
		}
  	}  
	else if (len == 1 && (rx_data[0] == 0xFF)) {
		if (i2c_address != 0)
			i2c1_set_send_data(i2c_address, 1);
  	} 	
	/* NOTE : This function should not be modified, when the callback is needed,
			  the i2c1_receive_callback could be implemented in the user file
	 */
}

__weak void i2c1_addr_req_callback(uint8_t TransferDirection) {
  UNUSED(TransferDirection);
}

void i2c1_set_send_data(uint8_t *tx_ptr, uint16_t len) {
  if (len > I2C_RECEIVE_BUFFER_LEN) {
    len = I2C_RECEIVE_BUFFER_LEN;
	}
	tx_len = len;

  if (len == 0 || tx_ptr == NULL) {
    return;
  }
  memcpy((void *)tx_buffer, tx_ptr, len);
}

void I2CInit(void)
{
  HAL_I2C_EnableListen_IT(&hi2c1);
}

void HAL_I2C_AddrCallback(I2C_HandleTypeDef *hi2c, uint8_t TransferDirection, uint16_t AddrMatchCode) {
	if (hi2c->Instance == hi2c1.Instance) {
		hi2c->State = HAL_I2C_STATE_READY;
		i2c1_addr_req_callback(TransferDirection);
		if (TransferDirection == I2C_WRITE_OPERATION) {
			HAL_I2C_Slave_Receive_IT(hi2c, (uint8_t *)rx_buffer, I2C_RECEIVE_BUFFER_LEN);
		}
		else {
			HAL_I2C_Slave_Transmit_IT(hi2c, (uint8_t *)tx_buffer, tx_len);
      tx_state = 1;
		}
    __HAL_I2C_CLEAR_FLAG(hi2c, I2C_FLAG_ADDR);
		HAL_I2C_EnableListen_IT(&hi2c1);
	}
}

// read finish will callback
void HAL_I2C_ListenCpltCallback(I2C_HandleTypeDef *hi2c) {					//���ص�
	if (hi2c->Instance == hi2c1.Instance) {

    if (tx_state != 1) {
      i2c1_receive_callback((uint8_t *)&rx_buffer[0], I2C_RECEIVE_BUFFER_LEN - hi2c->XferSize);
    }
    tx_state = 0;
		HAL_I2C_EnableListen_IT(&hi2c1);
	}
}

// write finish will callback
void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *hi2c) {						//д�ص�
	if (hi2c->Instance == hi2c1.Instance) {
		i2c1_receive_callback((uint8_t *)&rx_buffer[0], I2C_RECEIVE_BUFFER_LEN);
		HAL_I2C_EnableListen_IT(&hi2c1);
	}
}

// write finish will callback
void HAL_I2C_SlaveTxCpltCallback(I2C_HandleTypeDef *hi2c) {
	if (hi2c->Instance == hi2c1.Instance) {
    tx_state = 0;
		HAL_I2C_EnableListen_IT(&hi2c1);
	}
}

void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c) {
    if (hi2c->Instance == hi2c1.Instance) {
		HAL_I2C_EnableListen_IT(&hi2c1);
		__HAL_I2C_GENERATE_NACK(&hi2c1);
	}
}

