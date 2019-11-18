#include <stdio.h>
#include "stm32f0xx_gpio.h"
#include "stm32f0xx_rcc.h"
#include "stm32f0xx_spi.h"
#include "cmsis/cmsis_device.h"
#include "diag/Trace.h"
#include "spi_lcd.h"

/* Maps to EN and RS bits for LCD. */
#define LCD_ENABLE (0x80)
#define LCD_DISABLE (0x0)
#define LCD_COMMAND (0x0)
#define LCD_DATA (0x40)

/* LCD Config commands. */
#define LCD_OUTPUT_SET (0x28)   /* Sets display to 4bits, 2 lines, 5x7 font. */
#define LCD_NO_CURSOR (0x0C)    /* Turns off cursor and cursor blinking. */
#define LCD_AUTO_INC (0x06)     /* Sets cursor to auto-increment with no display shifting. */
#define LCD_CLEAR_CMD (0x1)     /* Clears the LCD Display of text. */
#define LCD_RESET_CURSOR (0x2)  /* Resets the cursor to the top leftmost LCD. */
#define LCD_FIRST_LINE (0x80)   /* Moves the cursor to the first line. */
#define LCD_SECOND_LINE (0xC0)  /* Moves the cursor to the second line. */

/* TIM6 Configuration. */
#define myTIM6_PRESCALE ((uint16_t)((SystemCoreClock - 1) / 1000)) /* 1ms per tick */
#define myTIM6_PERIOD_DEFAULT (100) /* In ms */

/* Initializes LCD display. */
void myLCD_Init(void){
    
    myGPIOB_Init(); /* Initialize GPIOB to control LCD via SPI. */
    myTIM6_Init();  /* Initialize TIM6 for Delay purposes. */
    mySPI_Init();   /* Initialize SPI. */

    /* If in 8bit mode, switches to 4bit. If in 4bit, resets cursor position */
    LCD_COMMAND(LCD_RESET_CURSOR);
    Delay(2);       /* Ensures mode switch/reset has time to execute. */

    /* Configure LCD Display. */
    LCD_COMMAND(LCD_OUTPUT_SET);
    LCD_COMMAND(LCD_NO_CURSOR;
    LCD_COMMAND(LCD_AUTO_INC);

    LCD_Clear();
}


/* Initializes GPIOB pins. */
void myGPIOB_Init(void){
    /* Enable clock for GPIOB peripheral. */
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN;

    GPIO_InitTypeDef GPIO_InitStruct;

    // PB3 --AF0-> SPI MOSI
    // PB5 --AF0-> SPI SCK

    /* Configure PB3 and PB5 pins. */
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_5;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* Configure LCK pin. */
    GPIO_InitStruct.GPIO_Pin = LCD_LCK_PIN;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOB, &GPIO_InitStruct);
}


/* Initializes TIM6 counter. */
void myTIM6_Init(void) {
    /* Enable clock for TIM6 peripheral. */
    RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;

  /*
   * Settings for TIM6
         *
   * bit 7: 	Auto reload preload enable = 1
   * bit 6-5: Center-aligned mode selection = 00
   * bit 4:		Direction = 0 - up counter
   * bit 3:		One-pulse mode = 0
   * bit 2:		Update request source = 1 - only counter overflow
generates an update interrupt
   * bit 1:		Update disable = 0 - UEV enabled. The UEV event is
generated
   * bit 0:		Counter enable = 0 - counter disabled
   */
  //  10000100
    
    TIM6->CR1 = 0x84;
    /* Set clock prescaler value. */
    TIM6->PSC = myTIM6_PRESCALE;
    /* Set auto-reloaded delay. */
    TIM6->ARR = myTIM6_PERIOD_DEFAULT;
    /* Update timer registers. */
    TIM6->EGR = ((uint16_t)0x0001);
}


/* Initializes SPI. */
void mySPI_Init(void){
    /* Enable Clock for SPI Peripheral. */
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;

    SPI_InitTypeDef SPI_InitStruct;

    SPI_InitStruct.SPI_Direction = SPI_Direction_1Line_Tx;
    SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
    SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;
    SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
    SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStruct.SPI_CRCPolynomial = 7;

    SPI_Init(SPI1, &SPI_InitStruct);
    SPI_Cmd(SPI1, ENABLE);
}


/* Delays for specified amount of ms. */
void Delay(uint32_t time) {
    /* Clear timer. */
    TIM6->CNT = (uint32_t)0x0;

    /* Set timeout. */
    TIM6->ARR = time;
    /* Update timer registers. */
    TIM6->EGR |= 0x0001;
    /* Start the timer. */
    TIM6->CR1 |= TIM_CR1_CEN;

    /* Wait until interrupt occurs. */
    while ((TIM6->SR & TIM_SR_UIF) == 0){
        /* Waiting... */
    };

    /* Stop timer. */
    TIM6->CR1 &= ~(TIM_CR1_CEN);
    /* Reset the interrupt flag. */
    TIM6->SR &= ~(TIM_SR_UIF);
}


/* Write to SPI. Toggles LCK to ensure register values only send to LCD when fully loaded. */
void SPI_Write(uint8_t word){
    // LCK = 0
    GPIOB->BRR = LCD_LCK_PIN;

    /* Wait until it is ready to send -> TXE = 1 | BSY = 0 */
    while ((SPI1->SR & SPI_SR_BSY) && ~(SPI1->SR & SPI_SR_TXE)){
        /* Waiting... */
    };

    /* Send the data */
    SPI_SendData8(SPI1, word);

    /* Wait SPI until it has sent everything -> BSY = 0 */
    while (SPI1->SR & SPI_SR_BSY){
        /* Waiting... */
    };

    /* LCK = 1 */
    GPIOB->BSRR = LCD_LCK_PIN;
}


/* Writes data to LCD. */
void LCD_Data(uint8_t type, uint8_t word){
    /* The high half of the register output is always reserved for EN and RS.
     * To send an 8b target word we have to send it as two sequential 4b
     * half-words, with the target half-words in the lower half of the word. */

    uint8_t high = ((word & 0xF0) >> 4);
    uint8_t low = word & 0x0F;

    SPI_Write(LCD_DISABLE | type | high);
    SPI_Write(LCD_ENABLE  | type | high);
    SPI_Write(LCD_DISABLE | type | high);

    SPI_Write(LCD_DISABLE | type | low);
    SPI_Write(LCD_ENABLE  | type | low);
    SPI_Write(LCD_DISABLE | type | low);
}


/* Writes a single 8 bit (1 byte) character to the LCD. */
void LCD_Char(char ch){ 
    LCD_Data(LCD_CHAR, (uint8_t)(ch)); 
}


/* Displays a string on the LCD. */
void LCD_Word(char *s){
    char *ch = s;
    int count = 0;
    while (*ch != NULL) {
        LCD_Char(*ch);
        ch++;

        count++;
        if (count > 8)
            break; /* Stop if word is longer than 8 chars. */
    }
}


/* Sends an 8 bit command to the LCD. */
void LCD_Command(uint8_t data){
    LCD_Data(LCD_CMD, data);
}


/* Clears the LCD and wait for 2ms. */
void LCD_Clear(){
    /* Send the clear command. */
    LCD_Command(LCD_CLEAR_CMD);

    /* Wait 2ms for command to complete. */
    Delay(2);
}


/* Writes strings to both lines of the LCD. */
void Write_Both_Lines(char *first_line, char *second_line){
    /* Write the first line. */
    LCD_Command(LCD_FIRST_LINE);
    LCD_Word(first_line);

    /* Write the second line. */
    LCD_Command(LCD_SECOND_LINE);
    LCD_Word(second_line);
}