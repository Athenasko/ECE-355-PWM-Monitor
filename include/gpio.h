#ifndef __GPIO_H
#define __GPIO_H

#define MODER_NE555_TIMER 	GPIO_MODER_MODER1  //PA1
#define MODER_POT 			GPIO_MODER_MODER2  //PA2
#define MODER_OPTO_4N35	 	GPIO_MODER_MODER4  //PA4
#define MODER_SPI2_NSS 		GPIO_MODER_MODER12 //PB12
#define MODER_SPI2_SCK 		GPIO_MODER_MODER13 //PB13
#define MODER_SPI2_MISO	 	GPIO_MODER_MODER14 //PB14
#define MODER_SPI2_MOSI	 	GPIO_MODER_MODER15 //PB15

#define PUPDR_NE555_TIMER 	GPIO_PUPDR_PUPDR1
#define PUPDR_POT 			GPIO_PUPDR_PUPDR2
#define PUPDR_OPTO_4N35	 	GPIO_PUPDR_PUPDR4
#define PUPDR_SPI2_NSS 		GPIO_PUPDR_PUPDR12
#define PUPDR_SPI2_SCK 		GPIO_PUPDR_PUPDR13
#define PUPDR_SPI2_MISO	 	GPIO_PUPDR_PUPDR14
#define PUPDR_SPI2_MOSI	 	GPIO_PUPDR_PUPDR15
#define OSPEEDR_NE555_TIMER	GPIO_OSPEEDR_OSPEEDR1
#define OSPEEDR_POT			GPIO_OSPEEDR_OSPEEDR2
#define OSPEEDR_OPTO_4N35	GPIO_OSPEEDR_OSPEEDR4
#define OSPEEDR_SPI2_NSS 	GPIO_OSPEEDR_OSPEEDR12
#define OSPEEDR_SPI2_SCK 	GPIO_OSPEEDR_OSPEEDR13
#define OSPEEDR_SPI2_MISO	GPIO_OSPEEDR_OSPEEDR14
#define OSPEEDR_SPI2_MOSI	GPIO_OSPEEDR_OSPEEDR15
#define OTYPER_OPTO_4N35	GPIO_OTYPER_OT_4
#define OTYPER_SPI2_NSS 	GPIO_OTYPER_OT_12
#define OTYPER_SPI2_SCK 	GPIO_OTYPER_OT_13
#define OTYPER_SPI2_MISO	GPIO_OTYPER_OT_14
#define OTYPER_SPI2_MOSI	GPIO_OTYPER_OT_15

extern void InitGPIO();

#endif /* __GPIO_H */
