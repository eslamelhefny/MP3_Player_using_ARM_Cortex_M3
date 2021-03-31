/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "memory.h"
#include "LCD_int.h"
/* FreeRTOS Main Header files */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"


/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;
ADC_HandleTypeDef hadc2;

UART_HandleTypeDef huart1;

/* Definitions for defaultTask */
/*osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 128 * 4
};*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_ADC1_Init(void);
static void MX_ADC2_Init(void);





xTaskHandle				t1_handle = NULL;
xTaskHandle				t2_handle = NULL;
xTaskHandle				t3_handle = NULL;
xTaskHandle				t4_handle = NULL;
xTaskHandle				t5_handle = NULL;


#define     E1         (1<<0)
#define     E2         (1<<1)
#define     E3         (1<<2)
#define     E4         (1<<3)
#define     E5         (1<<4)
#define     E6         (1<<5)
#define     E7         (1<<6)
#define     E8         (1<<7)
#define     E_Alarm    (1<<8)
#define     E_LED      (1<<9)


EventGroupHandle_t         egSym;

EventBits_t                egBits;

xSemaphoreHandle       bsUartRes = NULL;
void system_init(void);
void T_Alarm(void*pvParam);
void T_TempSens(void*pvParam);
void T_Check_TEMP(void*pvParam);
void T_Display(void*pvParam);
void T_UserTerm(void*pvParam);


typedef enum{
	SSMain=0,
	SSConfig,
	SSAlarm,
	SSLED_ON
}SystemState_t;
typedef struct{
	uint8_t C;        // TEMPREATURE
	uint8_t L;        // LDR
	uint8_t T;          // THRESHOLD
	uint8_t I;          // INTENSITY
	uint8_t AS;           //ALARM STATUS
	SystemState_t   SystemState;
}TempAlarmState;
TempAlarmState TAS;

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint8_t SHOWTRSH [4];  //BUFFER TO CHANGE INTEGER TO ASCII USING (ITOA)
uint8_t SHOWPOT [4];  //BUFFER TO CHANGE INTEGER TO ASCII USING (ITOA)
uint8_t SHOWLDR [4];  //BUFFER TO CHANGE INTEGER TO ASCII USING (ITOA)
uint8_t buff[4]={0};   //BUFFER TO STORE  UART STRINGS
uint8_t count=0;// COUNT USED IN USER TRMINAL TO CHECK FOR DIGITS NOT PASSED THE COUNT
uint8_t buff2[4]={0};      //USED TO STORE DIGITS RECEIVE FROM UART
uint8_t buff3[20]={0};      //USED TO STORE DIGITS RECEIVE FROM UART
uint8_t AD_RES[4];     //ADC RESULT
uint8_t AD_RES_ldr[4];     //ADC RESULT
uint16_t i=0;     //ADC RESULT


/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{

	/*init preperals*/
	system_init();

	HAL_ADCEx_Calibration_Start(&hadc1);

	TAS.AS='E';   //ALRAM STATUS ENABLE
	TAS.T=20;     //THRESHOLD 20
	TAS.I=30;     //INTENSITY 30
	TAS.SystemState=SSMain;



	vSemaphoreCreateBinary(bsUartRes);

	egSym=xEventGroupCreate();

	//HAL_UART_Transmit(&huart1, "Enter Password:", 14, 1000);
	LCD_vidWriteString("Enter Password");

	while (i<1000000){
		HAL_UART_Receive( &huart1, buff3, 12, 200);
		i++;

		if( strcmp( buff3, "Welcome 1234" ) == 0 ){
			LCD_vidClear();
			LCD_vidWriteString("Welcome to Home");
			HAL_Delay(3000);

			/* Create Tasks*/
			xTaskCreate(T_Alarm,"Alarm",100,(void*)"T_T1 Started ...\r\n",5,&t1_handle);
			xTaskCreate(T_TempSens,"TempSens",100,(void*)"T_T2 Started ...\r\n",2,&t2_handle);
			xTaskCreate(T_Check_TEMP,"Check",100,(void*)"T_T3 Started ...\r\n",4,&t3_handle);
			xTaskCreate(T_Display,"Display",100,(void*)"T_T4 Started ...\r\n",1,&t4_handle);
			xTaskCreate(T_UserTerm,"UserTerm",100,(void*)"T_T5 Started ...\r\n",3,&t5_handle);


			egBits=xEventGroupSetBits(egSym, E1);

			/* Start RTOS */
			vTaskStartScheduler();


		}
	}

	//}
	while(1){
		asm("NOP");
	}




}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void system_init(void){

	HAL_Init();

	/* Configure the system clock */
	SystemClock_Config();

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_USART1_UART_Init();
	MX_ADC1_Init();
	MX_ADC2_Init();
	LCD_vidInit ();
	HAL_UART_Transmit(&huart1, "system Started.....\n", 20, 1000);


}



void T_Alarm(void*pvParam){
	//	HAL_UART_Transmit(&huart1, "Asystem Started.....\n", 21, 1000);

	while(1){
		egBits=xEventGroupWaitBits(egSym, (E_Alarm|E_LED), 1, 0, portMAX_DELAY);
		if(((egBits& E_Alarm) == E_Alarm)){
			//HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_6);
			HAL_GPIO_WritePin( GPIOB, GPIO_PIN_6, GPIO_PIN_SET);
			vTaskDelay(500);
			HAL_GPIO_WritePin( GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);
			vTaskDelay(500);
		}
		if(((egBits& E_LED) == E_LED)){
			//HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_6);
			HAL_GPIO_WritePin( GPIOB, GPIO_PIN_8, GPIO_PIN_SET);
		}



	}


}


void T_TempSens(void*pvParam){
	HAL_UART_Transmit(&huart1, "Tsystem Started.....\n", 21, 1000);
	//if(flag == 0)
	//{
	//	flag = 1;

	while(1){

		// Start ADC Conversion
		HAL_ADC_Start(&hadc1);
		HAL_ADC_Start(&hadc2);
		// Poll ADC1 Perihperal & TimeOut = 1mSec
		HAL_ADC_PollForConversion(&hadc1, 1);
		HAL_ADC_PollForConversion(&hadc2, 1);
		// Read The ADC Conversion Result
		AD_RES[0] = ((HAL_ADC_GetValue(&hadc1))*33) / 4096;
		AD_RES_ldr[0] = ((HAL_ADC_GetValue(&hadc2))*33) / 4096;
		TAS.C = AD_RES[0];
		TAS.L = AD_RES_ldr[0];

		if(TAS.SystemState == SSMain){
			egBits=xEventGroupSetBits(egSym, E4);
			egBits=xEventGroupSetBits(egSym, E7);
			xSemaphoreGive(bsUartRes);

		}
		else{

			//timeout
		}
		vTaskDelay(100);
	}


}



void T_Check_TEMP(void*pvParam){
	HAL_UART_Transmit(&huart1, "Csystem Started.....\n", 21, 1000);


	while(1){
		if(xSemaphoreTake(bsUartRes,portMAX_DELAY)){

			if(((TAS.SystemState == SSMain)||(TAS.SystemState == SSAlarm)) && ((TAS.C) >= (TAS.T)) && (TAS.AS == 'E')){
				TAS.SystemState = SSAlarm;
				egBits=xEventGroupSetBits(egSym, E3);
				egBits=xEventGroupSetBits(egSym, E_Alarm);
				xSemaphoreGive(bsUartRes);

			}
			if(((TAS.SystemState == SSAlarm)) && (((TAS.C) < (TAS.T)) || (TAS.AS == 'D'))){
				egBits=xEventGroupClearBits(egSym, E_Alarm);
				TAS.SystemState = SSMain;
				egBits=xEventGroupSetBits(egSym, E1|E4|E5|E6|E7);
				xSemaphoreGive(bsUartRes);

			}
			if(((TAS.SystemState == SSMain)||(TAS.SystemState == SSLED_ON)) && ((TAS.L) >= (TAS.I))){
				TAS.SystemState = SSLED_ON;

				egBits=xEventGroupSetBits(egSym, E8);
				egBits=xEventGroupSetBits(egSym, E_LED);
				xSemaphoreGive(bsUartRes);
			}
			if(((TAS.SystemState == SSLED_ON)) && ((TAS.L) < (TAS.I))){
				HAL_GPIO_WritePin( GPIOB, GPIO_PIN_8, GPIO_PIN_RESET);
				egBits=xEventGroupClearBits(egSym, E_LED);
				//egBits=xEventGroupClearBits(egSym, E8);
				TAS.SystemState = SSMain;


				egBits=xEventGroupSetBits(egSym, E1|E4|E5|E6|E7);
				xSemaphoreGive(bsUartRes);
			}


		}
		vTaskDelay(100);

	}

}


void T_Display(void*pvParam){
	HAL_UART_Transmit(&huart1, "Dsystem Started.....\n", 21, 1000);

	while(1){
		egBits=xEventGroupWaitBits(egSym, (E1|E2|E3|E4|E5|E6|E7|E8), 1, 0, portMAX_DELAY);
		if(((egBits& E1) == E1)){
			//	LCD_vidClear( );
			LCD_vidGotoXY( 1 , '\0' );
			LCD_vidWriteString("C:00  T:20  AS:E ");
			LCD_vidGotoXY( 1 , 8 );
			/*itoa((TAS.T)/10,SHOWTRSH,10);
			LCD_vidWriteString(SHOWTRSH);
			itoa((TAS.T)%10,SHOWTRSH,10);
			LCD_vidWriteString(TAS.T);*/
			LCD_vidGotoXY( 2 , '\0' );
			LCD_vidWriteString("L:00 I:30 PR:C/T");

		}

		else if(((egBits& E2) == E2)){
			//	LCD_vidClear( );
			LCD_vidGotoXY( 1 , '\0' );
			LCD_vidWriteString("      T:00       ");
			LCD_vidGotoXY( 2 , '\0' );
			LCD_vidWriteString("   OK:K     CN:N    ");
		}
		else if(((egBits& E3) == E3)){
			LCD_vidGotoXY( 1 , '\0' );
			LCD_vidWriteString("  !!!!Alarm!!!!  ");
			LCD_vidGotoXY( 2 , '\0' );
			LCD_vidWriteString("      SA:S      ");
		}
		else if(((egBits& E4) == E4)){
			LCD_vidGotoXY( 1 , 2 );
			itoa((TAS.C)/10,SHOWPOT,10);
			LCD_vidWriteString(SHOWPOT);
			itoa((TAS.C)%10,SHOWPOT,10);
			LCD_vidWriteString(SHOWPOT);

		}
		else if(((egBits& E5) == E5)){
			LCD_vidGotoXY( 1 , 8 );
			itoa((TAS.T)/10,SHOWTRSH,10);
			LCD_vidWriteString(SHOWTRSH);
			itoa((TAS.T)%10,SHOWTRSH,10);
			LCD_vidWriteString(TAS.T);

		}
		else if(((egBits& E6) == E6)){
			LCD_vidGotoXY( 1 , 15 );
			LCD_vidWriteChar(TAS.AS);

		}
		else if(((egBits& E7) == E7)){
			LCD_vidGotoXY( 2 , 2 );
			itoa((TAS.L)/10,SHOWLDR,10);
			LCD_vidWriteString(SHOWLDR);
			itoa((TAS.L)%10,SHOWPOT,10);
			LCD_vidWriteString(SHOWLDR);


		}
		else if(((egBits& E8) == E8)){
			//

			LCD_vidGotoXY( 1 , '\0' );
			LCD_vidWriteString("   LIGHT ON !       ");
			LCD_vidGotoXY( 2 , '\0' );
			LCD_vidWriteString("   LIGHT ON !       ");

		}
		else{

			//timeout
		}
	}

}

void T_UserTerm(void*pvParam){

	HAL_UART_Transmit(&huart1, "Usystem Started.....\n", 21, 1000);
	//uint8_t count=0;

	while(1){
		HAL_UART_Receive( &huart1, buff, 2, 200);

		if((TAS.SystemState==SSMain)){
			switch(*buff){
			case 'C': egBits=xEventGroupSetBits(egSym, E2);
			TAS.SystemState=SSConfig;
			memset(buff, 0, sizeof buff);
			break;

			case 'T':if((TAS.AS=='E')){
				TAS.AS='D';
				egBits=xEventGroupSetBits(egSym, E6);
				xSemaphoreGive(bsUartRes);
				memset(buff, 0, sizeof buff);
				break;
			}else if((TAS.AS=='D')){
				TAS.AS='E';
				egBits=xEventGroupSetBits(egSym, E6);
				xSemaphoreGive(bsUartRes);
				memset(buff, 0, sizeof buff);
				break;
			}

			default:;
			}
		}

		if((TAS.SystemState==SSConfig)){

			if((buff[count] >= '0' && buff[count] <= '9')&&count<3){
				buff2[count]=buff[count];
				count ++;

			}

			if(count>=3){
				TAS.T=*buff2;
				//itoa((TAS.T)/10,SHOWTRSH,10);
				//	itoa((TAS.T)%10,SHOWTRSH,10);
				//TAS.T=SHOWTRSH;
				//count++;

				//memset(buff, 0, sizeof buff);
			}
			egBits=xEventGroupSetBits(egSym, E5);
			switch (*buff){
			case 'N': TAS.SystemState=SSMain;
			egBits=xEventGroupSetBits(egSym, E1|E4|E5|E6|E7);
			memset(buff, 0, sizeof buff);
			break;
			case 'k':
				TAS.T=buff2[0];
				TAS.SystemState=SSMain;
				egBits=xEventGroupSetBits(egSym, E1|E4|E5|E6|E7);
				memset(buff, 0, sizeof buff);
				break;
			default:;
			}

		} if((TAS.SystemState==SSAlarm)){
			if(*buff=='S'){
				TAS.AS='D';
				xSemaphoreGive(bsUartRes);
				//egBits=xEventGroupClearBits(egSym, E_Alarm);
				//HAL_GPIO_WritePin( GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);
				//	TAS.SystemState = SSMain;
				//egBits=xEventGroupSetBits(egSym, E1|E4|E5|E6);
			}
			memset(buff, 0, sizeof buff);

		}



		vTaskDelay(50);
	}

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
	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
	PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV2;
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
	{
		Error_Handler();
	}
}

/**
 * @brief ADC1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_ADC1_Init(void)
{

	/* USER CODE BEGIN ADC1_Init 0 */

	/* USER CODE END ADC1_Init 0 */

	ADC_ChannelConfTypeDef sConfig = {0};

	/* USER CODE BEGIN ADC1_Init 1 */

	/* USER CODE END ADC1_Init 1 */
	/** Common config
	 */
	hadc1.Instance = ADC1;
	hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
	hadc1.Init.ContinuousConvMode = DISABLE;
	hadc1.Init.DiscontinuousConvMode = DISABLE;
	hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadc1.Init.NbrOfConversion = 1;
	if (HAL_ADC_Init(&hadc1) != HAL_OK)
	{
		Error_Handler();
	}
	/** Configure Regular Channel
	 */
	sConfig.Channel = ADC_CHANNEL_0;
	sConfig.Rank = ADC_REGULAR_RANK_1;
	sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN ADC1_Init 2 */

	/* USER CODE END ADC1_Init 2 */

}

/**
 * @brief ADC2 Initialization Function
 * @param None
 * @retval None
 */
static void MX_ADC2_Init(void)
{

	/* USER CODE BEGIN ADC2_Init 0 */

	/* USER CODE END ADC2_Init 0 */

	ADC_ChannelConfTypeDef sConfig = {0};

	/* USER CODE BEGIN ADC2_Init 1 */

	/* USER CODE END ADC2_Init 1 */
	/** Common config
	 */
	hadc2.Instance = ADC2;
	hadc2.Init.ScanConvMode = ADC_SCAN_DISABLE;
	hadc2.Init.ContinuousConvMode = DISABLE;
	hadc2.Init.DiscontinuousConvMode = DISABLE;
	hadc2.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	hadc2.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadc2.Init.NbrOfConversion = 1;
	if (HAL_ADC_Init(&hadc2) != HAL_OK)
	{
		Error_Handler();
	}
	/** Configure Regular Channel
	 */
	sConfig.Channel = ADC_CHANNEL_9;
	sConfig.Rank = ADC_REGULAR_RANK_1;
	sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
	if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
	{
		Error_Handler();
	}


}


static void MX_USART1_UART_Init(void)
{


	huart1.Instance = USART1;
	huart1.Init.BaudRate = 9600;
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


}


static void MX_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4
			|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8
			|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_15, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8, GPIO_PIN_RESET);

	/*Configure GPIO pins : PA1 PA2 PA3 PA4
                           PA5 PA6 PA7 PA8
                           PA11 PA12 PA15 */
	GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4
			|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8
			|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_15;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/*Configure GPIO pin : PB12 */
	GPIO_InitStruct.Pin = GPIO_PIN_12;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/*Configure GPIO pins : PB6 PB7 */
	GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartDefaultTask */
/**
 * @brief  Function implementing the defaultTask thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
	/* USER CODE BEGIN 5 */
	/* Infinite loop */
	for(;;)
	{
		osDelay(1);
	}
	/* USER CODE END 5 */
}

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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
