#include "main.h"
#include "serialImageTransmittion.h"
enum TransferState transferState = READY;
uint32_t pixelPosition = 0;
uint32_t maxPixelPosition = 2 * 320 * 480;
UART_HandleTypeDef huart2;
uint8_t* rx_buffer;
/**
  * Enable DMA controller clock
  */
/*
static void MX_DMA_Init(void)
{


  __HAL_RCC_DMA1_CLK_ENABLE();
  HAL_NVIC_SetPriority(DMA1_Channel6_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel6_IRQn);

}
*/


static void MX_USART2_UART_Init(void)
{
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 1152000; // !!!!
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
}
void InitUSB()
{
	rx_buffer = (uint8_t *)malloc(sizeof(uint8_t) * RX_BUFFER_SIZE);
	MX_USART2_UART_Init();
}
void defineLCDDrawArea()
{
	// telling LCD: start drawing IN 0,0,480,320 rect
	CommandLCD(0x3A);
	Write8ToLCD(0x55);	// 16-bit RGB565

	CommandLCD(ILI9341_COLADDRSET);
	Write16ToLCD(0);
	Write16ToLCD(480);

	CommandLCD(ILI9341_PAGEADDRSET);
	Write16ToLCD(0);
	Write16ToLCD(320);
	CommandLCD(ILI9341_MEMORYWRITE);

}

void transmitReadyMessage()
{
	if (transferState == READY)
	{
		uint8_t* readyMessage = (uint8_t*)"READY";
		HAL_UART_Transmit(&huart2, readyMessage, sizeof(uint8_t) * 5, HAL_MAX_DELAY);
	}
}
void drawIncomingImage()
{
	HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, SET);
	HAL_UART_Receive(&huart2, rx_buffer, RX_BUFFER_SIZE, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, RESET);

	transferState = BUSY;
	pixelPosition = 0;
	while (pixelPosition < RX_BUFFER_SIZE /*&& pixelPosition + (uint32_t)RX_BUFFER_SIZE < maxPixelPosition*/)
	{
		Write8ToLCD(rx_buffer[pixelPosition]);
		pixelPosition++;
	}
	transferState = READY;
}
