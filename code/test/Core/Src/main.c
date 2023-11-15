/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "fonts.h"
#include "ST7735.h"
#include "table_content.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
 RTC_HandleTypeDef hrtc;

SPI_HandleTypeDef hspi1;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
static void MX_RTC_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void show_table(int bcolor, int fcolor)
{
	state = TABLE_SHOWING;
	ST7735_FillScreen(bcolor);

	for (int i = 0; i < main_table.size; ++i) {
		char format_string[20];
		for (int j = 0; j < main_table.size; ++j) {
			format_string[j * 3] = main_table.data[i][j] / 10 + '0';
			format_string[j * 3 + 1] = main_table.data[i][j] % 10 + '0';
			format_string[j * 3 + 2] = ' ';
		}
		format_string[main_table.size * 3 - 1] = '\0';
		ST7735_DrawString(3, 3 + i * 15, format_string, Font_7x10, fcolor, bcolor);
	}

	for(int i = 1; i < main_table.size; ++i) {
		ST7735_DrawLine(i * 18, 0, i * 18, main_table.size * 14 + 4, fcolor);
		ST7735_DrawLine(0, i * 15, 18 * main_table.size + 2, i * 15, fcolor);
	}

	char format_string[] = "Size: _x_";
	format_string[6] = format_string[8] =  main_table.size + '0';
	ST7735_DrawString(3, 3 + (main_table.size + 1) * 15, format_string, Font_7x10, fcolor, bcolor);
}

void write_size(int bcolor, int fcolor) {
	if (main_table.size == 0) {
		ST7735_DrawString(3, 78, "None", Font_7x10, fcolor, bcolor);
		state = CHOOSING_SIZE;
	} else {
		char format_str[5] = "    \0";
		format_str[0] = main_table.size + '0';
		ST7735_DrawString(3, 78, format_str, Font_7x10, fcolor, bcolor);
		state = CHOSED_SIZE;
	}
}

void reset_screen(int bcolor, int fcolor) {
	  ST7735_FillScreen(bcolor);
	  ST7735_DrawString(3, 3, "Choose size from", Font_7x10, fcolor, bcolor);
	  ST7735_DrawString(3, 18, "[3;7] by pressing", Font_7x10, fcolor, bcolor);
	  ST7735_DrawString(3, 33, "corresponding button", Font_7x10, fcolor, bcolor);
	  ST7735_DrawString(3, 63, "Chosen size:", Font_7x10, fcolor, bcolor);
	  write_size(bcolor, fcolor);
}

void size_chosed(int size, int entropy, int bcolor, int fcolor) {
	main_table.size = size;
	write_size(bcolor, fcolor);
	fill_table(&main_table, entropy, HAL_GetTick());
	state = CHOSED_SIZE;
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

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
  MX_SPI1_Init();
  MX_RTC_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  int bcolor = ST7735_BLACK;
  int fcolor = ST7735_WHITE;
  ST7735_Init();
  reset_screen(bcolor, fcolor);
  int time = 0;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_11) == 0 && state != TABLE_SHOWING) {
		  size_chosed(3, 18, bcolor, fcolor);
	  } else if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_12) == 0 && state != TABLE_SHOWING) {
		  size_chosed(4, 32, bcolor, fcolor);
	  }  else if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_13) == 0 && state != TABLE_SHOWING) {
		  size_chosed(5, 50, bcolor, fcolor);
	  } else if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_14) == 0 && state != TABLE_SHOWING) {
		  size_chosed(6, 72, bcolor, fcolor);
	  } else if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_15) == 0 && state != TABLE_SHOWING) {
		  size_chosed(7, 98, bcolor, fcolor);
	  } else if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0) == 0 && state == CHOSED_SIZE) {
		  show_table(bcolor, fcolor);
		  time = HAL_GetTick();
	  } else if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1) == 0 && state == TABLE_SHOWING) {
		  time = HAL_GetTick() - time;
		  results[main_table.size - 3].time_sum += time;
		  ++results[main_table.size - 3].count;

		  char new_record[100], mean_record[100];
		  sprintf(new_record, "New result for %dx%d table:  %02d:%02d:%03d\r\n", main_table.size, main_table.size, time / 60000, (time / 1000) % 60, time % 1000);
		  int sum_time = results[main_table.size - 3].time_sum / results[main_table.size - 3].count;
		  sprintf(mean_record, "Mean result for %dx%d table: %02d:%02d:%03d\r\n", main_table.size, main_table.size, sum_time / 60000, (sum_time / 1000) % 60, sum_time % 1000);

		  HAL_UART_Transmit(&huart1, (uint8_t *)new_record, strlen(new_record), 100);

		  if (time <= results[main_table.size - 3].best_time) {
			  char message[40];
			  sprintf(message, "New best time for %dx%d table!\r\n", main_table.size, main_table.size);
			  HAL_UART_Transmit(&huart1, (uint8_t *)message, strlen(message), 100);
			  results[main_table.size - 3].best_time = time;
		  }

		  HAL_UART_Transmit(&huart1, (uint8_t *)mean_record, strlen(mean_record), 100);
		  HAL_UART_Transmit(&huart1, (uint8_t *)"\r\n", strlen("\r\n"), 100);

		  if (time == results[main_table.size - 3].best_time) {
			  for (int i = 0; i < 100; ++i) {
				  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, 0);
				  HAL_Delay(15);
				  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, 1);
				  HAL_Delay(5);
			  }
		  }

		  main_table.size = 0;
		  reset_screen(bcolor, fcolor);
	  }
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */

  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.AsynchPrediv = RTC_AUTO_1_SECOND;
  hrtc.Init.OutPut = RTC_OUTPUTSOURCE_ALARM;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */

}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15
                          |GPIO_PIN_7, GPIO_PIN_RESET);

  /*Configure GPIO pins : PB0 PB1 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PB12 PB13 PB14 PB15
                           PB7 */
  GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15
                          |GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PA11 PA12 PA13 PA14
                           PA15 */
  GPIO_InitStruct.Pin = GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14
                          |GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

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

#ifdef  USE_FULL_ASSERT
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
