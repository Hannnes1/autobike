#include "vescDecoder.h"

#include "common.h"

extern float packet_process_byte(uint8_t rx_data) {
	float rpm;
	
    switch (handler_state.rx_state) {
        case 0:
            if (rx_data == 2) {
                // 1 byte PL len
                handler_state.rx_state += 2;
                handler_state.rx_timeout = PACKET_RX_TIMEOUT;
                handler_state.rx_data_ptr = 0;
                handler_state.payload_length = 0;
            } else if (rx_data == 3) {
                // 2 byte PL len
                handler_state.rx_state++;
                handler_state.rx_timeout = PACKET_RX_TIMEOUT;
                handler_state.rx_data_ptr = 0;
                handler_state.payload_length = 0;
            } else {
                handler_state.rx_state = 0;
            }
            break;

        case 1:
            handler_state.payload_length = (unsigned int)rx_data << 8;
            handler_state.rx_state++;
            handler_state.rx_timeout = PACKET_RX_TIMEOUT;
            break;

        case 2:
            handler_state.payload_length |= (unsigned int)rx_data;
            if (handler_state.payload_length > 0 &&
                handler_state.payload_length <= PACKET_MAX_PL_LEN) {
                handler_state.rx_state++;
                handler_state.rx_timeout = PACKET_RX_TIMEOUT;
            } else {
                handler_state.rx_state = 0;
            }
            break;

        case 3:
            handler_state.rx_buffer[handler_state.rx_data_ptr++] = rx_data;
            if (handler_state.rx_data_ptr == handler_state.payload_length) {
                handler_state.rx_state++;
            }
            handler_state.rx_timeout = PACKET_RX_TIMEOUT;
            break;

        case 4:
            handler_state.crc_high = rx_data;
            handler_state.rx_state++;
            handler_state.rx_timeout = PACKET_RX_TIMEOUT;
            break;

        case 5:
            handler_state.crc_low = rx_data;
            handler_state.rx_state++;
            handler_state.rx_timeout = PACKET_RX_TIMEOUT;
            break;

        case 6:
            if (rx_data == 3) {
                if (crc16(handler_state.rx_buffer, handler_state.payload_length) == ((unsigned short)handler_state.crc_high << 8 | (unsigned short)handler_state.crc_low)) {
                    // Packet received!
                    bldc_interface_process_packet(handler_state.rx_buffer,
                                                  handler_state.payload_length);

					rpm = values.rpm;
                }
            }
            handler_state.rx_state = 0;
            break;

        default:
            handler_state.rx_state = 0;
            break;
    }

	return rpm;
}

void bldc_interface_process_packet(unsigned char *data, unsigned int len) {
    int ind = 0;
    int i = 0;
    unsigned char id = data[0];
    data++;
    len--;
    if (id == 4) {
        values.temp_mos = buffer_get_float16(data, 1e1, &ind);
        values.temp_motor = buffer_get_float16(data, 1e1, &ind);
        values.current_motor = buffer_get_float32(data, 1e2, &ind);
        values.current_in = buffer_get_float32(data, 1e2, &ind);
        values.id = buffer_get_float32(data, 1e2, &ind);
        values.iq = buffer_get_float32(data, 1e2, &ind);
        values.duty_now = buffer_get_float16(data, 1e3, &ind);
        values.rpm = buffer_get_float32(data, 1e0, &ind);
        values.v_in = buffer_get_float16(data, 1e1, &ind);
        values.amp_hours = buffer_get_float32(data, 1e4, &ind);
        values.amp_hours_charged = buffer_get_float32(data, 1e4, &ind);
        values.watt_hours = buffer_get_float32(data, 1e4, &ind);
        values.watt_hours_charged = buffer_get_float32(data, 1e4, &ind);
        values.tachometer = buffer_get_int32(data, &ind);
        values.tachometer_abs = buffer_get_int32(data, &ind);
        values.fault_code = (mc_fault_code)data[ind++];

        if (ind < (int)len) {
            values.pid_pos = buffer_get_float32(data, 1e6, &ind);
        } else {
            values.pid_pos = 0.0;
        }

        if (ind < (int)len) {
            values.vesc_id = data[ind++];
        } else {
            values.vesc_id = 255;
        }
    }
}

float buffer_get_float32(const uint8_t *buffer, float scale, int *index) {
    return (float)buffer_get_int32(buffer, index) / scale;
}

int buffer_get_int32(const uint8_t *buffer, int *index) {
    int res = ((uint32_t)buffer[*index]) << 24 |
              ((uint32_t)buffer[*index + 1]) << 16 |
              ((uint32_t)buffer[*index + 2]) << 8 |
              ((uint32_t)buffer[*index + 3]);
    *index += 4;
    return res;
}

float buffer_get_float16(const uint8_t *buffer, float scale, int *index) {
    return (float)buffer_get_int16(buffer, index) / scale;
}

int16_t buffer_get_int16(const uint8_t *buffer, int *index) {
    int16_t res = ((uint16_t)buffer[*index]) << 8 |
                  ((uint16_t)buffer[*index + 1]);
    *index += 2;
    return res;
}
