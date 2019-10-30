#include <stdio.h>
#include "diag/Trace.h"
#include "cmsis/cmsis_device.h"
#include "gpio.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"



/* Global variables. */
volatile double pot_voltage = 0.0;


void myEXTI_Init()
{
	/* Map EXTI1 line to PA1 */
	SYSCFG->EXTICR[0] |= ((uint16_t)0x0000);

	/* EXTI1 line interrupts: set rising-edge trigger */
	EXTI->RTSR |= ((uint16_t)0x0002);

	/* Unmask interrupts from EXTI1 line */
	EXTI->IMR |= ((uint16_t)0x0002);

	/* Assign EXTI1 interrupt priority = 0 in NVIC */
	NVIC_SetPriority(EXTI0_1_IRQn, 0);

	/* Enable EXTI1 interrupts in NVIC */
	NVIC_EnableIRQ(EXTI0_1_IRQn);
}

void myADC_Init()
{
	/* Enable the clock for ADC peripheral*/
	RCC->APB2ENR |= RCC_APB2ENR_ADCEN;
	//ADC1->CR |= ADC_CR_ADEN;
	//ADC1->CFGR1 |= ADC_CFGR1_CONT;
	//ADC1->CHSELR |= ADC_CHSELR_CHSEL7;

}

void myDAC_Init()
{
	/* Enable DAC Control Register. */
	DAC->CR |= DAC_CR_DACEN1;
}

void myTIM2_Init()
{
	/* Enable the clock for TIM2 peripheral. */
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
}

int main(int argc, char* argv[])
{

	InitGPIO();
	myEXTI_Init();		/* Initialize EXTI */
	myADC_Init();		/* Initialize ADC */
	myDAC_Init();		/* Initialize DAC */
	myTIM2_Init();		/* Initialize TIM2 */

	/* ADC->CR |= ADC_CR_ADSTART; */
	while (1)
	{
		/* Nothing here. */
	}

	return 0;

}


/* This handler is declared in system/src/cmsis/vectors_stm32f0xx.c */
void EXTI0_1_IRQHandler()
{
	/* Local variables */
	unsigned int period = 0;
	unsigned int frequency = 0;

	/* Check if EXTI1 interrupt pending flag is indeed set */
	if ((EXTI->PR & EXTI_PR_PR1) != 0)
	{

		/* Code from Part 2 of Introductory Lab. */
		if(first_edge == 1){
			first_edge = 0;
			/* Clear count register (TIM2->CNT) */
			TIM2->CNT &= ~(TIM2->CNT);
			/* Start timer (TIM2->CR1) */
			TIM2->CR1 |= TIM_CR1_CEN;
		}
		else if(first_edge == 0){
			/* Stop timer (TIM2->CR1) */
			TIM2->CR1 &= ~((uint16_t)0x0001);
			/* Calculate signal period and frequency */
			frequency = ((float)SystemCoreClock / ((uint32_t)TIM2->CNT);
			period = 1000.0 / frequency;
			
			/*This needs to be changed to LCD. Lab Tech said to do last once we figure out the LCD display.*/
			/* Print calculated values to the console */
			trace_printf("freq = %u KHz\n ; period = %u ms", frequency, period);
			first_edge = 1;
		}
				
		/* Clear EXTI1 interrupt pending flag */
		EXTI->PR |= ((uint32_t)0x2);

	}

}


#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
