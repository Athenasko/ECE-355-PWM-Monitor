#include <stdio.h>
#include "diag/Trace.h"
#include "cmsis/cmsis_device.h"
#include "gpio.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"



// Your global variables...
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

}

void myTIM2_Init()
{

}

int main(int argc, char* argv[])
{

	InitGPIO();
	myEXTI_Init();		/* Initialize EXTI */
	myADC_Init();
	myDAC_Init();
	myTIM2_Init();

	//ADC->CR |= ADC_CR_ADSTART;
	while (1)
	{
		// Nothing is going on here...
	}

	return 0;

}


/* This handler is declared in system/src/cmsis/vectors_stm32f0xx.c */
void EXTI0_1_IRQHandler()
{

	/* Check if EXTI1 interrupt pending flag is indeed set */
	if ((EXTI->PR & EXTI_PR_PR1) != 0)
	{

	}
	//Clear EXTI1 interrupt pending flag
	EXTI->PR |= ((uint32_t)0x2);

}


#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
