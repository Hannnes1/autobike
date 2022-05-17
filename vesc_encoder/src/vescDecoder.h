#ifndef _VESC_DECODE_H_
#define _VESC_DECODE_H_

#include "common.h"

#define PACKET_RX_TIMEOUT		2
#define PACKET_MAX_PL_LEN		512

typedef enum {
	FAULT_CODE_NONE = 0,
	FAULT_CODE_OVER_VOLTAGE,
	FAULT_CODE_UNDER_VOLTAGE,
	FAULT_CODE_DRV,
	FAULT_CODE_ABS_OVER_CURRENT,
	FAULT_CODE_OVER_TEMP_FET,
	FAULT_CODE_OVER_TEMP_MOTOR
} mc_fault_code;

typedef struct {
	float v_in;
	float temp_mos;
	float temp_motor;
	float current_motor;
	float current_in;
	float id;
	float iq;
	float rpm;
	float duty_now;
	float amp_hours;
	float amp_hours_charged;
	float watt_hours;
	float watt_hours_charged;
    int tachometer;
    int tachometer_abs;
    mc_fault_code fault_code;
    float pid_pos;
    uint8_t vesc_id;
} mc_values;

static mc_values values;

typedef struct {
	volatile unsigned char rx_state;
	volatile unsigned char rx_timeout;
	void(*send_func)(unsigned char *data, unsigned int len);
	void(*process_func)(unsigned char *data, unsigned int len);
	unsigned int payload_length;
	unsigned char rx_buffer[PACKET_MAX_PL_LEN];
	unsigned char tx_buffer[PACKET_MAX_PL_LEN + 6];
	unsigned int rx_data_ptr;
	unsigned char crc_low;
	unsigned char crc_high;
} PACKET_STATE_t;

static PACKET_STATE_t handler_state;

void bldc_interface_process_packet(unsigned char *data, unsigned int len);
float buffer_get_float32(const uint8_t *buffer, float scale, int *index);
int buffer_get_int32(const uint8_t *buffer, int *index);
extern float packet_process_byte(uint8_t rx_data);
float buffer_get_float16(const uint8_t *buffer, float scale, int *index);
int16_t buffer_get_int16(const uint8_t *buffer, int *index);

#endif
