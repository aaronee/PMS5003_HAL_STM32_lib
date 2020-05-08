
/*
 *	@File: main.c
 *
 *	@Created on: May, 05, 2020
 *  @Author: AARONEE - khoahuynh
 * 	@Email: khoahuynh.ld@gmail.com
 * 	@Github: https://github.com/aaronee
 *
 *
 *	@Introduction: This file belongs to PMS5003_HAL_STM32 library
 *				> This is the example project to demonstrate PMS5003_HAL_STM32 library
 *				> This project is conducted on Eclipse IDE
 *				> Hardware: STM32L4R5ZI Nucleo board + PMS5003 air sensor
 *
 *	@MainFunction:
 *				> Initialize Sensor
 *				> Control sensor via UART2
 *				> Send collected data via LPUART1
 *
 *
 *	Copyright (C) 2020 - Khoa Huynh
 *  This is a free software under the GNU license, you can redistribute it and/or modify it under the terms
 *	of the GNU General Public Licenseversion 3 as published by the Free Software Foundation.
 *
 *	This software library is shared with puplic for educational purposes, without WARRANTY and Author is not liable for any damages caused directly
 *	or indirectly by this software, read more about this on the GNU General Public License.
 */

#include "stm32l4xx_hal_conf.h"
#include "stm32l4r5xx.h"
#include "PMS5003_HAL_STM32.h"
#include "main.h"
#include <string.h>
#include <stdio.h>

void SystemClockConfig(void);
void lpuart1_Init(void);
void GPIO_Init(void);
void uart3_Init(void);
void sensor_init(void);
void Error_handler(void);

UART_HandleTypeDef hlpuart1;
UART_HandleTypeDef huart2;


PMS_typedef PMS5003 = {0};

char mesg[1000] = {0};


//////////////////////////////////////////////////////////////////////////////////
//
//					Main() begins
//
//////////////////////////////////////////////////////////////////////////////////
int main(void)
{
	HAL_Init();
	SystemClockConfig();
	lpuart1_Init();
	uart3_Init();
	GPIO_Init();
	sensor_init();

	while(1)
	{

		if (PMS_read(&PMS5003) == PMS_OK)
		{

			sprintf(mesg,"\r\n\
PM1.0 factory	  is : %d \r\n\
PM2.5 factory	  is : %d \r\n\
PM10  factory	  is : %d \r\n\
PM1.0 atmospheric is : %d \r\n\
PM2.5 atmospheric is : %d \r\n\
PM10  atmospheric is : %d \r\n\
0.3um density	  is : %d \r\n\
0.5um density	  is : %d \r\n\
1.0um density 	  is : %d \r\n\
2.5um density 	  is : %d \r\n\
5.0um density 	  is : %d \r\n\
10um  density	  is : %d \r\n",PMS5003.PM1_0_factory,
			PMS5003.PM2_5_factory,PMS5003.PM10_factory,PMS5003.PM1_0_atmospheric,
			PMS5003.PM2_5_atmospheric,PMS5003.PM10_atmospheric,PMS5003.density_0_3um,
			PMS5003.density_0_5um,PMS5003.density_1_0um,PMS5003.density_2_5um,
			PMS5003.density_5_0um,PMS5003.density_10um);
			HAL_UART_Transmit(&hlpuart1,(uint8_t*)mesg,strlen(mesg),HAL_MAX_DELAY);
		}
		else
		{
			HAL_UART_Transmit(&hlpuart1,(uint8_t*)"Read fail\r\n",strlen("Read fail\r\n"),HAL_MAX_DELAY);
		}
		HAL_Delay(5000);

	}
	return 0;

}
//////////////////////////////////////////////////////////////////////////////////
//
//					Functions for PMS5003 configuration
//
//////////////////////////////////////////////////////////////////////////////////
void sensor_init(void)
{
	PMS5003.PMS_huart = &huart2; 			// passing uart handler to communicate with sensor
	PMS5003.PMS_MODE = PMS_MODE_PASSIVE;	// choosing the MODE
	if (PMS_Init(&PMS5003) != PMS_OK)
	{
		HAL_UART_Transmit(&hlpuart1,(uint8_t*)"PMS_Init fail",strlen("PMS_Init fail"),HAL_MAX_DELAY);
		Error_handler();
	}
}

//////////////////////////////////////////////////////////////////////////////////
//
//					Functions for System configuration
//
//////////////////////////////////////////////////////////////////////////////////
void SystemClockConfig(void)
{
	RCC_OscInitTypeDef Osc_Init_Param = {0};
	RCC_ClkInitTypeDef Clk_Init_Param = {0};

	Osc_Init_Param.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	Osc_Init_Param.HSIState = RCC_HSI_ON;
	Osc_Init_Param.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;

	if (HAL_RCC_OscConfig(&Osc_Init_Param) != HAL_OK)
	{
		Error_handler();
	}

	Clk_Init_Param.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
	Clk_Init_Param.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK |
								RCC_CLOCKTYPE_PCLK1	| RCC_CLOCKTYPE_PCLK2;
	Clk_Init_Param.AHBCLKDivider = RCC_SYSCLK_DIV1;
	Clk_Init_Param.APB1CLKDivider = RCC_HCLK_DIV1;
	Clk_Init_Param.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&Clk_Init_Param, FLASH_LATENCY_0) != HAL_OK)
	{
		Error_handler();
	}

	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
}
void lpuart1_Init(void)
{
	hlpuart1.Instance = LPUART1;
	hlpuart1.Init.BaudRate = 9600;
	hlpuart1.Init.WordLength = UART_WORDLENGTH_8B;
	hlpuart1.Init.StopBits = UART_STOPBITS_1;
	hlpuart1.Init.Parity = UART_PARITY_NONE;
	hlpuart1.Init.Mode = UART_MODE_TX_RX;
	hlpuart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	hlpuart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
	hlpuart1.Init.ClockPrescaler = UART_PRESCALER_DIV1;
	hlpuart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
	hlpuart1.FifoMode = UART_FIFOMODE_DISABLE;
	if ( HAL_UART_Init(&hlpuart1)!=HAL_OK)
	{
		Error_handler();
	}
}

void uart3_Init(void)
{
	huart2.Instance = USART2;
	huart2.Init.BaudRate = 9600;
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.StopBits = UART_STOPBITS_1;
	huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart2.Init.Mode =  UART_MODE_TX_RX;

	if( HAL_UART_Init(&huart2) != HAL_OK)
	{
		Error_handler();
	}
}

void GPIO_Init(void)
{
	__HAL_RCC_GPIOB_CLK_ENABLE();
	GPIO_InitTypeDef ledgpio;
	ledgpio.Pin = GPIO_PIN_14;
	ledgpio.Mode = GPIO_MODE_OUTPUT_PP;
	ledgpio.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOB,&ledgpio);

}
void Error_handler(void)
{
	while(1);
}


