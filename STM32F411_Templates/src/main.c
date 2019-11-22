/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include "stm32f411e_discovery.h"
#ifdef __GNUC__
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */	
#define MASTER_BOARD
#define I2C_ADDRESS        0x30F
#define I2C_DataSize			 1
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void I2C_Init(void);
static void Uart_Init(void);
static void Error_Handler(void);
static I2C_HandleTypeDef    I2cHandle;
UART_HandleTypeDef UartHandle;
uint8_t	TxBuffer[I2C_DataSize] = {100};
uint8_t RxBuffer[I2C_DataSize] ;


int main(void)
{
  HAL_Init();
  SystemClock_Config();
	Uart_Init();
	I2C_Init();
	BSP_LED_Init(LED3);
	BSP_LED_Init(LED4);
	BSP_LED_Init(LED5);
	BSP_LED_Init(LED6);
	
	BSP_PB_Init(BUTTON_KEY,BUTTON_MODE_GPIO);
	printf("Test Uart \r\n");
	

	
  while (1)
  {
		#ifdef MASTER_BOARD
		if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)==1)
		{
			printf("Transmit Data: %d\r\n",TxBuffer[0]);
			HAL_Delay(200);
			HAL_I2C_Master_Transmit(&I2cHandle, (uint16_t)I2C_ADDRESS, (uint8_t*)TxBuffer, 1, 0xffff);
			BSP_LED_On(LED3);
		}
		#endif
  }
}
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART1 and Loop until the end of transmission */
  HAL_UART_Transmit(&UartHandle, (uint8_t *)&ch, 1, 0xFFFF);

  return ch;
}


void I2C_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct ;
	
	__HAL_RCC_I2C1_CLK_ENABLE();
	__HAL_RCC_I2C2_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	
	/*PB6,PB7 */
	GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Alternate =	GPIO_AF4_I2C1;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	I2cHandle.Instance             = I2C1;
  I2cHandle.Init.AddressingMode  = I2C_ADDRESSINGMODE_7BIT;
  I2cHandle.Init.ClockSpeed      = 400000;
  I2cHandle.Init.DualAddressMode = I2C_DUALADDRESS_DISABLED;
  I2cHandle.Init.DutyCycle       = I2C_DUTYCYCLE_2;
  I2cHandle.Init.GeneralCallMode = I2C_GENERALCALL_DISABLED;
  I2cHandle.Init.NoStretchMode   = I2C_NOSTRETCH_DISABLED;
  I2cHandle.Init.OwnAddress1     = I2C_ADDRESS;
  I2cHandle.Init.OwnAddress2     = 0x00;
	
	/*PB8,PB9*/
	GPIO_InitStruct.Pin = GPIO_PIN_8 | GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Alternate =	GPIO_AF4_I2C1;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	I2cHandle.Instance             = I2C2;
  I2cHandle.Init.AddressingMode  = I2C_ADDRESSINGMODE_7BIT;
  I2cHandle.Init.ClockSpeed      = 400000;
  I2cHandle.Init.DualAddressMode = I2C_DUALADDRESS_DISABLED;
  I2cHandle.Init.DutyCycle       = I2C_DUTYCYCLE_2;
  I2cHandle.Init.GeneralCallMode = I2C_GENERALCALL_DISABLED;
  I2cHandle.Init.NoStretchMode   = I2C_NOSTRETCH_DISABLED;
  I2cHandle.Init.OwnAddress1     = I2C_ADDRESS;
  I2cHandle.Init.OwnAddress2     = 0x00;
	
  if(HAL_I2C_Init(&I2cHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();    
  }
}


void Uart_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct ;
	
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_USART2_CLK_ENABLE();
	
	/*SET UART*/
	UartHandle.Instance          = USART2;
  
  UartHandle.Init.BaudRate     = 115200;
  UartHandle.Init.WordLength   = UART_WORDLENGTH_8B;
  UartHandle.Init.StopBits     = UART_STOPBITS_1;
  UartHandle.Init.Parity       = UART_PARITY_NONE;
  UartHandle.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
  UartHandle.Init.Mode         = UART_MODE_TX_RX;
  UartHandle.Init.OverSampling = UART_OVERSAMPLING_16;
	
	HAL_UART_Init(&UartHandle);
	
	/*PA2 TX*/
	GPIO_InitStruct.Pin = GPIO_PIN_2;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
	HAL_GPIO_Init(GPIOA,&GPIO_InitStruct);	
	/*PA3 RX*/
	GPIO_InitStruct.Pin =GPIO_PIN_3;
	GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
	HAL_GPIO_Init(GPIOA,&GPIO_InitStruct);
}	
	
static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;

  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();
  
  /* The voltage scaling allows optimizing the power consumption when the device is 
     clocked below the maximum system frequency, to update the voltage scaling value 
     regarding system frequency refer to product datasheet.  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);
  
  /* Enable HSI Oscillator and activate PLL with HSI as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 0x10;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 400;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);
 
  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;  
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3);
}
static void Error_Handler(void)
{
  /* Turn LED5 on */
  BSP_LED_On(LED5);
  while(1)
  {
  }
}
