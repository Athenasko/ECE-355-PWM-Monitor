#include "stm32f0xx.h"
#include "gpio.h"

extern void InitGPIO(){
	myGPIOA_Init(); /* Initialize I/O ports for NE555, POT, and 4N35 */
	myGPIOB_Init(); /* Initialize I/O SPI interface*/
}

static void myGPIOA_Init()
{
	/* Enable clock for GPIOA peripherals */
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;

	/* Configure PA1 and PA2 as analog inputs for NE555 and POT */
	GPIOA->MODER |= MODER_NE555_TIMER | MODER_POT;
	/* Ensure no pull-up/pull-down for NE555 and POT */
	GPIOA->PUPDR &= ~(PUPDR_NE555_TIMER | PUPDR_POT);
	/* Ensure high-speed mode for NE555 and POT */
	GPIOA->OSPEEDR |= OSPEEDR_NE555_TIMER | OSPEEDR_POT;

	/* Configure PA4 as output for the 4N35 */
	GPIOA->MODER |= MODER_OPTO_4N35;
	/* Ensure no pull-up/pull-down for 4N35 */
	GPIOA->PUPDR &= ~PUPDR_OPTO_4N35;
	/* Ensure push-pull mode selected for 4N35*/
	GPIOA->OTYPER &= ~GPIO_OTYPER_OT_4;

	/* Ensure high-speed mode for 4N35 */
	GPIOA->OSPEEDR |= OSPEEDR_OPTO_4N35;

}

static void myGPIOB_Init()
{
	/* Enable clock for GPIOB peripherals */
	RCC->AHBENR |= RCC_AHBENR_GPIOBEN;

	/* Configure PB12/PB13/PB14/PB15 as output for SPI2 */
	GPIOB->MODER |= MODER_SPI2_NSS | MODER_SPI2_SCK | MODER_SPI2_MISO | MODER_SPI2_MOSI;
	/* Ensure no pull-up/pull-down for SPI2 */
	GPIOB->PUPDR &= ~(PUPDR_SPI2_NSS | PUPDR_SPI2_SCK | PUPDR_SPI2_MISO | PUPDR_SPI2_MOSI);
	/* Ensure push-pull mode selected for SPI2*/
	GPIOB->OTYPER &= ~(OTYPER_SPI2_NSS | OTYPER_SPI2_SCK | OTYPER_SPI2_MISO | OTYPER_SPI2_MOSI);
	/* Ensure low-speed mode for SPI2 */
	GPIOB->OSPEEDR &= ~(OTYPER_SPI2_NSS | OTYPER_SPI2_SCK | OTYPER_SPI2_MISO | OTYPER_SPI2_MOSI);
}
