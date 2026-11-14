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
#include "bme280.h"
#include "ESPLib.h"
#include <string.h>
#include <stdio.h>
#include <stdint.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* 1 = full project (sensor + ESP8266 + MQTT)
 * 0 = BME280 sensor test only (ESP8266 not needed, nothing is sent)      */
#define ENABLE_ESP        1

/* Enter YOUR OWN Wi-Fi details here (2.4 GHz network only).
 * Remove them before sharing this file with anyone.                      */
#define WIFI_SSID         "SNUC"
#define WIFI_PASSWORD     "snu12345"

#define MQTT_BROKER_HOST  "broker.emqx.io"
#define MQTT_BROKER_PORT  "1883"

/* The public broker is shared: keep the client ID unique.
 * Your friend's project uses "STM32_BME280_Device01", so do not reuse it. */
#define MQTT_CLIENT_ID    "STM32_BME280_SNUC_Dev02"
#define MQTT_TOPIC        "snuc/iot/bme280/device01"

#define PUBLISH_PERIOD_MS 5000U

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

struct bme280_dev bme280;
struct bme280_data sensor_data;
struct bme280_settings settings;

MQTT_InitTypeDef_t mqttConfig;

char mqttMessage[100];
char mqttTopic[] = MQTT_TOPIC;

uint8_t bme280_address = 0;
int8_t rslt;

char msg[150];

/* ---- I2C read-failure diagnostics (filled by BME280_I2C_Read) ---- */
uint8_t  i2c_diag_valid  = 0;   /* 1 = a HAL read failure was captured   */
uint8_t  i2c_diag_status = 0;   /* HAL_StatusTypeDef returned by HAL     */
uint32_t i2c_diag_error  = 0;   /* HAL_I2C_GetError() bit flags          */
uint8_t  i2c_diag_state  = 0;   /* HAL_I2C_GetState() value              */
uint8_t  i2c_diag_dev    = 0;   /* 7-bit device address used             */
uint8_t  i2c_diag_reg    = 0;   /* register being read                   */
uint16_t i2c_diag_len    = 0;   /* number of bytes requested             */
uint8_t  i2c_diag_busy   = 0;   /* I2C BUSY flag right after failure     */
uint32_t i2c_fail_count  = 0;   /* total failed reads since reset        */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_I2C1_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */

static void BME280_Init(void);
static int8_t BME280_ReadData(void);

void Terminal_Print(char *message);

#if ENABLE_ESP
static uint8_t Find_Connack(const char *buf, uint16_t len);
#endif

static BME280_INTF_RET_TYPE BME280_I2C_Read(
		uint8_t reg_addr,
		uint8_t *reg_data,
		uint32_t len,
		void *intf_ptr
);

static BME280_INTF_RET_TYPE BME280_I2C_Write(
		uint8_t reg_addr,
		const uint8_t *reg_data,
		uint32_t len,
		void *intf_ptr
);

static void BME280_Delay(
		uint32_t period,
		void *intf_ptr
);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* Send text to the PC terminal through USART2 (ST-LINK virtual COM port) */
void Terminal_Print(char *message)
{
	HAL_UART_Transmit(&huart2,
	                  (uint8_t *)message,
	                  strlen(message),
	                  HAL_MAX_DELAY);
}

/* ---- BME280 bus callbacks (blocking HAL I2C) ---- */

static BME280_INTF_RET_TYPE BME280_I2C_Read(
		uint8_t reg_addr,
		uint8_t *reg_data,
		uint32_t len,
		void *intf_ptr)
{
	uint8_t address = *(uint8_t *)intf_ptr;

	HAL_StatusTypeDef st = HAL_I2C_Mem_Read(&hi2c1,
	                                        (uint16_t)(address << 1),
	                                        reg_addr,
	                                        I2C_MEMADD_SIZE_8BIT,
	                                        reg_data,
	                                        (uint16_t)len,
	                                        100);

	if (st == HAL_OK)
	{
		return BME280_INTF_RET_SUCCESS;
	}

	/* Capture the failure details. No UART printing here: this runs inside
	 * the Bosch driver; BME280_ReadData() prints the values afterwards. */
	i2c_diag_valid  = 1;
	i2c_diag_status = (uint8_t)st;
	i2c_diag_error  = HAL_I2C_GetError(&hi2c1);
	i2c_diag_state  = (uint8_t)HAL_I2C_GetState(&hi2c1);
	i2c_diag_dev    = address;
	i2c_diag_reg    = reg_addr;
	i2c_diag_len    = (uint16_t)len;
	i2c_diag_busy   = (__HAL_I2C_GET_FLAG(&hi2c1, I2C_FLAG_BUSY) != RESET) ? 1U : 0U;
	i2c_fail_count++;

	return BME280_E_COMM_FAIL;
}

static BME280_INTF_RET_TYPE BME280_I2C_Write(
		uint8_t reg_addr,
		const uint8_t *reg_data,
		uint32_t len,
		void *intf_ptr)
{
	uint8_t address = *(uint8_t *)intf_ptr;

	if (HAL_I2C_Mem_Write(&hi2c1,
	                      (uint16_t)(address << 1),
	                      reg_addr,
	                      I2C_MEMADD_SIZE_8BIT,
	                      (uint8_t *)reg_data,
	                      (uint16_t)len,
	                      100) == HAL_OK)
	{
		return BME280_INTF_RET_SUCCESS;
	}

	return BME280_E_COMM_FAIL;
}

/* Bosch driver asks for microseconds; HAL_Delay works in milliseconds */
static void BME280_Delay(uint32_t period, void *intf_ptr)
{
	(void)intf_ptr;
	HAL_Delay((period + 999U) / 1000U);
}

/* Print the I2C failure details captured by BME280_I2C_Read() */
static void I2C_PrintDiag(void)
{
	uint32_t e = i2c_diag_error;

	if (!i2c_diag_valid)
	{
		Terminal_Print("  I2C diag: no HAL failure was recorded by the read callback\r\n");
		return;
	}

	snprintf(msg, sizeof(msg),
	         "  I2C diag: HAL status=%u (0=OK 1=ERROR 2=BUSY 3=TIMEOUT), HAL error=0x%02lX, state=0x%02X\r\n",
	         (unsigned)i2c_diag_status,
	         (unsigned long)e,
	         (unsigned)i2c_diag_state);
	Terminal_Print(msg);

	snprintf(msg, sizeof(msg),
	         "  Read: dev=0x%02X reg=0x%02X len=%u, BUSY flag=%u, total I2C read failures=%lu\r\n",
	         (unsigned)i2c_diag_dev,
	         (unsigned)i2c_diag_reg,
	         (unsigned)i2c_diag_len,
	         (unsigned)i2c_diag_busy,
	         (unsigned long)i2c_fail_count);
	Terminal_Print(msg);

	Terminal_Print("  HAL error flags:");
	if (e == HAL_I2C_ERROR_NONE) { Terminal_Print(" NONE"); }
	if (e & HAL_I2C_ERROR_BERR)    { Terminal_Print(" BERR"); }
	if (e & HAL_I2C_ERROR_ARLO)    { Terminal_Print(" ARLO"); }
	if (e & HAL_I2C_ERROR_AF)      { Terminal_Print(" AF(NACK)"); }
	if (e & HAL_I2C_ERROR_OVR)     { Terminal_Print(" OVR"); }
	if (e & HAL_I2C_ERROR_TIMEOUT) { Terminal_Print(" TIMEOUT"); }
	if (e & ~(uint32_t)(HAL_I2C_ERROR_BERR | HAL_I2C_ERROR_ARLO | HAL_I2C_ERROR_AF |
	                    HAL_I2C_ERROR_OVR | HAL_I2C_ERROR_TIMEOUT))
	{
		Terminal_Print(" OTHER");
	}
	Terminal_Print("\r\n");
}

/* ---- BME280 setup ---- */

static void BME280_Init(void)
{
	memset(&bme280, 0, sizeof(bme280));

	bme280.intf_ptr = &bme280_address;
	bme280.intf     = BME280_I2C_INTF;
	bme280.read     = BME280_I2C_Read;
	bme280.write    = BME280_I2C_Write;
	bme280.delay_us = BME280_Delay;

	rslt = bme280_init(&bme280);
	if (rslt != BME280_OK)
	{
		snprintf(msg, sizeof(msg), "BME280 init failed (code %d)\r\n", rslt);
		Terminal_Print(msg);
		Error_Handler();
	}
	Terminal_Print("BME280 initialized successfully!\r\n");

	settings.osr_t        = BME280_OVERSAMPLING_1X;
	settings.osr_p        = BME280_OVERSAMPLING_1X;
	settings.osr_h        = BME280_OVERSAMPLING_1X;
	settings.filter       = BME280_FILTER_COEFF_OFF;
	settings.standby_time = BME280_STANDBY_TIME_1000_MS;

	rslt = bme280_set_sensor_settings(BME280_SEL_OSR_PRESS |
	                                  BME280_SEL_OSR_TEMP  |
	                                  BME280_SEL_OSR_HUM   |
	                                  BME280_SEL_FILTER    |
	                                  BME280_SEL_STANDBY,
	                                  &settings,
	                                  &bme280);
	if (rslt != BME280_OK)
	{
		snprintf(msg, sizeof(msg), "BME280 settings error (code %d)\r\n", rslt);
		Terminal_Print(msg);
		Error_Handler();
	}

	rslt = bme280_set_sensor_mode(BME280_POWERMODE_NORMAL, &bme280);
	if (rslt != BME280_OK)
	{
		snprintf(msg, sizeof(msg), "BME280 mode error (code %d)\r\n", rslt);
		Terminal_Print(msg);
		Error_Handler();
	}

	Terminal_Print("BME280 ready to measure!\r\n");
}

/* Returns 0 on success, non-zero on failure. sensor_data is only
 * trusted by the caller when this returns 0. */
static int8_t BME280_ReadData(void)
{
	int8_t r;

	i2c_diag_valid = 0;   /* forget any older failure */
	r = bme280_get_sensor_data(BME280_ALL, &sensor_data, &bme280);

	if (r == BME280_OK)
	{
		snprintf(msg, sizeof(msg),
		         "\r\n"
		         "-----------------------------\r\n"
		         "Temperature : %.2f C\r\n"
		         "Pressure    : %.2f hPa\r\n"
		         "Humidity    : %.2f %%RH\r\n"
		         "-----------------------------\r\n",
		         sensor_data.temperature,
		         sensor_data.pressure / 100.0,
		         sensor_data.humidity);
		Terminal_Print(msg);
	}
	else
	{
		snprintf(msg, sizeof(msg), "Sensor reading failed (Bosch code %d)\r\n", r);
		Terminal_Print(msg);
		I2C_PrintDiag();
	}

	return r;
}

#if ENABLE_ESP
/* Best-effort search for an MQTT CONNACK packet (20 02 00 00 = accepted).
 * Not seeing it is NOT proof of failure (the library may have consumed
 * the bytes elsewhere), but seeing it IS good evidence the broker accepted us. */
static uint8_t Find_Connack(const char *buf, uint16_t len)
{
	uint16_t i;

	if (len < 4U)
	{
		return 0;
	}

	for (i = 0; i <= (uint16_t)(len - 4U); i++)
	{
		if (((uint8_t)buf[i]     == 0x20U) &&
		    ((uint8_t)buf[i + 1] == 0x02U) &&
		    ((uint8_t)buf[i + 2] == 0x00U) &&
		    ((uint8_t)buf[i + 3] == 0x00U))
		{
			return 1;
		}
	}
	return 0;
}
#endif

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
  MX_USART1_UART_Init();
  MX_I2C1_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */

	Terminal_Print("\r\n==============================\r\n");
	Terminal_Print("   STM32 BME280 MQTT MONITOR\r\n");
	Terminal_Print("==============================\r\n");

	/* ---- Step 1: find the BME280 on I2C (0x76 or 0x77) ---- */
	Terminal_Print("[1] Looking for BME280...\r\n");

	if (HAL_I2C_IsDeviceReady(&hi2c1, (uint16_t)(0x76 << 1), 3, 100) == HAL_OK)
	{
		bme280_address = 0x76;
	}
	else if (HAL_I2C_IsDeviceReady(&hi2c1, (uint16_t)(0x77 << 1), 3, 100) == HAL_OK)
	{
		bme280_address = 0x77;
	}
	else
	{
		Terminal_Print("BME280 NOT DETECTED! Check wiring (SCL=PB6, SDA=PB7, power, GND).\r\n");
		Error_Handler();
	}

	snprintf(msg, sizeof(msg), "BME280 found at address 0x%02X\r\n", bme280_address);
	Terminal_Print(msg);

	/* ---- Step 2: initialise the sensor ---- */
	BME280_Init();
	HAL_Delay(1100);   /* let the first measurement complete */

#if ENABLE_ESP
	/* ---- Step 3: ESP8266. These library calls retry forever if the
	 * module does not answer, so the LAST message you see in the
	 * terminal tells you which step is stuck. ---- */
	Terminal_Print("[2] ESP8266 AT test (stuck here = ESP wiring/power/firmware)...\r\n");
	ESP8266_Init(StationMode, huart1);
	Terminal_Print("    ESP8266 responded to AT.\r\n");

	Terminal_Print("[3] Connecting to Wi-Fi (stuck here = wrong SSID/password or 5 GHz)...\r\n");
	ESP8266_wifiConnect(WIFI_SSID, WIFI_PASSWORD);
	Terminal_Print("    Wi-Fi step finished.\r\n");

	Terminal_Print("[4] Opening TCP connection to broker...\r\n");
	ESP8266_portConnect("TCP", MQTT_BROKER_HOST, MQTT_BROKER_PORT);
	Terminal_Print("    TCP step finished.\r\n");

	/* ---- Step 4: MQTT CONNECT ---- */
	mqttConfig.clientID    = MQTT_CLIENT_ID;
	mqttConfig.username    = NULL;
	mqttConfig.password    = NULL;
	mqttConfig.keepAlive   = 60;
	mqttConfig.connectFlag = CleanSession;

	Terminal_Print("[5] Sending MQTT CONNECT...\r\n");
	MQTT_connectBroker(&mqttConfig);
	HAL_Delay(1000);

	if (Find_Connack(Buffer, BUFFERSIZE) || Find_Connack(mqttBuffer, BUFFERSIZE))
	{
		Terminal_Print("    MQTT CONNACK seen: broker accepted the connection.\r\n");
	}
	else
	{
		Terminal_Print("    CONNACK not seen (not proof of failure). Check MQTTX for data.\r\n");
	}
#else
	Terminal_Print("ENABLE_ESP = 0: sensor-only test, nothing is published.\r\n");
#endif

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

	if (BME280_ReadData() == 0)
	{
#if ENABLE_ESP
		snprintf(mqttMessage, sizeof(mqttMessage),
		         "{\"temperature\":%.2f,\"pressure\":%.2f,\"humidity\":%.2f}",
		         sensor_data.temperature,
		         sensor_data.pressure / 100.0,
		         sensor_data.humidity);

		MQTT_publishTopic(mqttTopic, mqttMessage, QoS0);
		Terminal_Print("MQTT publish function called.\r\n");
#endif
	}
	else
	{
		Terminal_Print("Skipping publish: no valid reading.\r\n");
	}

	HAL_Delay(PUBLISH_PERIOD_MS);

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
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 84;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
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