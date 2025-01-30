// #ifndef WS2812B_H
// #define WS2812B_H

// #include "driver/gpio.h"

// #define LED_PIN 5 
// #define LED_COUNT 12 
// #define RMT_CHANNEL 0  

typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} led_color_t;

void ws2812b_init(void);

void ws2812b_send_pixels(led_color_t *colors, int count);

void ws2812b_gradient_effect(led_color_t start_color, led_color_t end_color, int steps, int delay_ms);

void ws2812b_smooth_snake(void);

// #endif // WS2812B_H