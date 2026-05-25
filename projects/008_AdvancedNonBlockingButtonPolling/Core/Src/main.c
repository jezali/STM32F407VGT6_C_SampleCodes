/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

typedef enum {
   KEY_STATE_HOLD         = 0x00,
   KEY_STATE_RELEASED     = 0x01,
   KEY_STATE_PRESSED      = 0x02,
   KEY_STATE_NONE         = 0x03,
} Key_State;

typedef struct {
  GPIO_TypeDef*           GPIO;
  uint16_t                Pin;
} Key_PinConfig;

struct __Key;
typedef struct __Key Key;

typedef uint8_t (*Key_CallBackFN)(Key* key, Key_State state);

typedef union {
  Key_CallBackFN        fn[4];
  struct {
    Key_CallBackFN      onHOLD;
    Key_CallBackFN      onRELEASED;
    Key_CallBackFN      onPRESSED;
    Key_CallBackFN      onNONE;
  };
} Key_CallBacks;

struct __Key {
  const Key_PinConfig*    Config;
  Key_CallBacks           CallBacks;
  Key_State               State;
  uint8_t                 Handled;
};

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define KEY_SAMPLE_PERIOD       50 //ms

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

static const Key_PinConfig KEY_PIN_CONFIGS[] = {
  {BTN0_GPIO_Port, BTN0_Pin},
  {BTN1_GPIO_Port, BTN1_Pin}
};

uint16_t BlinkTimeout = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

void Key_init(Key* keys, const Key_PinConfig* configs, uint32_t len);
void Key_handle(Key* keys, uint32_t len);

uint8_t Btn0_onPRESSED(Key* key,Key_State state);

uint8_t Btn1_onPRESSED(Key* key,Key_State state);
uint8_t Btn1_onHOLD(Key* key,Key_State state);
uint8_t Btn1_onRELEASED(Key* key,Key_State state);


/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */
  uint32_t keyNextSample = 0;
  Key keys[2];
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  /* USER CODE BEGIN 2 */

  Key_init(keys, KEY_PIN_CONFIGS, 2);

  keys[0].CallBacks.onPRESSED = Btn0_onPRESSED;

  keys[1].CallBacks.onPRESSED = Btn1_onPRESSED;
  keys[1].CallBacks.onHOLD = Btn1_onHOLD;
  keys[1].CallBacks.onRELEASED = Btn1_onRELEASED;

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    if (keyNextSample <= HAL_GetTick()) {
      keyNextSample = HAL_GetTick() + KEY_SAMPLE_PERIOD;
      // Check key state
      Key_handle(keys, 2);
    }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

uint8_t Btn0_onPRESSED(Key* key,Key_State state) {
  HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin);
  return 0;
}

uint8_t Btn1_onPRESSED(Key* key,Key_State state) {
  BlinkTimeout = HAL_GetTick() + 3000;        /*[AGE // RO BARADARI KHARAB MISHE]*/
  HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
  return 0;
}
uint8_t Btn1_onHOLD(Key* key,Key_State state) {
  HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
  return BlinkTimeout <= HAL_GetTick();
}
uint8_t Btn1_onRELEASED(Key* key,Key_State state) {
  HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
  return 0;
}

void Key_init(Key* keys, const Key_PinConfig* configs, uint32_t len) {
  while (len-- > 0) {
    keys->Config = configs;
    keys->State = KEY_STATE_NONE;
    keys++;
    configs++;
  }
}

void Key_handle(Key* keys, uint32_t len) {
  while (len-- > 0) {
    keys->State = ((keys->State << 1) | HAL_GPIO_ReadPin(keys->Config->GPIO, keys->Config->Pin)) & 3;

    if (!keys->Handled && keys->CallBacks.fn[keys->State] != NULL) {
      keys->Handled = keys->CallBacks.fn[keys->State](keys, keys->State);
    }
    else if (keys->State == KEY_STATE_NONE) {
      keys->Handled = 0;
    }

    keys++;
  }

}


/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
