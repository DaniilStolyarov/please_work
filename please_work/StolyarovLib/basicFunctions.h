#ifndef BASIC_FUNCTIONS_H
#define BASIC_FUNCTIONS_H

#include "main.h"
#include "registers.h"

void StopIfBlueButtonWasPressed(void);
void LD2Init(void);
void write8(uint8_t value);
void CommandLCD(uint8_t command);
void Write8ToLCD(uint8_t data);
void Write16ToLCD(uint16_t data);
void EnterSleepLCD(void);
void ExitSleepLCD(void);
void initLCD(void);
void fillScreen(void);
void fillRect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
#endif
