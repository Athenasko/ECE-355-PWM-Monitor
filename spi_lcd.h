#ifndef __SPI_LCD_H
#define __SPI_LCD_H

/* Initializes LCD display. */
void myLCD_Init(void);

/* Initializes GPIOB pins. */
void myGPIOB_Init(void);

/* Initializes TIM6 counter. */
void myTIM6_Init(void);

/* Initializes SPI. */
void mySPI_Init(void);

/* Delays for specified amount of ms. */
void Delay(uint32_t time)

/* Write to SPI. Toggles LCK to ensure register values only send to LCD when fully loaded. */
void SPI_Write(uint8_t word);

/* Writes data to LCD. */
void LCD_Data(uint8_t type, uint8_t word);

/* Writes a single 8 bit (1 byte) character to the LCD. */
void LCD_Char(char ch);

/* Displays a string on the LCD. */
void LCD_Word(char *s);

/* Sends an 8 bit command to the LCD. */
void LCD_Command(uint8_t data);

/* Clears the LCD and wait for 2ms. */
void LCD_Clear();

/* Writes strings to both lines of the LCD. */
void Write_Both_Lines(char *first_line, char *second_line);