<<<<<<< HEAD
#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "display.h"

#define BTN_INC GPIO_NUM_27
#define BTN_DEC GPIO_NUM_14
#define BTN_START GPIO_NUM_12

// GPIO initialization button
void init_gpio_button() {
    gpio_reset_pin(BTN_INC);
    gpio_set_direction(BTN_INC, GPIO_MODE_INPUT);
    gpio_set_pull_mode(BTN_INC, GPIO_PULLUP_ONLY);

    gpio_reset_pin(BTN_DEC);
    gpio_set_direction(BTN_DEC, GPIO_MODE_INPUT);
    gpio_set_pull_mode(BTN_DEC, GPIO_PULLUP_ONLY);    

    gpio_reset_pin(BTN_START);
    gpio_set_direction(BTN_START, GPIO_MODE_INPUT);
    gpio_set_pull_mode(BTN_START, GPIO_PULLUP_ONLY);  
}

void button_task(void *param) {
    int *counter = (int *)param;  // Pointer to shared counter variable
    int hold_time_inc = 200;      // Initial hold time for increment button (in ms)
    int hold_time_dec = 200;      // Initial hold time for decrement button (in ms)
    int step_inc = 30;            // Step reduction for increment button hold delay
    int step_dec = 30;            // Step reduction for decrement button hold delay

    while (1) {
        if (gpio_get_level(BTN_START) == 0) {
            int timer_value = *counter;
            printf("Timer started with value: %d\n", timer_value);

            while (timer_value >= 0) {
                *counter = timer_value;
                printf("Timer: %d\n", timer_value);

                timer_value--;
                vTaskDelay(pdMS_TO_TICKS(1000));
            }

            printf("Timer finished!\n");

            *counter = 0;
        }

        // Increment button logic: Increase the counter
        if (gpio_get_level(BTN_INC) == 0) {
            (*counter)++;
            if (*counter > 9999) *counter = 0;

            // Adjust hold time for faster increments when button is held
            hold_time_inc -= step_inc;
            if (hold_time_inc < 20) {
                hold_time_inc = 20;
                step_inc = 5;
            }

            vTaskDelay(pdMS_TO_TICKS(hold_time_inc));
        } else {
            hold_time_inc = 200;
            step_inc = 30;
        }

        // Decrement button logic: Decrease the counter
        if (gpio_get_level(BTN_DEC) == 0) {
            (*counter)--;
            if (*counter < 0) *counter = 9999;

            // Adjust hold time for faster decrements when button is held
            hold_time_dec -= step_dec;
            if (hold_time_dec < 20) {
                hold_time_dec = 20;
                step_dec = 5;
            }

            vTaskDelay(pdMS_TO_TICKS(hold_time_dec));
        } else {
            hold_time_dec = 200;
            step_dec = 30;
        }

        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

void app_main() {
    static int counter = 0;

    init_gpio_button();
    init_gpio_display();

    xTaskCreate(display_task, "display_task", 2048, &counter, 1, NULL);
    xTaskCreate(button_task, "button_task", 2048, &counter, 1, NULL);
}
=======
#include <stdio.h>
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

#define BTN_INC GPIO_NUM_27
#define BTN_DEC GPIO_NUM_14
#define BTN_START GPIO_NUM_12

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
void init_gpio() {
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

    gpio_reset_pin(BTN_INC);
    gpio_set_direction(BTN_INC, GPIO_MODE_INPUT);
    gpio_set_pull_mode(BTN_INC, GPIO_PULLUP_ONLY);

    gpio_reset_pin(BTN_DEC);
    gpio_set_direction(BTN_DEC, GPIO_MODE_INPUT);
    gpio_set_pull_mode(BTN_DEC, GPIO_PULLUP_ONLY);    

    gpio_reset_pin(BTN_START);
    gpio_set_direction(BTN_START, GPIO_MODE_INPUT);
    gpio_set_pull_mode(BTN_START, GPIO_PULLUP_ONLY);  
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

void button_task(void *param) {
    int *counter = (int *)param;  // Pointer to shared counter variable
    int hold_time_inc = 200;      // Initial hold time for increment button (in ms)
    int hold_time_dec = 200;      // Initial hold time for decrement button (in ms)
    int step_inc = 30;            // Step reduction for increment button hold delay
    int step_dec = 30;            // Step reduction for decrement button hold delay

    while (1) {
        if (gpio_get_level(BTN_START) == 0) {
            int timer_value = *counter;
            printf("Timer started with value: %d\n", timer_value);

            while (timer_value >= 0) {
                *counter = timer_value;
                printf("Timer: %d\n", timer_value);

                timer_value--;
                vTaskDelay(pdMS_TO_TICKS(1000));
            }

            printf("Timer finished!\n");

            *counter = 0;
        }

        // Increment button logic: Increase the counter
        if (gpio_get_level(BTN_INC) == 0) {
            (*counter)++;
            if (*counter > 9999) *counter = 0;

            hold_time_inc -= step_inc;
            if (hold_time_inc < 20) {
                hold_time_inc = 20;
                step_inc = 5;
            }

            vTaskDelay(pdMS_TO_TICKS(hold_time_inc));
        } else {
            hold_time_inc = 200;
            step_inc = 30;
        }

        if (gpio_get_level(BTN_DEC) == 0) {
            (*counter)--;
            if (*counter < 0) *counter = 9999;

            hold_time_dec -= step_dec;
            if (hold_time_dec < 20) {
                hold_time_dec = 20;
                step_dec = 5;
            }

            vTaskDelay(pdMS_TO_TICKS(hold_time_dec));
        } else {
            hold_time_dec = 200;
            step_dec = 30;
        }

        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

void app_main() {
    static int counter = 0;

    init_gpio();

    xTaskCreate(display_task, "display_task", 2048, &counter, 1, NULL);
    xTaskCreate(button_task, "button_task", 2048, &counter, 1, NULL);
}
>>>>>>> dcc8f93df5a1eee60983caa9ac710b23e53d432d
