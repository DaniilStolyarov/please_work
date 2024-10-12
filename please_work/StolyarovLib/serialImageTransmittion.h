#ifndef SERIAL_IMAGE_TRANSMITTION_H
#define SERIAL_IMAGE_TRANSMITTION_H

#define RX_BUFFER_SIZE 17000
#include "basicFunctions.h"
#include <stdlib.h>
enum TransferState
{
	// transfer started and MCU is not currently painting pixels
	READY = 0,
	// MCU is busy and not ready to receive incoming data
	BUSY = 1,
	ENDED = 2
};
void transmitReadyMessage(void);
void defineLCDDrawArea(void);
void InitUSB(void);
void drawIncomingImage();
#endif // SERIAL_IMAGE_TRANSMITTION_H
