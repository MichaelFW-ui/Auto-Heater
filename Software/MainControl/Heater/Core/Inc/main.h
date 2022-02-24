/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define UART2_WEB 1
#define SPI3_Flash 1
#define VirtualPort_DebugOutput 1
#define UART1_UpperClassControl 1
#define SPI1_IPS 1
#define ADC1_NTC 1
#define IPS_CS_Pin GPIO_PIN_4
#define IPS_CS_GPIO_Port GPIOA
#define IPS_RES_Pin GPIO_PIN_0
#define IPS_RES_GPIO_Port GPIOB
#define IPS_DC_Pin GPIO_PIN_1
#define IPS_DC_GPIO_Port GPIOB
#define IPS_BLK_Pin GPIO_PIN_10
#define IPS_BLK_GPIO_Port GPIOB
#define EC11_SW_Pin GPIO_PIN_12
#define EC11_SW_GPIO_Port GPIOB
#define EC11_A_Pin GPIO_PIN_13
#define EC11_A_GPIO_Port GPIOB
#define EC11_C_Pin GPIO_PIN_14
#define EC11_C_GPIO_Port GPIOB
#define PWM_Heat_Pin GPIO_PIN_9
#define PWM_Heat_GPIO_Port GPIOA
#define Flash_Hold_Pin GPIO_PIN_15
#define Flash_Hold_GPIO_Port GPIOA
#define Flash_CS_Pin GPIO_PIN_7
#define Flash_CS_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
