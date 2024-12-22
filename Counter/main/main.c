#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "display.h"

// Task to increase the counter
void counter_task(void *param) {
    int *counter = (int *)param;

    while (1) {
        (*counter)++;
        if (*counter > 9999) {
            *counter = 0; // Reset
        }
        vTaskDelay(pdMS_TO_TICKS(1000)); // Interval of 1 second
    }
}

void app_main() {
    static int counter = 0; // The initial value of the counter

    init_gpio_display();

    xTaskCreate(display_task, "display_task", 2048, &counter, 1, NULL);
    xTaskCreate(counter_task, "counter_task", 2048, &counter, 1, NULL);
}
