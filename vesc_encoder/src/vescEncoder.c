#include "vescEncoder.h"
#include "common.h"

#include <string.h>

static unsigned char send_buffer[1024];

extern void bldc_interface_send_alive(uint8_t *array) {
    int send_index = 0;
    send_buffer[send_index++] = 30;

	for (uint8_t i = 0; i < 10; i++) {
        array[i] = packet_send_packet(send_buffer, send_index, 0)[i];
    }
}

void bldc_interface_get_values(uint8_t *array) {
	int send_index = 0;
	send_buffer[send_index++] = 4;

	uint8_t *packet = packet_send_packet(send_buffer, send_index, 0);

	for (uint8_t i = 0; i < 10; i++) {
        array[i] = packet[i];
    }
}

extern int bldc_interface_set_current(uint8_t *array, int current) {
	int send_index = 0;
	send_buffer[send_index++] = 6;
	buffer_append_float32(send_buffer, (float) current, 1000.0, &send_index);

    uint8_t *packet = packet_send_packet(send_buffer, send_index, 0);

	for (uint8_t i = 0; i < 10; i++) {
        array[i] = packet[i];
    }
	return current;
}

extern int bldc_interface_set_rpm(uint8_t *array, int rpm) {
    rpm *= 250; // convert to actual RPM.

    int send_index = 0;
    send_buffer[send_index++] = 8;
    buffer_append_int32(send_buffer, rpm, &send_index);

    uint8_t *packet = packet_send_packet(send_buffer, send_index, 0);

    for (uint8_t i = 0; i < 10; i++) {
        array[i] = packet[i];
    }
    return rpm;
}

uint8_t *packet_send_packet(uint8_t *data, unsigned int len, int handler_num) {
    int b_ind = 0;

    if (len <= 256) {
        tx_buffer[b_ind++] = 2;
        tx_buffer[b_ind++] = len;
    } else {
        tx_buffer[b_ind++] = 3;
        tx_buffer[b_ind++] = len >> 8;
        tx_buffer[b_ind++] = len & 0xFF;
    }

    memcpy(tx_buffer + b_ind, data, len);
    b_ind += len;

    unsigned short crc = crc16(data, len);
    tx_buffer[b_ind++] = (uint8_t)(crc >> 8);
    tx_buffer[b_ind++] = (uint8_t)(crc & 0xFF);
    tx_buffer[b_ind++] = 3;

    return tx_buffer;
}

void buffer_append_float32(uint8_t* buffer, float number, float scale, int *index) {
    buffer_append_int32(buffer, (int)(number * scale), index);
}

void buffer_append_int32(uint8_t *buffer, int number, int *index) {
    buffer[(*index)++] = number >> 24;
    buffer[(*index)++] = number >> 16;
    buffer[(*index)++] = number >> 8;
    buffer[(*index)++] = number;
}
