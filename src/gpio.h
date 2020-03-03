//
// Created by Torkis on 2020.02.08.
//

#ifndef __GPIO_H__
#define __GPIO_H__

uint32_t *get_gpio_reg();

void init_gpio_output(volatile uint32_t *gpio_reg, uint8_t gpio_pin);

void set_gpio_output(volatile uint32_t *gpio_reg, uint8_t gpio_pin, int b);

#endif // __GPIO_H__
