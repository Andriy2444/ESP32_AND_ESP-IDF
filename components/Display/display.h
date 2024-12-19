#ifndef DISPLAY_H
#define DISPLAY_H

#include "driver/gpio.h"
#include <stdint.h>

void init_gpio_display(void);

void display_digit(int digit, int position);

void display_task(void *param);

#endif // DISPLAY_H'