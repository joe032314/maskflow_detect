/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <stdio.h>
#include <stdlib.h>

#ifdef __GNUC__
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */	

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
UART_HandleTypeDef UartHandle;
static void SystemClock_Config(void);

static void Uart_Init(void);

void I2C_Init(void);

int main(void)
{
  HAL_Init();
  SystemClock_Config();
	Uart_Init();
	I2C_Init();
	
	

	printf("Test Uart \r\n");
  while (1)
  {

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
	GPIO_InitTypeDef GPIO_InitStruct;
	I2C_HandleTypeDef I2C_Master_Handle;
	I2C_HandleTypeDef I2C_Slave_Handle;
	
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_I2C1_CLK_ENABLE();
	__HAL_RCC_I2C2_CLK_ENABLE();
	
	//PB6 : SCL
	//PB7 : SDA
	GPIO_InitStruct.Pin				=	GPIO_PIN_6 | GPIO_PIN_7;
	GPIO_InitStruct.Mode			=	GPIO_MODE_AF_OD;
	GPIO_InitStruct.Pull			=	GPIO_PULLUP;
	GPIO_InitStruct.Speed 		= GPIO_SPEED_FAST;
	GPIO_InitStruct.Alternate =	GPIO_AF4_I2C1;
	
	HAL_GPIO_Init(GPIOB,&GPIO_InitStruct);
	
	#if 1 //Test SDA GPIO
	GPIO_InitStruct.Pin				=	GPIO_PIN_7;
	GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
	HAL_GPIO_Init(GPIOB,&GPIO_InitStruct);
	#endif
	
  I2C_Master_Handle.Instance             = I2C1;
  
  I2C_Master_Handle.Init.AddressingMode  = I2C_ADDRESSINGMODE_10BIT;
  I2C_Master_Handle.Init.ClockSpeed      = 400000;
  I2C_Master_Handle.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  I2C_Master_Handle.Init.DutyCycle       = I2C_DUTYCYCLE_16_9;
  I2C_Master_Handle.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  I2C_Master_Handle.Init.NoStretchMode   = I2C_NOSTRETCH_DISABLE;
  I2C_Master_Handle.Init.OwnAddress1     = 0x30F; //TEST ADDRESS
  I2C_Master_Handle.Init.OwnAddress2     = 0xFE;	
	
	//PB10 : SCL
	//PB11 : SDA
	
	GPIO_InitStruct.Pin				=	GPIO_PIN_10 | GPIO_PIN_11;
	GPIO_InitStruct.Mode			=	GPIO_MODE_AF_OD;
	GPIO_InitStruct.Pull			=	GPIO_PULLUP;
	GPIO_InitStruct.Speed 		= GPIO_SPEED_FAST;
	GPIO_InitStruct.Alternate =	GPIO_AF4_I2C2;	
	
	HAL_GPIO_Init(GPIOB,&GPIO_InitStruct);

  I2C_Slave_Handle.Instance             = I2C2;
  
  I2C_Slave_Handle.Init.AddressingMode  = I2C_ADDRESSINGMODE_10BIT;
  I2C_Slave_Handle.Init.ClockSpeed      = 400000;
  I2C_Slave_Handle.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  I2C_Slave_Handle.Init.DutyCycle       = I2C_DUTYCYCLE_16_9;
  I2C_Slave_Handle.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  I2C_Slave_Handle.Init.NoStretchMode   = I2C_NOSTRETCH_DISABLE;
  I2C_Slave_Handle.Init.OwnAddress1     = 0x30F; //TEST ADDRESS
  I2C_Slave_Handle.Init.OwnAddress2     = 0xFE;	

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

