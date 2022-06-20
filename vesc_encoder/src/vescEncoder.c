#include "vescEncoder.h"
#include "common.h"

#include <string.h>

static unsigned char buffer[1024];

// Calculates command used to keep the motor alive.
extern void sendAlive(uint8_t *array) {
    int index = 0;

    buffer[index++] = 30; // Number correspondning to the keepAlive command.

    // Build the packet for the command.
    uint8_t *packet = buildPacket(buffer, index);

    // Copy calculated packet to the referance array.
	for (uint8_t i = 0; i < 10; i++) {
        array[i] = packet[i];
    }
}

// Calculates command used to set the current of the motor.
// Based on the given parameter "current".
extern int setCurrent(uint8_t *array, int current) {
	int index = 0;

	buffer[index++] = 6; // Number correspondning to the setCurrent command.

    // Rescale the current and swap its endianness.
	bufferAppendFloat32(buffer, (float) current, 1000.0, &index);

    // Build the packet for the command.
    uint8_t *packet = buildPacket(buffer, index);

    // Copy the calculated packet unto the referance array.
	for (uint8_t i = 0; i < 10; i++) {
        array[i] = packet[i];
    }
	return current;
}

// Calculates command used to set the rpm of the motor.
// Based on the given parameter "rpm".
extern int setRpm(uint8_t *array, int rpm) {
    int index = 0;

    rpm *= 250; // Convert to the actual RPM of the red bike's pedals.

    buffer[index++] = 8; // Number correspondning to the setRpm command.

    // Swpan the endianness of the rpm.
    bufferAppendInt32(buffer, rpm, &index);
 
    // Build the packet for the command.
    uint8_t *packet = buildPacket(buffer, index);

    // Copy the calculated packet unto the referance array.
    for (uint8_t i = 0; i < 10; i++) {
        array[i] = packet[i];
    }
    return rpm;
}

// Builds the packet based on the given command and payload.
// See http://vedder.se/2015/10/communicating-with-the-vesc-using-uart/ for more information.
uint8_t *buildPacket(uint8_t *data, unsigned int len) {
    int bufferIndex = 0;

    if (len <= 256) {
        txBuffer[bufferIndex++] = 2;
        txBuffer[bufferIndex++] = len;
    } else {
        txBuffer[bufferIndex++] = 3;
        txBuffer[bufferIndex++] = len >> 8;
        txBuffer[bufferIndex++] = len & 0xFF;
    }

    memcpy(txBuffer + bufferIndex, data, len);
    bufferIndex += len;

    unsigned short crc = crc16(data, len);
    txBuffer[bufferIndex++] = (uint8_t)(crc >> 8);
    txBuffer[bufferIndex++] = (uint8_t)(crc & 0xFF);
    txBuffer[bufferIndex++] = 3;

    return txBuffer;
}

// Rescales the given "number" with "scale", before swapping its endianness with "bufferAppendInt32".
void bufferAppendFloat32(uint8_t* buffer, float number, float scale, int *index) {
    bufferAppendInt32(buffer, (int)(number * scale), index);
}

// Swaps the endianness of the given "number".
void bufferAppendInt32(uint8_t *buffer, int number, int *index) {
    buffer[(*index)++] = number >> 24;
    buffer[(*index)++] = number >> 16;
    buffer[(*index)++] = number >> 8;
    buffer[(*index)++] = number;
}
