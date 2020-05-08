/*
 * it.c
 *
 *  Created on: Dec. 22, 2019
 *      Author: khoahuynh
 */

void SysTick_Handler (void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();

}


