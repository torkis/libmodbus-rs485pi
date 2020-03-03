# libmodbus-rs485pi
Libmodbus extension for RS485 half duplex chips

The half duplex RS485 handling is not working with simple RS485 chips, because the DE pin is not handled correctly with RPi4 UART CTS/RTS pin. This library wraps the libmodbus RTU initialization function and set the modbus_rtu_pi_set_rts callback. In this callback the DE pin control can be very fast with native memory mapped GPIO handling. The GPIO handling speed can below 10us, so the library can use the RS485 in 57600 or even faster.

I used this library with Home Automation project in HomeAssistant. See also the libmodbus-rs485 homeassistant plugin.
