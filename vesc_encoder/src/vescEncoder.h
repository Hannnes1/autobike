#ifndef _VESC_H_
#define _VESC_H_

typedef unsigned char uint8_t;

unsigned short crc16(unsigned char *buf, unsigned int len);

extern void bldc_interface_send_alive(uint8_t *array);

extern int bldc_interface_set_rpm(uint8_t* array, int rpm);

extern int bldc_interface_set_current(uint8_t *array, int current);

void buffer_append_float32(uint8_t* buffer, float number, float scale, int *index);

void buffer_append_int32(uint8_t* buffer, int number, int *index);

uint8_t *packet_send_packet(uint8_t *data, unsigned int len, int handler_num);

uint8_t tx_buffer[512 + 6];

#endif