//
// Created by Torkis on 2020.02.08.
//

#ifndef MODBUS_RTU_PI_H_
#define MODBUS_RTU_PI_H_

#include <modbus/modbus-rtu.h>

MODBUS_API modbus_t* modbus_new_rtu_rs485pi(const char *device, int baud,
		char parity, int data_bit, int stop_bit, char rts_mode, int rts_pin, int rts_delayus);

MODBUS_API void modbus_free_rtu_rs485pi(modbus_t *ctx);

#endif /* MODBUS_RTU_PI_H_ */
