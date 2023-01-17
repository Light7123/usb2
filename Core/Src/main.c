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
#include "usart.h"
#include "usb_host.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdarg.h"
#include "usbh_cdc.h"
#include "../2can/hw.h"
#include "../2can/PaymentController.h"
#include "../2can/WebObject.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef enum {
	UART_RX_0 = 48,
	UART_RX_1,
	UART_RX_2,
	UART_RX_NONE = 0xFF,
}UARTCommandTypeDef;

#define RXBUFFERSIZE                     64


/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define DEBUG_ENABLED 					1
#define AUTH                    		1
#define GET_READER_INFO         		1
#define TEST_LINKED_CARD 				0
#define TEST_CARD_PAYMENT 				1
#define TEST_SCHEDULE					0
#define TEST_REVERSE					0
#define TEST_PURCHASES					0
#define TEST_PRODUCT					0
#define TEST_TAGS						0
#define TEST_TRANSACTION				0
#define TEST_FISCALIZE					0
#define TEST_PREPARE					0
#define TEST_REVERSE_LAST_TRANSACTION 	0

#define EMAIL            				""
#define SECRET_KEY					""
#define AMOUNT							23.0
#define AMOUNT_REVERSE					0.0
#define TRANSACTION_ID					"CBEC0494-6938-489E-B3FA-FC829C6EAAB0"
#define RECEIPT_EMAIL					"test@mail.de"
#define RECEIPT_PHONE					"+73211232312"
#define ACQUIRER_CODE					NULL		//"ACQ_CODE"
#define PRODUCT_FIELD_TEST_DATA			"field data"
#define PRINTER_TAPE_WIDTH_CHARS		32

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

static char uart_tx_buf[250];
int uart_buf_len;
int isBleuIOReady;
static UARTCommandTypeDef uartStatus;
static uint8_t aRxBuffer[RXBUFFERSIZE];
static int RX_value;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void MX_USB_HOST_Process(void);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void UARTPrint(const char *args, ...)
{
	char StrBuff[256];
	va_list ap;
	va_start(ap, args);
	char len = vsnprintf(StrBuff, sizeof(StrBuff), args, ap);
	va_end(ap);
	HAL_UART_Transmit(&huart5,(uint8_t*)StrBuff,len,100);
}

//extern USBH_HandleTypeDef hUsbHostFS;
extern ApplicationTypeDef Appli_state;
extern USBH_StatusTypeDef usbresult;

#define RX_BUFF_SIZE   64  /* Max Received data 1KB */

uint8_t CDC_RX_Buffer[RX_BUFF_SIZE];
uint8_t CDC_TX_Buffer[RX_BUFF_SIZE];

typedef enum {
	CDC_STATE_IDLE = 0,
	CDC_SEND,
	CDC_RECEIVE,
}CDC_StateTypedef;

CDC_StateTypedef CDC_STATE = CDC_STATE_IDLE;

uint8_t i=4;
void CDC_HANDLE (void)
{
	switch (CDC_STATE)
	{
	case CDC_STATE_IDLE:
	{
		USBH_CDC_Stop(&hUsbHostFS);
		int len = sprintf ((char *)CDC_TX_Buffer, "DATA = %d", i);
		if (USBH_CDC_Transmit (&hUsbHostFS, CDC_TX_Buffer, len) == USBH_OK)
		{
			CDC_STATE = CDC_RECEIVE;
		}
		i++;
		break;
	}

	case CDC_RECEIVE:
	{
		USBH_CDC_Stop(&hUsbHostFS);
		usbresult = USBH_CDC_Receive(&hUsbHostFS, (uint8_t *) CDC_RX_Buffer, RX_BUFF_SIZE);
		HAL_Delay (1000);
		CDC_STATE = CDC_STATE_IDLE;
	}

	default:
		break;
	}
}
Ibox_Account *mAccount = NULL;
const char *mTransactionId = NULL;
void startTransactionAction(const char *transactionId)
{
	mTransactionId = transactionId;
	fprintf(stderr, "TransactionID:%s\n", transactionId);
}
int selectEmvApplicationAction(char **emvApplications, int emvApplicationsCount)
{
	fprintf(stderr, "selectEmvApplicationAction\n");
	int selectedIndex = 1;
	for (int i = 0; i < emvApplicationsCount; i++)
		fprintf(stderr, "emv app title: %d - %s\n", i, emvApplications[i]);
	return selectedIndex;
}
void sendUSB(char *buf, int size)
{


		switch(Appli_state)
		{
		case  APPLICATION_IDLE:
			UARTPrint("APPLICATION_IDLE\r\n");
			break;
		case  APPLICATION_DISCONNECT:
			UARTPrint("APPLICATION_DISCONNECT\r\n");
			break;
		case    APPLICATION_START:
			//UARTPrint("  APPLICATION_START\r\n");
			USBH_CDC_Stop(&hUsbHostFS);
			USBH_CDC_Transmit (&hUsbHostFS, buf, size);
			//HAL_UART_Transmit_IT(&huart5, buf, size);
			i++;
			HAL_Delay(1000);
//			USBH_CDC_Receive(&hUsbHostFS,buf, size);
//			UARTPrint("Recive ------------------------------\r\n\n");
//
//			HAL_UART_Transmit_IT(&huart5, buf, size);
			break;
		case APPLICATION_READY:
			UARTPrint("  APPLICATION_READY\r\n");
			USBH_CDC_Stop(&hUsbHostFS);
			USBH_CDC_Transmit (&hUsbHostFS, buf, size);
//			USBH_CDC_Stop(&hUsbHostFS);
//			USBH_CDC_Transmit (&hUsbHostFS, buf, size);
//			HAL_UART_Transmit_IT(&huart5, buf, size);
//			i++;
//			HAL_Delay(1000);
//			USBH_CDC_Receive(&hUsbHostFS,buf, size);
//			UARTPrint("Recive:\r\n");
//
//			HAL_UART_Transmit_IT(&huart5, buf, size);
			break;
		}

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
  MX_USB_HOST_Init();
  MX_UART5_Init();
  /* USER CODE BEGIN 2 */

//	Ibox_Result_Authentication *authResult = NULL;
//	Ibox_PaymentController_SetSendWebRequestAction(&sendWebRequest);
//	Ibox_PaymentController_SetSendReaderRequestAction(&sendReaderRequest);
//	Ibox_PaymentController_SetStartTransactionAction(&startTransactionAction);
//	Ibox_PaymentController_SetSelectEmvApplicationAction(&selectEmvApplicationAction);
//	Ibox_PaymentController_SetCredentials(EMAIL, SECRET_KEY);
//	Ibox_PaymentController_SetDebugEnabled(DEBUG_ENABLED);
//	Ibox_PaymentController_ReaderSoundEnabled(1);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET);
	HAL_UART_Receive_IT(&huart5, "ready", 5);
	int i=0;
	//HAL_UART_Transmit_IT(&huart5, "1\n", 3);
	//HAL_UART_Transmit_IT(&huart5, (uint8_t*)"2", 1);

//	HAL_UART_Transmit_IT(&huart5, "3", 1);
	//HAL_UART_Transmit_IT(&huart5, (uint8_t*)"4", 1);
	//UARTPrint("Getting reader info.\r\n");
	//Ibox_Result_ReaderId *readerIdResult;// = Ibox_PaymentController_ReaderId();

	while (1)
	{
		/* USER CODE END WHILE */
		MX_USB_HOST_Process();
//		switch(Appli_state)
//		{
//		case  APPLICATION_IDLE:
//			UARTPrint("APPLICATION_IDLE\r\n");
//			break;
//		case  APPLICATION_DISCONNECT:
//			UARTPrint("APPLICATION_DISCONNECT\r\n");
//			break;
//		case    APPLICATION_START:
//			//UARTPrint("  APPLICATION_START\r\n");
//			readerIdResult = Ibox_PaymentController_ReaderId();
//			if (!readerIdResult->errorCode)
//			{
//				UARTPrint("ReaderId:%s\r\n", readerIdResult->id);
//
//			}
//			else
//			{
//				UARTPrint("Reader id error.\n");
//			}
//			HAL_Delay(5000);
//			break;
//		case    APPLICATION_READY:
//			UARTPrint("  APPLICATION_READY\r\n");
//			readerIdResult = Ibox_PaymentController_ReaderId();
//			if (!readerIdResult->errorCode)
//			{
//				UARTPrint("ReaderId:%s\r\n", readerIdResult->id);
//
//			}
//			else
//			{
//				UARTPrint("Reader id error.\n");
//			}
//			HAL_Delay(5000);
//			break;
//		}

	}



//	if (!readerIdResult->errorCode)
//	{
//		UARTPrint("ReaderId:%s\r\n", readerIdResult->id);
//
//	}
//	else
//	{
//		UARTPrint("Reader id error.\n");
//	}
//	free(readerIdResult);
//
//	Ibox_Result_ReaderInfo *readerInfoResult = Ibox_PaymentController_ReaderInfo();
//	if (!readerInfoResult->errorCode)
//	{
//		UARTPrint( "BootloaderVersion:%s\n", readerInfoResult->bootloaderVersion);
//		UARTPrint( "FirmwareVersion:%s\n", readerInfoResult->firmwareVersion);
//		UARTPrint("HardwareVersion:%s\n", readerInfoResult->hardwareVersion);
//	}
//	else
//	{
//		UARTPrint("Reader info error.\n");
//	}
//	free(readerInfoResult);
//
//	HAL_Delay(5000);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1)
	{
    /* USER CODE END WHILE */
    MX_USB_HOST_Process();

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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV5;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.Prediv1Source = RCC_PREDIV1_SOURCE_PLL2;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  RCC_OscInitStruct.PLL2.PLL2State = RCC_PLL2_ON;
  RCC_OscInitStruct.PLL2.PLL2MUL = RCC_PLL2_MUL8;
  RCC_OscInitStruct.PLL2.HSEPrediv2Value = RCC_HSE_PREDIV2_DIV5;
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
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLL_DIV3;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure the Systick interrupt time
  */
  __HAL_RCC_PLLI2S_ENABLE();
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM1 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM1) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

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
