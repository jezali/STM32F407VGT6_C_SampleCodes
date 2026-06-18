#ifndef _ALCD_H_
#define _ALCD_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define ALCD_Delay_us(US)                       __ALCD_Delay_us((US))
#define ALCD_Delay_ms(MS)                       __ALCD_Delay_ms((MS))

#define ALCD_RS_GPIO                            GPIOD
#define ALCD_RS_PIN                             GPIO_PIN_11

#define ALCD_RW_GPIO                            GPIOD
#define ALCD_RW_PIN                             GPIO_PIN_10

#define ALCD_EN_GPIO                            GPIOD
#define ALCD_EN_PIN                             GPIO_PIN_7

#define ALCD_D4_GPIO                            GPIOD
#define ALCD_D4_PIN                             GPIO_PIN_15

#define ALCD_D5_GPIO                            GPIOD
#define ALCD_D5_PIN                             GPIO_PIN_14

#define ALCD_D6_GPIO                            GPIOD
#define ALCD_D6_PIN                             GPIO_PIN_13

#define ALCD_D7_GPIO                            GPIOD
#define ALCD_D7_PIN                             GPIO_PIN_12

#define ALCD_SUPPORT_PUTNUM                     1
#define ALCD_SUPPORT_PRINTF                     1

#define ALCD_PRINTF_BUF_SIZE                    40

/* USER CODE BEGIN PFP */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

#define ALCD_CLK_ENABLE()                       __HAL_RCC_GPIOD_CLK_ENABLE()

/* USER CODE END PM */

// SoftwareDelay
void __ALCD_Delay_us(uint32_t us);
void __ALCD_Delay_ms(uint32_t ms);

void ALCD_init(uint8_t col, uint8_t row);

void ALCD_clear();
void ALCD_goto(uint8_t x, uint8_t y);
void ALCD_clearLocation(uint8_t x, uint8_t y);
void ALCD_clearSection(int8_t x0, int8_t y0, int8_t x1, int8_t y1);

void ALCD_putChar(char c);
void ALCD_puts(const char* str);

void ALCD_putCharXY(uint8_t x, uint8_t y, char c);
void ALCD_putsXY(uint8_t x, uint8_t y, const char* str);

#if ALCD_SUPPORT_PUTNUM
    void ALCD_putNum(uint32_t num);
    void ALCD_putNumXY(uint8_t x, uint8_t y, float num);
#endif
#if ALCD_SUPPORT_PRINTF
    void ALCD_printf(const char* fmt, ...);
    void ALCD_printfXY(uint8_t x, uint8_t y, const char* fmt, ...);
#endif


/* USER CODE END PFP */



#ifdef __cplusplus
};
#endif

#endif

