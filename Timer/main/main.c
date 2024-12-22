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
