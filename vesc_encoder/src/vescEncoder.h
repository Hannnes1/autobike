#ifndef _VESC_H_
#define _VESC_H_

#include "common.h"

unsigned short crc16(unsigned char *buf, unsigned int len);

extern void sendAlive(uint8_t *array);

extern int setRpm(uint8_t* array, int rpm);

extern int setCurrent(uint8_t *array, int current);

void bufferAppendFloat32(uint8_t* buffer, float number, float scale, int *index);

void bufferAppendInt32(uint8_t* buffer, int number, int *index);

uint8_t *buildPacket(uint8_t *data, unsigned int len);

uint8_t txBuffer[512 + 6];

#endif