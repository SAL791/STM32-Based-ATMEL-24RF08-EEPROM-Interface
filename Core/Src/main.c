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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

typedef uint8_t byte;
typedef bool boolean;

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define confTableSize 64

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

byte orgData[16];

byte kornData[10] = {
    0x00, 0x00,
    //0x25, 0x18, 0x13, 0x31, 0x00, 0x00, 0x00, 0x81};//password korn
    //0x02, 0x03, 0x04, 0x04, 0x00, 0x00, 0x00, 0x0e};//password 1234
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};// clear password

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */

void UART_Print(const char *message);
void UART_Printf(const char *format, ...);
char UART_ReadChar(void);

boolean send_i2c_data(byte *data, byte byte_count);
boolean receive_i2c_data(byte addr, byte *received);

static void dumpEeprom(boolean debug);
static void readPassword(const char *table);
static void setPwKorn(void);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

static const char convTable[256] = {
		' ', ' ', '1', '2', '3', '4', '5', '6',    '7', '8', '9', '0', '-', '=', ' ', ' ',
		'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I',    'O', 'P', ' ', '$', ' ', ' ', 'A', 'S',
		'D', 'F', 'G', 'H', 'J', 'K', 'L', ';',    '\'','`', '\\','\\','Z', 'X', 'C', 'V',
		'B', 'N', 'M', ',', '.', '/', '"', '*',    ' ', ' ', ' ', ' ', 'f', 'f', 'f', 'f',

		'f', 'f', 'f', 'f', 'f', ' ', ' ', '7',    '8', '9', '-', '4', '5', '6', '+', '1',
		'2', '3', '0', '.', ' ', ' ', '>', ' ',    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
		' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
		' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',

		' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
		' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
		' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
		' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',

		' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
		' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
		' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
		' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
		};

static const char convTableAlternate[256] = {
		' ', ' ', ' ', ' ', '1', '1', '2', '3',    '3', '3', '4', '4', '5', '5', '6', '6',
		'7', '7', '8', '8', '9', '9', '0', '0',    '-', '-', '=', '=', ' ', ' ', ' ', ' ',
		'Q', 'Q', 'W', 'W', 'E', 'E', 'R', 'R',    'T', 'T', 'Y', 'Y', 'U', 'U', 'I', 'I',
		'O', 'O', 'P', 'P', ' ', ' ', '$', '$',    ' ', ' ', ' ', ' ', 'A', 'A', 'S', 'S',

		'D', 'D', 'F', 'F', 'G', 'G', 'H', 'H',    'J', 'J', 'K', 'K', 'L', 'L', ';', ';',
		'\'','\'','`', '`', '\\','\\','\\','\\',   'Z', 'Z', 'X', 'X', 'C', 'C', 'V', 'V',
		'B', 'B', 'N', 'N', 'M', 'M', ',', ',',    '.', '.', '/', '/', '"', '"', '*', '*',
		' ', ' ', ' ', ' ', ' ', ' ', 'f', 'f',    'f', 'f', 'f', 'f', 'f', 'f', 'f', 'f',

		'f', 'f', 'f', 'f', 'f', 'f', 'f', 'f',    'f', 'f', ' ', ' ', ' ', ' ', '7', '7',
		'8', '8', '9', '9', '-', '-', '4', '4',    '5', '5', '6', '6', '+', '+', '1', '1',
		'2', '2', '3', '3', '0', '0', '.', '.',    ' ', ' ', ' ', ' ', '>', '>', ' ', ' ',
		' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',

		' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
		' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
		' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
		' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
		};

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
  MX_I2C1_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */

  HAL_Delay(20);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

    UART_Print("\r\n\r\n\r\nDeif Thinkpad Eeprom Reader\r\n");
    UART_Print("Usage:\r\n");
    UART_Print("p: Read the Password in Plaintext\r\n");
    UART_Print("w: Read the Password in Plaintext (alternate Scancode)\r\n");
    UART_Print("d: Dump the Eeprom in HEX format (for import in IBMpass)\r\n");
    UART_Print("g: Dump the Eeprom in Text format (for debug)\r\n");
    UART_Print("h: Clear the password\r\n");
    UART_Print("> ");

    char c = UART_ReadChar();
    while (c == '\r' || c == '\n')
        c = UART_ReadChar();

    if (c == 'p')
        readPassword(convTable);
    else if (c == 'w')
        readPassword(convTableAlternate);
    else if (c == 'd')
        dumpEeprom(false);
    else if (c == 'g')
        dumpEeprom(true);
    else if (c == 'h'){
        UART_Print("\r\nThis operation writes zeroes to EEPROM password data.\r\n");
        UART_Print("Type Y to confirm, any other key to cancel: ");

        if (UART_ReadChar() == 'Y')
            setPwKorn();
        else
            UART_Print("\r\nWrite cancelled.\r\n");
    }
    else
        UART_Print("\r\nUnknown command!\r\n");

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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

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
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

//returns false on nack
boolean send_i2c_data(byte *data, byte byte_count){
    HAL_StatusTypeDef status;

    if (data == NULL || byte_count < 2)
        return false;

    status = HAL_I2C_Master_Transmit(&hi2c1,
                                     (uint16_t)data[0],
                                     &data[1],
                                     (uint16_t)(byte_count - 1U),
                                     1000);

    if (status != HAL_OK){
        UART_Printf("xNACK!! I2C transmit error: %d\r\n", (int)status);
        return false;
    }
    HAL_Delay(10);

    return true;
}


boolean receive_i2c_data(byte addr, byte *received){
    HAL_StatusTypeDef status;

    if (received == NULL)
        return false;

    status = HAL_I2C_Master_Receive(&hi2c1,
                                   (uint16_t)(addr & 0xFEU),
                                   received,
                                   1,
                                   1000);

    if (status != HAL_OK){
        UART_Printf("yNACK!! I2C receive error: %d\r\n", (int)status);
        return false;
    }

    return true;
}

void UART_Print(const char *message){
    HAL_UART_Transmit(&huart2,
                      (uint8_t *)message,
                      (uint16_t)strlen(message),
                      HAL_MAX_DELAY);
}

void UART_Printf(const char *format, ...){
    char buffer[160];
    va_list args;

    va_start(args, format);
    int length = vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    if (length < 0)
        return;

    if (length >= (int)sizeof(buffer))
        length = (int)sizeof(buffer) - 1;

    HAL_UART_Transmit(&huart2,
                      (uint8_t *)buffer,
                      (uint16_t)length,
                      HAL_MAX_DELAY);
}

char UART_ReadChar(void){
    uint8_t received = 0;

    if (HAL_UART_Receive(&huart2, &received, 1, HAL_MAX_DELAY) != HAL_OK)
        return '\0';

    return (char)received;
}

static void dumpEeprom(boolean debug){
    byte page = 0;
    byte data[2];
    byte dat = 0;
    boolean res;

    data[0] = 0xa8 | 0x00;
    data[1] = 0;

    UART_Print("Dump the whole eeprom: \r\n");

    while (true){
        res = send_i2c_data(data, 2);
        if (!res){
            UART_Print("   NACK   \r\n");
            UART_Print("!!! Error !!! Reading Eeprom failed!\r\n");
            return;
        }

        if (!receive_i2c_data((byte)(data[0] | 0x01), &dat)){
            UART_Print("!!! Error !!! Reading Eeprom failed!\r\n");
            return;
        }

        if (debug){
            UART_Printf("Page %u; %u: %X => 0x",
                        (unsigned int)(page + 1U),
                        (unsigned int)data[1],
                        (unsigned int)((unsigned int)data[1] +
                                       255U * (unsigned int)page));
        }

        if (dat < 0x10)
            UART_Print("0");

        UART_Printf("%X ", (unsigned int)dat);

        if (debug){
            UART_Printf(" => %c => %c\r\n",
                        convTable[dat],
                        convTableAlternate[dat]);
        }

        data[1]++;
        if (data[1] == 0){
            if (page == 0){
                data[0] = 0xa8 | 0x02;
                page = 1;
            }
            else if (page == 1){
                data[0] = 0xa8 | 0x04;
                page = 2;
            }
            else if (page == 2){
                data[0] = 0xa8 | 0x06;
                page = 3;
            }
            else if (page == 3){
                UART_Print("\r\n");
                return;
            }
            data[1] = 0;
        }
    }
}

static void readPassword(const char *table){
    byte data[2];
    byte dat = 0;
    boolean res;

    data[0] = 0xa8 | 0x06;
    data[1] = 56;

    UART_Print("The Supervisor Password is: ");

    while (true){
        res = send_i2c_data(data, 2);

        if (!res){
            UART_Print("   NACK   \r\n");
            UART_Print("!!! Error !!! Could not read the Password!\r\n");
            return;
        }

        if (!receive_i2c_data((byte)(data[0] | 0x01), &dat)){
            UART_Print("!!! Error !!! Could not read the Password!\r\n");
            return;
        }
        UART_Printf("%c", table[dat]);

        data[1]++;

        if (data[1] > 62){
            UART_Print("\r\n");
            return;
        }
    }
}


/*
void readAccessProt(){
    byte data[2];

    UART_Print("Access Page: ");

    data[0] = 0xa8;
    data[1] = 0;
    boolean res = send_i2c_data(data,2);



    for (byte i = 0; i < 10; i++) {
        data[0] = 0xb8;
        data[1] = i;
        //boolean res = send_i2c_data(data,2);
        byte dat = 0;
        receive_i2c_data((byte)(data[0] | 0x01), &dat);
        if (dat < 0x10) UART_Print("0");
        UART_Printf("%X ", dat);
    }

    UART_Print("\r\n");
}*/






static void setPwKorn(void){
    byte data[2];
    byte dat = 0;
    int i = 0;
    boolean res;

    data[0] = 0xa8 | 0x06;
    data[1] = 56;

    UART_Print("Original data:\r\n");

    while (true){
        res = send_i2c_data(data, 2);

        if (!res){
            UART_Print("   NACK   \r\n");
            UART_Print("!!! Error !!! Could not read the Password!\r\n");
            return;
        }

        if (!receive_i2c_data((byte)(data[0] | 0x01), &dat)){
            UART_Print("!!! Error !!! Could not read the Password!\r\n");
            return;
        }

        orgData[i++] = dat;

        if (dat < 0x10)
            UART_Print("0");

        UART_Printf("%X ", (unsigned int)dat);

        data[1]++;

        if (data[1] > 71){
            UART_Print("\r\n");
            break;
        }
    }

    kornData[0] = 0xa8 | 0x06;
    kornData[1] = 56;
    res = send_i2c_data(kornData, 10);

    if (!res){
        UART_Print("   NACK   \r\n");
        UART_Print("!!! Error !!! Could not set the Password!\r\n");
        return;
    }

    kornData[0] = 0xa8 | 0x06;
    kornData[1] = 64;
    res = send_i2c_data(kornData, 10);

    if (!res){
        UART_Print("   NACK2   \r\n");
        UART_Print("!!! Error !!! Could not set the Password!\r\n");
        return;
    }

    UART_Print("Write transactions completed. Read back the EEPROM to verify.\r\n");
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
