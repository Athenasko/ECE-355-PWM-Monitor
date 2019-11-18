#include <stdio.h>
#include "diag/Trace.h"
#include "cmsis/cmsis_device.h"
/* DONE */
#include "gpio.h"
#include "spi_lcd.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

/* DONE */
void myEXTI_Init(){
	/* Map EXTI1 line to PA1 */
	SYSCFG->EXTICR[0] |= ((uint16_t)0x0000);

	/* EXTI1 line interrupts: set rising-edge trigger */
	EXTI->RTSR |= ((uint32_t)0x00000002);

	/* Unmask interrupts from EXTI1 line */
	EXTI->IMR |= ((uint32_t)0x00000002);

	/* Assign EXTI1 interrupt priority = 0 in NVIC */
	NVIC_SetPriority(EXTI0_1_IRQn, 0);

	/* Enable EXTI1 interrupts in NVIC */
	NVIC_EnableIRQ(EXTI0_1_IRQn);
}

/* DONE */
void myADC_Init(){
	/* Enable the clock for ADC peripheral*/
	RCC->APB2ENR |= RCC_APB2ENR_ADCEN;

	/* Calibrate ADC */
	trace_printf("Start ADC calibration\n");
	ADC1->CR |= ADC_CR_ADCAL;
	while((ADC1->CR & ADC_CR_ADCAL) == 1){};
	trace_printf("Finished ADC calibration\n\n");

	/* Configure Continuous conversion on Channel 2 */
	ADC1->CFGR1 |= ADC_CFGR1_CONT;
	ADC1->CHSELR |= ADC_CHSELR_CHSEL2;

	/* Enable ADC */
	trace_printf("Start enable ADC\n");
	ADC1->CR |= ADC_CR_ADEN;
	while((ADC1->ISR & ADC_ISR_ADRDY) == 0){};
	trace_printf("Finished enable ADC\n\n");

}

/* DONE */
void myDAC_Init(){
	/* Enable the clock for DAC peripheral*/
	RCC->APB1ENR |= RCC_APB1ENR_DACEN;

	/* Enable DAC*/
	DAC->CR |= DAC_CR_EN1;
}

/* DONE */
void myTIM3_Init(){
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;

  	/* Set TIM3 to auto-reload, count up, stop on overflow, EUI, and interrupt on underflow only. */
  	TIM3->CR1 = ((uint16_t)0x008C);

  	/* Set prescaler */
  	TIM3->PSC = myTIM3_PRESCALER;

  	/* Set auto reload delay */
  	TIM3->ARR = myTIM3_PERIOD;

  	/* Update timer registers */
  	TIM3->EGR = ((uint16_t)0x0001);

  	/* Assign TIM3 interrupt priority = 1 in NVIC */
  	NVIC_SetPriority(TIM3_IRQn, 1);

  	/* Enable TIM3 interrupts in NVIC */
  	NVIC_EnableIRQ(TIM3_IRQn);

  	/* Enable update interrupt generation */
  	TIM3->DIER |= TIM_DIER_UIE;

  	/* Start timer */
  	TIM3->CR1 |= TIM_CR1_CEN;
}


int main(int argc, char* argv[]){
	trace_printf("Final Project by Aman Nijjar, Austin Lee, and Emmanuel\n");
	trace_printf("System clock: %u Hz\n", SystemCoreClock);

	myGPIO_Init(); /* Init ports A/B for POT, ADC, DAC */
	myEXTI_Init(); /* Init EXTI to trigger on NE555 input */
	myADC_Init();  /* Init ADC for continuous POT input */
	myDAC_Init();  /* Init DAC for timer voltage */
	myTIM2_Init(); /* Init TIM2 for NE555 signal */
	myLCD_Init();  /* Init LCD for display */
	myTIM3_Init(); /* Init TIM3 for LCD display */

	LCD_Clear();

	//ADC1->CR |= ADC_CR_ADSTART;
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
