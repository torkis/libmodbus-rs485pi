//
// Created by Torkis on 2020.02.08.
//

#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

// Physical Memory Allocation, from raspberrypi/userland demo.
#include "mailbox.h"

#define BCM2711_PI4_PERI_BASE  0xFE000000
#define PERI_BASE   BCM2711_PI4_PERI_BASE

#define PAGE_SIZE 4096

// ---- GPIO specific defines
#define GPIO_REGISTER_BASE 0x200000
#define GPIO_SET_OFFSET 0x1C
#define GPIO_CLR_OFFSET 0x28

// Return a pointer to a periphery subsystem register.
static void *mmap_bcm_register(off_t register_offset) {
    const off_t base = PERI_BASE;

    int mem_fd;
    if ((mem_fd = open("/dev/gpiomem", O_RDWR | O_SYNC)) < 0) {
        perror("can't open /dev/gpiomem: ");
        return NULL;
    }

    uint32_t *result =
            (uint32_t *) mmap(NULL,                  // Any adddress in our space will do
                              PAGE_SIZE,
                              PROT_READ | PROT_WRITE,  // Enable r/w on GPIO registers.
                              MAP_SHARED,
                              mem_fd,                // File to map
                              base + register_offset // Offset to bcm register
            );
    close(mem_fd);

    if (result == MAP_FAILED) {
        fprintf(stderr, "mmap error %p\n", result);
        return NULL;
    }
    return result;
}

uint32_t *get_gpio_reg() {
    return mmap_bcm_register(GPIO_REGISTER_BASE);
}

void init_gpio_output(volatile uint32_t *gpio_reg, uint8_t gpio_pin) {
    *(gpio_reg + (gpio_pin / 10)) &= ~(7 << ((gpio_pin % 10) * 3));  // prepare: set as input
    *(gpio_reg + (gpio_pin / 10)) |= (1 << ((gpio_pin % 10) * 3));  // set as output.
}

void set_gpio_output(volatile uint32_t *gpio_reg, uint8_t gpio_pin, int b) {
    if (b) {
        *(gpio_reg + (GPIO_SET_OFFSET / sizeof(uint32_t))) = (1 << gpio_pin);
    } else {
        *(gpio_reg + (GPIO_CLR_OFFSET / sizeof(uint32_t))) = (1 << gpio_pin);
    }
}
