/*
 * msp.c
 *
 *  Created on: Dec. 22, 2019
 *      Author: khoahuynh
 */
#include "stm32l4xx_hal.h"

void HAL_MspInit(void)
{
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
	SCB->SHCSR |= 0x07 << 16;
	HAL_NVIC_SetPriority(MemoryManagement_IRQn,0,0);
	HAL_NVIC_SetPriority(BusFault_IRQn,0,0);
	HAL_NVIC_SetPriority(UsageFault_IRQn,0,0);

}

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
	if (huart->Instance == LPUART1)
	{
		__HAL_RCC_PWR_CLK_ENABLE();
	    HAL_PWREx_EnableVddIO2();
		GPIO_InitTypeDef gpio_usart;

		__HAL_RCC_LPUART1_CLK_ENABLE();
		__HAL_RCC_GPIOG_CLK_ENABLE();

		gpio_usart.Pin = GPIO_PIN_7 | GPIO_PIN_8;
		gpio_usart.Mode = GPIO_MODE_AF_PP;
		gpio_usart.Speed = GPIO_SPEED_FREQ_MEDIUM;
		gpio_usart.Alternate = GPIO_AF8_LPUART1;

		HAL_GPIO_Init(GPIOG, &gpio_usart);

		HAL_NVIC_EnableIRQ(LPUART1_IRQn);
		HAL_NVIC_SetPriority(LPUART1_IRQn,15,0);
	}
	if (huart->Instance == USART2)
	{
		GPIO_InitTypeDef gpio_usart;
		__HAL_RCC_USART2_CLK_ENABLE();
		__HAL_RCC_GPIOA_CLK_ENABLE();

		gpio_usart.Pin = GPIO_PIN_2 | GPIO_PIN_3;
		gpio_usart.Mode = GPIO_MODE_AF_PP;
		gpio_usart.Pull = GPIO_NOPULL;
		gpio_usart.Speed = GPIO_SPEED_FREQ_HIGH;
		gpio_usart.Alternate = GPIO_AF7_USART2;

		HAL_GPIO_Init(GPIOA, &gpio_usart);

		HAL_NVIC_EnableIRQ(USART2_IRQn);
		HAL_NVIC_SetPriority(USART2_IRQn,15,0);
	}
}


