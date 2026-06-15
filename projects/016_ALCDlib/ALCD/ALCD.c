#include <ALCD.h>
#include <main.h>

#if ALCD_SUPPORT_PUTNUM || ALCD_SUPPORT_PRINTF
  #include <stdio.h>
  #include <string.h>
  #include <math.h>
#endif
#if ALCD_SUPPORT_PRINTF
  #include <stdarg.h>
#endif

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

typedef enum {
    ALCD_RS_State_Command                 = 0,
    ALCD_RS_State_Data                    = 1,
  } ALCD_RS_State;
typedef union {
    uint8_t                               Value;
    struct {
        uint8_t                             D0  : 1;
        uint8_t                             D1  : 1;
        uint8_t                             D2  : 1;
        uint8_t                             D3  : 1;
        uint8_t                             D4  : 1;
        uint8_t                             D5  : 1;
        uint8_t                             D6  : 1;
        uint8_t                             D7  : 1;
    };
} ALCD_Byte;
typedef struct {
    int8_t                                X;
    int8_t                                Y;
    int8_t                                Rows;
    int8_t                                Columns;

} ALCD_Handle;

/* USER CODE END PTD */

// ALCD COMMANDS -------------------------------------------------------------*

#define ALCD_EN_HIGH_TIME                       20      //us
#define ALCD_EN_LOW_TIME                        20      //us

#define ALCD_CMD_CLEAR_DISPLAY                  0x01
#define ALCD_CMD_CLEAR_DISPLAY_TIMEOUT          2000    //us

#define ALCD_CMD_RETURN_HOME                    0x02
#define ALCD_CMD_RETURN_HOME_TIMEOUT            2000    //us

#define ALCD_CMD_ENTRY_MODE_SET                 0x04
#define ALCD_CMD_ENTRY_MODE_SET_TIMEOUT         50      //us

#define ALCD_CMD_DISPLAY_CONTROL                0x08
#define ALCD_CMD_DISPLAY_CONTROL_TIMEOUT        50      //us

#define ALCD_CMD_SHIFT                          0x10
#define ALCD_CMD_SHIFT_TIMEOUT                  50      //us

#define ALCD_CMD_FUNCTION_SET                   0x20
#define ALCD_CMD_FUNCTION_SET_TIMEOUT           50      //us

#define ALCD_CMD_SET_CGRAM                      0x40
#define ALCD_CMD_SET_CGRAM_TIMEOUT              50      //us

#define ALCD_CMD_SET_DDRAM                      0x80
#define ALCD_CMD_SET_DDRAM_TIMEOUT              50      //us

#define ALCD_CMD_WRITE_DATA                     50      //us



// ALCD FLAGS ----------------------------------------------------------------*

#define ALCD_FLAG_ENTRY_MODE_INCREMENT          0x02
#define ALCD_FLAG_ENTRY_MODE_DECREMENT          0x00
#define ALCD_FLAG_ENTRY_MODE_SHIFT              0x01

#define ALCD_FLAG_DISPLAY_CONTROL_ON            0x04
#define ALCD_FLAG_DISPLAY_CONTROL_OFF           0x00
#define ALCD_FLAG_DISPLAY_CONTROL_CURSOR_ON     0x02
#define ALCD_FLAG_DISPLAY_CONTROL_CURSOR_OFF    0x00
#define ALCD_FLAG_DISPLAY_CONTROL_BLINK_ON      0x01
#define ALCD_FLAG_DISPLAY_CONTROL_BLINK_OFF     0x00

#define ALCD_FLAG_FUNCTION_SET_8Bit             0x10
#define ALCD_FLAG_FUNCTION_SET_4Bit             0x00
#define ALCD_FLAG_FUNCTION_SET_2Line            0x08
#define ALCD_FLAG_FUNCTION_SET_1Line            0x00
#define ALCD_FLAG_FUNCTION_SET_5x10_DOT         0x04
#define ALCD_FLAG_FUNCTION_SET_5x8_DOT          0x00

/* USER CODE END PD */

/* USER CODE BEGIN PV */

static void ALCD_initPins(void);
static void ALCD_write(ALCD_RS_State rs, uint8_t val, uint32_t timeout);
static void ALCD_write4Bit(ALCD_RS_State rs, uint8_t val);
static void ALCD_trigger(void);

static ALCD_Handle alcdHandle = { 0 };
static uint32_t oldNum = 0;
static uint32_t oldLen = 0;

/* USER CODE END PV */

/* USER CODE BEGIN 4 */

// SoftwareDelay--------------------------------------------------------------*

void __ALCD_Delay_us(uint32_t us) {
  us = SystemCoreClock / 3000000 * us;

  __asm volatile (
    "1:                 \n"     // Label
    "subs %0, %0, #1    \n"     // us--
    "bne 1b             \n"     // Back to loop
    : "+r" (us)
    :
    : "cc"
  );
}
void __ALCD_Delay_ms(uint32_t ms) {
  ms = SystemCoreClock / 3000 * ms;

  __asm volatile (
    "1:                 \n"     // Label
    "subs %0, %0, #1    \n"     // ms--
    "bne 1b             \n"     // Back to loop
    : "+r" (ms)
    :
    : "cc"
  );
}

// End of SoftwareDelay-------------------------------------------------------*

void ALCD_init(uint8_t col, uint8_t row) {

  ALCD_initPins();

  // wait 40ms for rise up
  ALCD_Delay_ms(40);
  // Send 4Bit Command: 8Bit Interface
  ALCD_write4Bit(
    ALCD_RS_State_Command,
    ALCD_CMD_FUNCTION_SET | ALCD_FLAG_FUNCTION_SET_8Bit
  );
  // wait 5ms
  ALCD_Delay_ms(5);
  // Send 4Bit Command: 8Bit Interface
  ALCD_write4Bit(
    ALCD_RS_State_Command,
    ALCD_CMD_FUNCTION_SET | ALCD_FLAG_FUNCTION_SET_8Bit
  );
  // wait 100us
  ALCD_Delay_us(100);
  // Send 4Bit Command: 8Bit Interface
  ALCD_write4Bit(
    ALCD_RS_State_Command,
    ALCD_CMD_FUNCTION_SET | ALCD_FLAG_FUNCTION_SET_8Bit
  );
  // Send 4Bit Command: 4Bit Interface
  ALCD_write4Bit(
    ALCD_RS_State_Command,
    ALCD_CMD_FUNCTION_SET | ALCD_FLAG_FUNCTION_SET_4Bit
  );

  // Send Command: 4Bit Interface, 2 Line, 5x8 dot
  ALCD_write(
    ALCD_RS_State_Command,
    ALCD_CMD_FUNCTION_SET | ALCD_FLAG_FUNCTION_SET_4Bit |
    ALCD_FLAG_FUNCTION_SET_2Line | ALCD_FLAG_FUNCTION_SET_5x8_DOT,
    ALCD_CMD_FUNCTION_SET_TIMEOUT
  );
  // Send Command: Display off
  ALCD_write(
    ALCD_RS_State_Command,
    ALCD_CMD_DISPLAY_CONTROL | ALCD_FLAG_DISPLAY_CONTROL_OFF,
    ALCD_CMD_DISPLAY_CONTROL_TIMEOUT
  );
  // Send Command: Display clear
  ALCD_write(
    ALCD_RS_State_Command,
    ALCD_CMD_CLEAR_DISPLAY,
    ALCD_CMD_CLEAR_DISPLAY_TIMEOUT
  );

  // Display on
  ALCD_write(
    ALCD_RS_State_Command,
    ALCD_CMD_DISPLAY_CONTROL | ALCD_FLAG_DISPLAY_CONTROL_ON |
    ALCD_FLAG_DISPLAY_CONTROL_CURSOR_OFF | ALCD_FLAG_DISPLAY_CONTROL_BLINK_OFF,
    ALCD_CMD_DISPLAY_CONTROL_TIMEOUT
  );

  alcdHandle.Rows = row;
  alcdHandle.Columns = col;
  alcdHandle.X = 0;
  alcdHandle.Y = 0;

}
static void ALCD_initPins(void) {

  GPIO_InitTypeDef init;

  //Clock Enable
  ALCD_CLK_ENABLE();

  init.Mode = GPIO_MODE_OUTPUT_PP;
  init.Pull = GPIO_NOPULL;
  init.Speed = GPIO_SPEED_HIGH;
  // init RS
  init.Pin = ALCD_RS_PIN;
  HAL_GPIO_Init(ALCD_RS_GPIO, &init);
  // init RW
  init.Pin = ALCD_RW_PIN;
  HAL_GPIO_Init(ALCD_RW_GPIO, &init);
  // init EN
  init.Pin = ALCD_EN_PIN;
  HAL_GPIO_Init(ALCD_EN_GPIO, &init);
  // init D4
  init.Pin = ALCD_D4_PIN;
  HAL_GPIO_Init(ALCD_D4_GPIO, &init);
  // init D5
  init.Pin = ALCD_D5_PIN;
  HAL_GPIO_Init(ALCD_D5_GPIO, &init);
  // init D6
  init.Pin = ALCD_D6_PIN;
  HAL_GPIO_Init(ALCD_D6_GPIO, &init);
  // init D7
  init.Pin = ALCD_D7_PIN;
  HAL_GPIO_Init(ALCD_D7_GPIO, &init);

}
static void ALCD_write(ALCD_RS_State rs, uint8_t val, uint32_t timeout) {

  ALCD_Byte byte =  { val };
  //RS LOW
  HAL_GPIO_WritePin(ALCD_RS_GPIO, ALCD_RS_PIN, (GPIO_PinState) rs);
  //RW LOW
  HAL_GPIO_WritePin(ALCD_RW_GPIO, ALCD_RW_PIN, GPIO_PIN_RESET);
  //Write High Nibble (Upper 4x Bit)
  HAL_GPIO_WritePin(ALCD_D7_GPIO, ALCD_D7_PIN, byte.D7);
  HAL_GPIO_WritePin(ALCD_D6_GPIO, ALCD_D6_PIN, byte.D6);
  HAL_GPIO_WritePin(ALCD_D5_GPIO, ALCD_D5_PIN, byte.D5);
  HAL_GPIO_WritePin(ALCD_D4_GPIO, ALCD_D4_PIN, byte.D4);
  // Trigger
  ALCD_trigger();
  //Write Low Nibble (Upper 4x Bit)
  HAL_GPIO_WritePin(ALCD_D7_GPIO, ALCD_D7_PIN, byte.D3);
  HAL_GPIO_WritePin(ALCD_D6_GPIO, ALCD_D6_PIN, byte.D2);
  HAL_GPIO_WritePin(ALCD_D5_GPIO, ALCD_D5_PIN, byte.D1);
  HAL_GPIO_WritePin(ALCD_D4_GPIO, ALCD_D4_PIN, byte.D0);
  // Trigger
  ALCD_trigger();
  // Wait
  ALCD_Delay_us(timeout);

}
static void ALCD_write4Bit(ALCD_RS_State rs, uint8_t val) {

  ALCD_Byte byte =  { val };
  //RS LOW
  HAL_GPIO_WritePin(ALCD_RS_GPIO, ALCD_RS_PIN, GPIO_PIN_RESET);
  //RW LOW
  HAL_GPIO_WritePin(ALCD_RW_GPIO, ALCD_RW_PIN, GPIO_PIN_RESET);
  //Write High Nibble (Upper 4x Bit)
  HAL_GPIO_WritePin(ALCD_D7_GPIO, ALCD_D7_PIN, byte.D7);
  HAL_GPIO_WritePin(ALCD_D6_GPIO, ALCD_D6_PIN, byte.D6);
  HAL_GPIO_WritePin(ALCD_D5_GPIO, ALCD_D5_PIN, byte.D5);
  HAL_GPIO_WritePin(ALCD_D4_GPIO, ALCD_D4_PIN, byte.D4);
  // Trigger
  ALCD_trigger();

}
static void ALCD_trigger(void) {

  // SET HIGH
  HAL_GPIO_WritePin(ALCD_EN_GPIO, ALCD_EN_PIN, GPIO_PIN_SET);
  ALCD_Delay_us(ALCD_EN_HIGH_TIME);
  // SET LOW
  HAL_GPIO_WritePin(ALCD_EN_GPIO, ALCD_EN_PIN, GPIO_PIN_RESET);
  ALCD_Delay_us(ALCD_EN_LOW_TIME);

}

void ALCD_clear() {

  ALCD_write(
    ALCD_RS_State_Command,
    ALCD_CMD_CLEAR_DISPLAY,
    ALCD_CMD_CLEAR_DISPLAY_TIMEOUT
  );

}
void ALCD_goto(uint8_t x, uint8_t y) {

  static const uint8_t LINE_ADDR[4] = {
    0x00, 0x40, 0x20, 0x60
  };

  if (x >= alcdHandle.Columns) {
    x = 0;
    y++;
  }
  if (y >= alcdHandle.Rows) {
    y = 0;
  }

  ALCD_write(
    ALCD_RS_State_Command,
    ALCD_CMD_SET_DDRAM | (LINE_ADDR[y] + x),
    ALCD_CMD_SET_DDRAM_TIMEOUT
  );
  alcdHandle.X = x;
  alcdHandle.Y = y;

}
void ALCD_clearLocation(uint8_t x, uint8_t y) {
  ALCD_putsXY(x, y, " ");
}
void ALCD_clearSection(int8_t x0, int8_t y0, int8_t x1, int8_t y1) {

  if (y0 == y1) {
    while (x1 >= x0) {
      ALCD_clearLocation(x1, y0);
      x1--;
    }
  } else if (y1 != y0) {
    while (x1 >= 0) {
      ALCD_clearLocation(x1, y1);
      x1--;
    } while (x0 <= 15) {
      ALCD_clearLocation(x0, y0);
      x0++;
    }
  }
}

void ALCD_putChar(char c) {

  ALCD_write(ALCD_RS_State_Data, c, ALCD_CMD_WRITE_DATA);

  if (++alcdHandle.X >= alcdHandle.Columns) {
    alcdHandle.X = 0;
    if (++alcdHandle.Y >= alcdHandle.Rows) {
      alcdHandle.Y = 0;
    }
    ALCD_goto(alcdHandle.X, alcdHandle.Y);
  }
}
void ALCD_putCharXY(uint8_t x, uint8_t y, char c) {
  ALCD_goto(x, y);
  ALCD_putChar(c);
}

void ALCD_puts(const char* str) {

  while (*str != '\0') {
    ALCD_putChar(*str++);
  }
}
void ALCD_putsXY(uint8_t x, uint8_t y, const char* str) {
  ALCD_goto(x, y);
  ALCD_puts(str);
}

#if ALCD_SUPPORT_PUTNUM
/* void ALCD_putNum(uint32_t num) {

  char tmp[13];

  snprintf(tmp, sizeof(tmp) - 1, "%d", num);
  ALCD_puts(tmp);

} */
void ALCD_putNumXY(uint8_t x, uint8_t y, uint32_t num) {
  ALCD_goto(x, y);
  char tmp[13];
  snprintf(tmp, sizeof(tmp) - 1, "%d", num);
  ALCD_puts(tmp);

  // For clearing dead spaces
  if (floor(log10(oldNum)) > floor(log10(num))) {
    ALCD_clearSection(x + floor(log10(num)), y, x + (floor(log10(oldNum))), y);
  }
  oldNum = num;

}
#endif
#if ALCD_SUPPORT_PRINTF
/* void ALCD_printf(const char* fmt, ...) {

  char tmp[ALCD_PRINTF_BUF_SIZE];

  va_list args;
  va_start(args, fmt);
  vsnprintf(tmp, sizeof(tmp) - 1, fmt, args);
  va_end(args);
  ALCD_puts(tmp);

} */
void ALCD_printfXY(uint8_t x, uint8_t y, const char* fmt, ...) {
  char tmp[ALCD_PRINTF_BUF_SIZE];

  va_list args;
  va_start(args, fmt);
  vsnprintf(tmp, sizeof(tmp) - 1, fmt, args);
  va_end(args);
  ALCD_goto(x, y);
  ALCD_puts(tmp);

  // For clearing dead spaces
  if (oldLen > strlen(tmp)) {
    ALCD_clearSection(x + strlen(tmp) , y, x + oldLen , y);
  }
  oldLen = strlen(tmp);
}
#endif

/* USER CODE END 4 */