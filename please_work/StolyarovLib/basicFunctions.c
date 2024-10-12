#include "basicFunctions.h"

void split16BitTo8Bit(uint16_t input, uint8_t *highByte, uint8_t *lowByte) {
    *highByte = (input >> 8) & 0xFF;
    *lowByte = input & 0xFF;
}
void StopIfBlueButtonWasPressed()
{
	if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == GPIO_PIN_RESET)
	{
		EnterSleepLCD();
//		      	  HAL_UART_DeInit(&huart2);
//		      	  HAL_GPIO_DeInit(GPIOA, 8);
//		      	  __disable_irq();
		HAL_DeInit();
		LD2Init();
		while (1)
		{
			HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
			HAL_Delay(300);
		}
	}
}

void LD2Init()
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	__HAL_RCC_GPIOA_CLK_ENABLE();
	HAL_GPIO_WritePin(GPIOA, LD2_Pin, GPIO_PIN_RESET);

	GPIO_InitStruct.Pin = LD2_Pin;
	  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void write8(uint8_t value)
{
	HAL_GPIO_WritePin(LCD_D0_GPIO_Port, LCD_D0_Pin, (value >> 0) & 1);
	HAL_GPIO_WritePin(LCD_D1_GPIO_Port, LCD_D1_Pin, (value >> 1) & 1);
	HAL_GPIO_WritePin(LCD_D2_GPIO_Port, LCD_D2_Pin, (value >> 2) & 1);
	HAL_GPIO_WritePin(LCD_D3_GPIO_Port, LCD_D3_Pin, (value >> 3) & 1);
	HAL_GPIO_WritePin(LCD_D4_GPIO_Port, LCD_D4_Pin, (value >> 4) & 1);
	HAL_GPIO_WritePin(LCD_D5_GPIO_Port, LCD_D5_Pin, (value >> 5) & 1);
	HAL_GPIO_WritePin(LCD_D6_GPIO_Port, LCD_D6_Pin, (value >> 6) & 1);
	HAL_GPIO_WritePin(LCD_D7_GPIO_Port, LCD_D7_Pin, (value >> 7) & 1);
}

void CommandLCD(uint8_t command)
{
	HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, RESET);
	HAL_GPIO_WritePin(LCD_RS_GPIO_Port, LCD_RS_Pin, RESET);
	write8(command);
	HAL_GPIO_WritePin(LCD_WR_GPIO_Port, LCD_WR_Pin, RESET);
	HAL_GPIO_WritePin(LCD_WR_GPIO_Port, LCD_WR_Pin, SET);
	HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, SET);
}

void Write8ToLCD(uint8_t data) {
    HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, RESET);
    HAL_GPIO_WritePin(LCD_RS_GPIO_Port, LCD_RS_Pin, SET);
    write8(data);
    HAL_GPIO_WritePin(LCD_WR_GPIO_Port, LCD_WR_Pin, RESET);
    HAL_GPIO_WritePin(LCD_WR_GPIO_Port, LCD_WR_Pin, SET);
    HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, SET);
}

void Write16ToLCD(uint16_t data)
{
	uint8_t chunk1 = 0x00;
	uint8_t chunk2 = 0x00;

	split16BitTo8Bit(data, &chunk1, &chunk2);
    HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, RESET);
    HAL_GPIO_WritePin(LCD_RS_GPIO_Port, LCD_RS_Pin, SET);
    write8(chunk1);
    HAL_GPIO_WritePin(LCD_WR_GPIO_Port, LCD_WR_Pin, RESET);
    HAL_GPIO_WritePin(LCD_WR_GPIO_Port, LCD_WR_Pin, SET);
    write8(chunk2);
    HAL_GPIO_WritePin(LCD_WR_GPIO_Port, LCD_WR_Pin, RESET);
    HAL_GPIO_WritePin(LCD_WR_GPIO_Port, LCD_WR_Pin, SET);
    HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, SET);
}


void initLCD() {
	HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, SET);
	HAL_GPIO_WritePin(LCD_RD_GPIO_Port, LCD_RD_Pin, SET);
	HAL_GPIO_WritePin(LCD_WR_GPIO_Port, LCD_WR_Pin, SET);

    HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, RESET);
    HAL_Delay(2);
    HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, SET);

    HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, RESET);
    HAL_GPIO_WritePin(LCD_RS_GPIO_Port, LCD_RS_Pin, RESET);
    write8(0x00);
    // write 0x00 4 times
    HAL_GPIO_WritePin(LCD_WR_GPIO_Port, LCD_WR_Pin, RESET);
    HAL_GPIO_WritePin(LCD_WR_GPIO_Port, LCD_WR_Pin, SET);
    HAL_GPIO_WritePin(LCD_WR_GPIO_Port, LCD_WR_Pin, RESET);
    HAL_GPIO_WritePin(LCD_WR_GPIO_Port, LCD_WR_Pin, SET);
    HAL_GPIO_WritePin(LCD_WR_GPIO_Port, LCD_WR_Pin, RESET);
    HAL_GPIO_WritePin(LCD_WR_GPIO_Port, LCD_WR_Pin, SET);
    HAL_GPIO_WritePin(LCD_WR_GPIO_Port, LCD_WR_Pin, RESET);
    HAL_GPIO_WritePin(LCD_WR_GPIO_Port, LCD_WR_Pin, SET);

    HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, SET);
    HAL_Delay(200);

    CommandLCD(0XF1);
    Write8ToLCD(0x36);
    Write8ToLCD(0x04);
    Write8ToLCD(0x00);
    Write8ToLCD(0x3C);
    Write8ToLCD(0x0F);
    Write8ToLCD(0x8F);

    CommandLCD(0XF2);
    Write8ToLCD(0x18);
    Write8ToLCD(0xA3);
    Write8ToLCD(0x12);
    Write8ToLCD(0x02);
    Write8ToLCD(0XB2);
    Write8ToLCD(0x12);
    Write8ToLCD(0xFF);
    Write8ToLCD(0x10);
    Write8ToLCD(0x00);

    CommandLCD(0XF8);
    Write8ToLCD(0x21);
    Write8ToLCD(0x04);

    CommandLCD(0XF9);
    Write8ToLCD(0x00);
    Write8ToLCD(0x08);

    CommandLCD(0x36);
    Write8ToLCD(0x08);

    CommandLCD(0xB4);
    Write8ToLCD(0x00);

    CommandLCD(0xC1);
    Write8ToLCD(0x41);

    CommandLCD(0xC5);
    Write8ToLCD(0x00);
    Write8ToLCD(0x91);
    Write8ToLCD(0x80);
    Write8ToLCD(0x00);

    CommandLCD(0xE0);
    Write8ToLCD(0x0F);
    Write8ToLCD(0x1F);
    Write8ToLCD(0x1C);
    Write8ToLCD(0x0C);
    Write8ToLCD(0x0F);
    Write8ToLCD(0x08);
    Write8ToLCD(0x48);
    Write8ToLCD(0x98);
    Write8ToLCD(0x37);
    Write8ToLCD(0x0A);
    Write8ToLCD(0x13);
    Write8ToLCD(0x04);
    Write8ToLCD(0x11);
    Write8ToLCD(0x0D);
    Write8ToLCD(0x00);

    CommandLCD(0XE1);
    Write8ToLCD(0x0F);
    Write8ToLCD(0x32);
    Write8ToLCD(0x2E);
    Write8ToLCD(0x0B);
    Write8ToLCD(0x0D);
    Write8ToLCD(0x05);
    Write8ToLCD(0x47);
    Write8ToLCD(0x75);
    Write8ToLCD(0x37);
    Write8ToLCD(0x06);
    Write8ToLCD(0x10);
    Write8ToLCD(0x03);
    Write8ToLCD(0x24);
    Write8ToLCD(0x20);
    Write8ToLCD(0x00);

    CommandLCD(0x3A);
    Write8ToLCD(0x55);

    CommandLCD(0x11);

    CommandLCD(0x36);
    Write8ToLCD(0x28);

    HAL_Delay(120);

    CommandLCD(0x29);
}

void EnterSleepLCD()
{
	CommandLCD(0x28);
	HAL_Delay(10);
	CommandLCD(0x10);
	HAL_Delay(120);
}

void ExitSleepLCD()
{
	CommandLCD(0x11);
	HAL_Delay(120);
	CommandLCD(0x29);
}

void fillRect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
	CommandLCD(0x3A);
	Write8ToLCD(0x55);	// 16-bit RGB565

	CommandLCD(ILI9341_COLADDRSET);
	Write16ToLCD(x1);
	Write16ToLCD(x2);

	CommandLCD(ILI9341_PAGEADDRSET);
	Write16ToLCD(y1);
	Write16ToLCD(y2);

	CommandLCD(ILI9341_MEMORYWRITE);
	for (int i = 0; i < (x2 - x1)* (y2 - y1); i++) {
	    Write16ToLCD(color);
	}
}
void fillScreen()
{
	CommandLCD(0x3A);
	Write8ToLCD(0x55);	// 16-bit RGB565

	CommandLCD(ILI9341_PAGEADDRSET);
	Write8ToLCD(0x00);
	Write8ToLCD(0x00);
	Write8ToLCD(0x01);
	Write8ToLCD(0x3F);

	CommandLCD(ILI9341_COLADDRSET);
	Write8ToLCD(0x00);
	Write8ToLCD(0x00);
	Write8ToLCD(0x01);
	Write8ToLCD(0xDF);

	CommandLCD(ILI9341_MEMORYWRITE);

	uint8_t blue[2] = {0x00, 0x1F};
	for (int i = 0; i < 320 * 480; i++) {
	    Write8ToLCD(blue[0]);
	    Write8ToLCD(blue[1]);
	}
}
