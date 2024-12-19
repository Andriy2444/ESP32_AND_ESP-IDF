#include "display.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// GPIO definitions for segments (a, b, c, d, e, f, g) and digits
#define SEG_A GPIO_NUM_23
#define SEG_B GPIO_NUM_22
#define SEG_C GPIO_NUM_21
#define SEG_D GPIO_NUM_19
#define SEG_E GPIO_NUM_18
#define SEG_F GPIO_NUM_5
#define SEG_G GPIO_NUM_4

#define DIGIT_1 GPIO_NUM_33
#define DIGIT_2 GPIO_NUM_32
#define DIGIT_3 GPIO_NUM_25
#define DIGIT_4 GPIO_NUM_26


// Segment table for numbers 0-9
const uint8_t digit_segments[10] = {
    0b00111111, // 0
    0b00000110, // 1
    0b01011011, // 2
    0b01001111, // 3
    0b01100110, // 4
    0b01101101, // 5
    0b01111101, // 6
    0b00000111, // 7
    0b01111111, // 8
    0b01101111  // 9
};

// Array of pins for segments
const gpio_num_t segment_pins[] = {
    SEG_A, SEG_B, SEG_C, SEG_D, SEG_E, SEG_F, SEG_G
};

// Array of pins for digits
const gpio_num_t digit_pins[] = {
    DIGIT_1, DIGIT_2, DIGIT_3, DIGIT_4
};

// GPIO initialization
void init_gpio_display() {
    // Set pins for segments
    for (int i = 0; i < 7; i++) {
        gpio_reset_pin(segment_pins[i]);
        gpio_set_direction(segment_pins[i], GPIO_MODE_OUTPUT);
        gpio_set_level(segment_pins[i], 0);
        gpio_set_pull_mode(segment_pins[i], GPIO_PULLDOWN_ONLY);
    }
    // Set pins for digits
    for (int i = 0; i < 4; i++) {
        gpio_reset_pin(digit_pins[i]);
        gpio_set_direction(digit_pins[i], GPIO_MODE_OUTPUT);
        gpio_set_level(digit_pins[i], 1);
    } 
}

// Output one digit per digit
void display_digit(int digit, int position) {
    // Turn off all segments before changing the digit
    for (int i = 0; i < 7; i++) {
        gpio_set_level(segment_pins[i], 0);
    }

    // Enable need digit
    for (int i = 0; i < 4; i++) {
        gpio_set_level(digit_pins[i], i == position ? 0 : 1);
    }

    // Enable need segments
    uint8_t segments = digit_segments[digit];
    for (int i = 0; i < 7; i++) {
        gpio_set_level(segment_pins[i], (segments >> i) & 1);
    }
}

// Main loop for multiplexing
void display_task(void *param) {
    int *counter = (int *)param;

    while (1) {
        int digits[4] = {
            *counter / 1000 % 10,
            *counter / 100 % 10,
            *counter / 10 % 10,
            *counter % 10
        };

        for (int i = 0; i < 4; i++) {
            display_digit(digits[i], i);
            vTaskDelay(pdMS_TO_TICKS(5)); // Time to update bit
        }
    }
}