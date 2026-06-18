/*
 * nokia1661_Hw.h
 *
 *  Created on: Sep 11, 2019
 *      Author: zeus
 */

#ifndef NOKIA1661_HW_H_
#define NOKIA1661_HW_H_

//#define LCD_AVR_HW		1
#define LCD_STM_HW		1

#if defined(LCD_AVR_HW) && defined(LCD_STM_HW)
#error Only Select One Hardware AVR/STM
#endif
#if !defined(LCD_AVR_HW) && !defined(LCD_STM_HW)
#error You need to define Hardware Type for LCD usage
#endif

#ifdef LCD_AVR_HW

#define LCD_PORT		PORTB
#define LCD_PIN			PINB
#define LCD_DDR			DDRB

#define LCD_RST			PINB1
#define LCD_CS			PINB2
#define LCD_SDA			PINB3
#define LCD_CLK			PINB5

#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>

#define SBI(port,bit) 	asm("sbi %0, %1" : : "I" (_SFR_IO_ADDR(port)), "I" (bit))
#define CBI(port,bit) 	asm("cbi %0, %1" : : "I" (_SFR_IO_ADDR(port)), "I" (bit))
#define LCD_RSDA()		(LCD_PIN&(1 << LCD_SDA))

#define _rst_set()	SBI(LCD_PORT,LCD_RST)
#define _rst_clr()	CBI(LCD_PORT,LCD_RST)
#define _cs_set()	SBI(LCD_PORT,LCD_CS)
#define _cs_clr()	CBI(LCD_PORT,LCD_CS)
#define _sda_set()	SBI(LCD_PORT,LCD_SDA)
#define _sda_clr()	CBI(LCD_PORT,LCD_SDA)
#define _clk_set()	SBI(LCD_PORT,LCD_CLK)
#define _clk_clr()	CBI(LCD_PORT,LCD_CLK)
#define _Init_GPIO()	LCD_DDR |= (1 << LCD_RST) | (1 << LCD_CS) | (1 << LCD_SDA) | (1 << LCD_CLK)

#elif defined(LCD_STM_HW)

#include "stm32f4xx.h"
#include <string.h>

#define LCD_PORT            GPIOB
#define LCD_GPIO_Enable()   (RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN)

#define LCD_RST             15
#define LCD_CS              14
#define LCD_SDA             13
#define LCD_CLK             12

#define LCD_RSDA()          ((LCD_PORT->IDR & (1U << LCD_SDA)) ? 1 : 0)

#define SBI(port,bit)       ((port)->BSRR = (1U << (bit)))
#define CBI(port,bit)       ((port)->BSRR = (1U << ((bit)+16)))

#define memcpy_P            memcpy
#define PROGMEM
#define PSTR(x)             (x)
#define pgm_read_byte(x)    (*(x))
#define _delay_ms(x)        delay_ms(x)

#define _rst_set()          SBI(LCD_PORT,LCD_RST)
#define _rst_clr()          CBI(LCD_PORT,LCD_RST)

#define _cs_set()           SBI(LCD_PORT,LCD_CS)
#define _cs_clr()           CBI(LCD_PORT,LCD_CS)

#define _sda_set()          SBI(LCD_PORT,LCD_SDA)
#define _sda_clr()          CBI(LCD_PORT,LCD_SDA)

#define _clk_set()          SBI(LCD_PORT,LCD_CLK)
#define _clk_clr()          CBI(LCD_PORT,LCD_CLK)

/* Configure pin as general-purpose output */
#define GPIO_OUT(pin)                                   \
do {                                                    \
    LCD_PORT->MODER &= ~(3U << ((pin)*2));              \
    LCD_PORT->MODER |=  (1U << ((pin)*2));              \
                                                        \
    LCD_PORT->OTYPER &= ~(1U << (pin));                 \
                                                        \
    LCD_PORT->OSPEEDR |= (3U << ((pin)*2));             \
                                                        \
    LCD_PORT->PUPDR &= ~(3U << ((pin)*2));              \
} while(0)

/* Configure pin as input */
#define GPIO_IN(pin)                                    \
do {                                                    \
    LCD_PORT->MODER &= ~(3U << ((pin)*2));              \
    LCD_PORT->PUPDR &= ~(3U << ((pin)*2));              \
} while(0)

#define _rst_Out()      GPIO_OUT(LCD_RST)
#define _cs_Out()       GPIO_OUT(LCD_CS)

#define _sda_Out()      GPIO_OUT(LCD_SDA)
#define _sda_In()       GPIO_IN(LCD_SDA)

#define _clk_Out()      GPIO_OUT(LCD_CLK)

#define _Init_GPIO()                                    \
do {                                                    \
    _rst_Out();                                         \
    _cs_Out();                                          \
    _sda_Out();                                         \
    _clk_Out();                                         \
} while(0)

#endif


#endif /* NOKIA1661_HW_H_ */
