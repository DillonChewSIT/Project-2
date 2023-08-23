/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  \author Dillon Chew Zhi Yuan
  \date 26-3-22
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#include "structures.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define PIXY_TYPE_REPONSE_ERROR 			 0x03
#define PIXY_TYPE_REQUEST_VERSION            0x0e
#define PIXY_TYPE_REQUEST_VERSION            0x0e
#define PIXY_RESULT_OK                       0
#define PIXY_RESULT_ERROR                    -1
#define PIXY_RESULT_BUSY                     -2
#define key									 0xff
#define MAX_SIGNATURE 						 7
#define CAM_X								 158 //centerpoint of the entire frame
#define CAM_Y								 104 //centerpoint of the entire frame
#define commkey                              0x7F
#define header                               0x7A

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
 UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;

/* USER CODE BEGIN PV */
int version[4]; //contains hardware version, major, minor and build
int resolution[2]; //contains width and height

/*struct getVer *Version = &ver1;
struct getRes *Resolution = &res1;*/


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */
int8_t getVersion();
int8_t getResolution();
int8_t setCameraBrightness(uint8_t brightness);
void getBlocks(uint8_t sigmap, uint8_t maxBlocks);
void Print();
void getDirection(Block blocks);
void Forward(char);
void Reverse(char);
void ForwardDiagLeft(char);
void ForwardDiagRight(char);
void ReverseDiagRight(char);
void ReverseDiagLeft(char);
void Right(char);
void Left(char);
void Stop();
char SetSpeed(int, int);

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
  MX_USART2_UART_Init();
  MX_USART3_UART_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  getBlocks(0x01, 0x01);

	   //getDirection();
	   //Print();


	  //HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
	  //HAL_Delay(500);
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL16;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
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
  huart1.Init.BaudRate = 19200;
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
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

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
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

}

/* USER CODE BEGIN 4 */


/***************************************************************************/
/*!
\brief
- getVersion
  transmits a packet and receives the version details of pixy2

\return
 state of pixy2. Returns OK if packet received is correct, returns ERROR if it is not
*/
/**************************************************************************/
int8_t getVersion(){

	  uint8_t 	tx_pixy2[] = {0xae, 0xc1, 0x0e, 0x00};
	  uint8_t 	rx_pixy2[22];


	  HAL_UART_Transmit(&huart1, (uint8_t *)tx_pixy2, 4, 100);
	  HAL_UART_Receive(&huart1, rx_pixy2, sizeof(rx_pixy2), 100);

	  uint16_t checksum = (rx_pixy2[4] & key) | ((rx_pixy2[5] & key) << 8);
	  int cal_checksum = 0;
	   for (int i=6; i < 22; i++){
		 cal_checksum += rx_pixy2[i];
	   }

	   if(cal_checksum == checksum)
	   {
		  version[0] = (rx_pixy2[6] & key) | ((rx_pixy2[7] & key) << 8);
		  version[1] = rx_pixy2[8];
		  version[2] = rx_pixy2[9];
		  version[3] = (rx_pixy2[10] & key) | ((rx_pixy2[11] & key) << 8);
		  return PIXY_RESULT_OK;
	   }
	   return PIXY_RESULT_ERROR;

}


/***************************************************************************/
/*!
\brief
- getResolution
  transmits a packet and receives the resolution details of pixy2

\return
 state of pixy2. Returns OK if packet received is correct, returns ERROR if it is not
*/
/**************************************************************************/
int8_t getResolution()
{
	uint8_t tx_pixy2[] = {0xae,  0xc1, 0x0c, 0x01, 0x00};
	uint8_t rx_pixy2[22];

	HAL_UART_Transmit(&huart1, (uint8_t *)tx_pixy2, sizeof(tx_pixy2), 100);
	HAL_UART_Receive(&huart1, rx_pixy2, sizeof(rx_pixy2), 100);

	uint16_t res_checksum = (rx_pixy2[4] & key) | ((rx_pixy2[5] & key) << 8);
	int res_cal_checksum = rx_pixy2[6] + rx_pixy2[7] + rx_pixy2[8] + rx_pixy2[9];
	if (res_cal_checksum == res_checksum)
	{
		resolution[0] = (rx_pixy2[6] & key) | ((rx_pixy2[7] & key) << 8);
		resolution[1] = (rx_pixy2[8] & key) | ((rx_pixy2[9] & key) << 8);
		return PIXY_RESULT_OK;
	}
	return PIXY_RESULT_ERROR;

}

/***************************************************************************/
/*!
\brief
- setCameraBrightness
  transmits a packet to set the camera brightness

\return
 state of pixy2. Returns OK if packet received is correct, returns ERROR if it is not
*/
/**************************************************************************/
int8_t setCameraBrightness(uint8_t brightness)
{
	uint8_t tx_pixy2[] = {0xae,  0xc1, 0x10, 0x01, brightness};
	uint8_t rx_pixy2[22];

	HAL_UART_Transmit(&huart1, (uint8_t *)tx_pixy2, sizeof(tx_pixy2), 100);
	HAL_UART_Receive(&huart1, rx_pixy2, sizeof(rx_pixy2), 100);

 	uint16_t camera_checksum = (rx_pixy2[4] & key) | ((rx_pixy2[5] & key) << 8);
 	uint32_t result = rx_pixy2[6] + rx_pixy2[7] + rx_pixy2[8] + rx_pixy2[9];
 	if (camera_checksum == result){
 		return PIXY_RESULT_OK;
 	}
 	return PIXY_RESULT_ERROR;

}

/***************************************************************************/
/*!
\brief
- getBlocks
  transmits a packet and receives the information of the block

\return
 state of pixy2. Returns OK if packet received is correct, returns ERROR if it is not
*/
/**************************************************************************/
void getBlocks(uint8_t sigmap, uint8_t maxBlocks)
{
	uint8_t tx_pixy2[] = {0xae, 0xc1, 0x20, 0x02, sigmap, maxBlocks};
	uint8_t rx_pixy2[255];
	Block blocks;

	HAL_UART_Transmit(&huart1, (uint8_t *)tx_pixy2, sizeof(tx_pixy2), 100);
	HAL_UART_Receive(&huart1, rx_pixy2, sizeof(rx_pixy2), 100);
	uint16_t checksum = (rx_pixy2[4] & key) | ((rx_pixy2[5] & key) << 8);
	int cal_checksum = 0;

	int array_size = rx_pixy2[3];


	for(int i = 6; i < array_size+6; i++)
	{
		cal_checksum += rx_pixy2[i];
	}

	if (cal_checksum == checksum)
	{
		if (cal_checksum == 0 )
		{
			Stop();
		}
		else {
			blocks.signature = (rx_pixy2[6] & key) | ((rx_pixy2[7] & key) << 8);
			blocks.x = (rx_pixy2[8] & key) | ((rx_pixy2[9] & key) << 8);
			blocks.y = (rx_pixy2[10] & key) | ((rx_pixy2[11] & key) << 8);
			blocks.width = (rx_pixy2[12] & key) | ((rx_pixy2[13] & key) << 8);
			blocks.height = (rx_pixy2[14] & key) | ((rx_pixy2[15] & key) << 8);
			blocks.angle = (rx_pixy2[16] & key) | ((rx_pixy2[17] & key) << 8);
			blocks.index = rx_pixy2[18];
			blocks.age = rx_pixy2[19];
			getDirection(blocks);
		}

	}
}
/*void Print(){
	char strbuff[255];
	int strbufflen = sprintf(strbuff, "signature: %d\r\n""x: %d\r\n""y: %d\r\n""width: %d\r\n""height: %d\r\n""angle: %d\r\n""index: %d\r\n""age: %d\r\n",
			blocks->signature,blocks->x,blocks->y,blocks->width,blocks->height,blocks->angle,blocks->index,blocks->age);
	HAL_UART_Transmit(&huart2, (uint8_t *)strbuff, strbufflen, 100);
}*/

/***************************************************************************/
/*!
\brief
- getDirection
  transmit a direction based on the area and coordinates of a block
*/
/**************************************************************************/
void getDirection(Block blocks)
{
	int x = (int) blocks.x; // x center of the block
	int y = (int) blocks.y; // y center of the block

	int area = (int) blocks.width * (int) blocks.height; //area of the block
	int area_sz = 100*60; //area to determine if block is too close
	char speed = SetSpeed(area, area_sz);


	if(x < 178 && x > 138) //if x center of block is within the middle section of the frame
	{
		area<=area_sz ? Forward(speed) : Reverse(speed);
	}
	if (x >= 178) //if x center of block is within the right section of the frame
	{
		if (y > 89 && y < 119)
		{
			area <= area_sz ? Right(speed) : ReverseDiagRight(speed);
		}
		if (y >= 119)
		{
			area <= area_sz ? ForwardDiagRight(speed) : ReverseDiagRight(speed);
		}
		if (y <= 89)
		{
			area <= area_sz ? ForwardDiagRight(speed) : ReverseDiagRight(speed);
		}
	}
	if (x <= 138) //if x center of block is within the left section of the frame
	{
		if (y > 89 && y < 119)
		{
			area <= area_sz ? Left(speed) : ReverseDiagLeft(speed);
		}
		if (y >= 119)
		{
			area <= area_sz ? ForwardDiagLeft(speed) : ReverseDiagLeft(speed);
		}
		if (y <= 89)
		{
			area <= area_sz ? ForwardDiagLeft(speed) : ReverseDiagLeft(speed);
		}
	}


}
void Forward(char speed) //transmit hex key for forward
{
	char direction = 0x01;
	char checksum = (direction ^ speed ) ^ commkey;
	char tmp[4] = {header, direction, speed, checksum};
	char temp[1];
	for (int i=0; i < 4; ++i)
	{
		temp[0] = tmp[i];
		HAL_UART_Transmit(&huart3, (uint8_t *)temp,sizeof(temp),100);
	}
}
void Reverse(char speed) //transmit hex key for reverse
{
	char direction = 0x02;
	char checksum = direction ^ speed ^ commkey;
	char tmp[4] = {header, direction, speed, checksum};
	char temp[1];
	for (int i=0; i < 4; ++i)
	{
		temp[0] = tmp[i];
		HAL_UART_Transmit(&huart3, (uint8_t *)temp,sizeof(temp),100);
	}
}
void ForwardDiagLeft(char speed) //transmit hex key for forward diagonal left
{
	char direction = 0x03;
	char checksum = direction ^ speed ^ commkey;
	char tmp[4] = {header, direction, speed, checksum};
	char temp[1];
	for (int i=0; i < 4; ++i)
	{
		temp[0] = tmp[i];
		HAL_UART_Transmit(&huart3, (uint8_t *)temp,sizeof(temp),100);
	}
}
void ReverseDiagRight(char speed) //transmit hex key for reverse diagonal right
{
	char direction = 0x04;
	char checksum = direction ^ speed ^ commkey;
	char tmp[4] = {header, direction, speed, checksum};
	char temp[1];
	for (int i=0; i < 4; ++i)
	{
		temp[0] = tmp[i];
		HAL_UART_Transmit(&huart3, (uint8_t *)temp,sizeof(temp),100);
	}
}
void ForwardDiagRight(char speed) //transmit hex key for forward diagonal right
{
	char direction = 0x05;
	char checksum = direction ^ speed ^ commkey;
	char tmp[4] = {header, direction, speed, checksum};
	char temp[1];
	for (int i=0; i < 4; ++i)
	{
		temp[0] = tmp[i];
		HAL_UART_Transmit(&huart3, (uint8_t *)temp,sizeof(temp),100);
	}
}

void ReverseDiagLeft(char speed) //transmit hex key for reverse diagonal left
{
	char direction = 0x06;
	char checksum = direction ^ speed ^ commkey;
	char tmp[4] = {header, direction, speed, checksum};
	char temp[1];
	for (int i=0; i < 4; ++i)
	{
		temp[0] = tmp[i];
		HAL_UART_Transmit(&huart3, (uint8_t *)temp,sizeof(temp),100);
	}
}

void Right(char speed) //transmit hex key for right
{
	char direction = 0x07;
	char checksum = direction ^ speed ^ commkey;
	char tmp[4] = {header, direction, speed, checksum};
	char temp[1];
	for (int i=0; i < 4; ++i)
	{
		temp[0] = tmp[i];
		HAL_UART_Transmit(&huart3, (uint8_t *)temp,sizeof(temp),100);
	}
}
void Left(char speed) //transmit hex key for left
{
	char direction = 0x08;
	char checksum = direction ^ speed ^ commkey;
	char tmp[4] = {header, direction, speed, checksum};
	char temp[1];
	for (int i=0; i < 4; ++i)
	{
		temp[0] = tmp[i];
		HAL_UART_Transmit(&huart3, (uint8_t *)temp,sizeof(temp),100);
	}
}
void Stop() //transmit hex key for stop
{
	char direction = 0x09;
	char speed = 0x00;
	char checksum = direction ^ speed ^ commkey;
	char tmp[4] = {header, direction, speed, checksum};
	char temp[1];
	for (int i=0; i < 4; ++i)
	{
		temp[0] = tmp[i];
		HAL_UART_Transmit(&huart3, (uint8_t *)temp,sizeof(temp),100);
	}
}

char SetSpeed(int block_area, int max_block_size) //speed setting based on how close the object is
{
	float frame = 30000;
	char speed;
	float ratio = 25000;
	if (block_area > max_block_size)
	{
		if (block_area > frame)
		{
			speed = 0x00;
		}
		else
		{
			speed = (0x28/ratio) * (block_area);
		}
	}
	else
	{
		speed = (0x28 - ((0x28/(float)max_block_size) * block_area));
	}
	return speed;
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
