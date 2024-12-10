#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// Визначення GPIO для сегментів (a, b, c, d, e, f, g) та розрядів
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

// Таблиця сегментів для цифр 0-9 (1 - увімкнено, 0 - вимкнено)
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

// Масив пінів для сегментів
const gpio_num_t segment_pins[] = {
    SEG_A, SEG_B, SEG_C, SEG_D, SEG_E, SEG_F, SEG_G
};

// Масив пінів для розрядів
const gpio_num_t digit_pins[] = {
    DIGIT_1, DIGIT_2, DIGIT_3, DIGIT_4
};

// Ініціалізація GPIO
void init_gpio() {
    // Налаштування пінів для сегментів
    for (int i = 0; i < 7; i++) {
        gpio_reset_pin(segment_pins[i]);
        gpio_set_direction(segment_pins[i], GPIO_MODE_OUTPUT);
        gpio_set_level(segment_pins[i], 0); // Вимкнути сегменти
        gpio_set_pull_mode(segment_pins[i], GPIO_PULLDOWN_ONLY); // Активувати пулдаун
    }
    // Налаштування пінів для розрядів
    for (int i = 0; i < 4; i++) {
        gpio_reset_pin(digit_pins[i]);
        gpio_set_direction(digit_pins[i], GPIO_MODE_OUTPUT);
        gpio_set_level(digit_pins[i], 1); // Вимкнути розряди (для загального аноду)
    }
}

// Виведення однієї цифри на один розряд
void display_digit(int digit, int position) {
    // Вимкнути всі сегменти перед зміною розряду
    for (int i = 0; i < 7; i++) {
        gpio_set_level(segment_pins[i], 0);
    }

    // Увімкнути відповідний розряд
    for (int i = 0; i < 4; i++) {
        gpio_set_level(digit_pins[i], i == position ? 0 : 1); // 0 - активний
    }

    // Увімкнути сегменти для цифри
    uint8_t segments = digit_segments[digit];
    for (int i = 0; i < 7; i++) {
        gpio_set_level(segment_pins[i], (segments >> i) & 1);
    }
}

// Головний цикл для мультиплексування
void display_task(void *param) {
    int *counter = (int *)param;

    while (1) {
        int digits[4] = {
            *counter / 1000 % 10,
            *counter / 100 % 10,
            *counter / 10 % 10,
            *counter % 10
        };

        // По черзі виводимо кожну цифру
        for (int i = 0; i < 4; i++) {
            display_digit(digits[i], i);
            vTaskDelay(pdMS_TO_TICKS(5)); // Час для оновлення розряду
        }
    }
}

// Задача для збільшення лічильника
void counter_task(void *param) {
    int *counter = (int *)param;

    while (1) {
        (*counter)++;
        if (*counter > 9999) {
            *counter = 0; // Скидання до 0000
        }
        vTaskDelay(pdMS_TO_TICKS(1000)); // Інтервал в 1 секунду
    }
}

void app_main() {
    static int counter = 0; // Початкове значення лічильника

    init_gpio();

    // Створення задач для відображення та лічильника
    xTaskCreate(display_task, "display_task", 2048, &counter, 1, NULL);
    xTaskCreate(counter_task, "counter_task", 2048, &counter, 1, NULL);
}
