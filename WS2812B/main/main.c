#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "WS2812B.h"

void app_main(void) {
    ws2812b_init();

    ws2812b_gradient_effect((led_color_t){255, 0, 0}, (led_color_t){0, 0, 255}, 12, 50);

    // while (true) {
    //     ws2812b_smooth_snake();
    // }
}
