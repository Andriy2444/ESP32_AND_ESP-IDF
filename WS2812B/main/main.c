#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/rmt_tx.h"
#include "driver/gpio.h"
#include "esp_err.h"

#define LED_PIN 5 
#define LED_COUNT 12   
#define RMT_CHANNEL 0   

typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} led_color_t;

static rmt_channel_handle_t rmt_tx_channel = NULL;
static rmt_encoder_handle_t rmt_ws2812_encoder = NULL;

void ws2812b_init() {
    rmt_tx_channel_config_t tx_chan_config = {
        .gpio_num = LED_PIN,
        .clk_src = RMT_CLK_SRC_DEFAULT,
        .resolution_hz = 10 * 1000 * 1000,  // 10 MHz
        .mem_block_symbols = 64,
        .trans_queue_depth = 4
    };
    ESP_ERROR_CHECK(rmt_new_tx_channel(&tx_chan_config, &rmt_tx_channel));

    rmt_bytes_encoder_config_t ws2812_encoder_config = {
        .bit0 = { .duration0 = 4, .level0 = 1, .duration1 = 9, .level1 = 0 },
        .bit1 = { .duration0 = 9, .level0 = 1, .duration1 = 4, .level1 = 0 },
        .flags.msb_first = 1
    };
    ESP_ERROR_CHECK(rmt_new_bytes_encoder(&ws2812_encoder_config, &rmt_ws2812_encoder));

    ESP_ERROR_CHECK(rmt_enable(rmt_tx_channel));
}

void ws2812b_send_pixels(led_color_t *colors, int count) {
    uint8_t buffer[count * 3];
    for (int i = 0; i < count; i++) {
        buffer[i * 3] = colors[i].g;
        buffer[i * 3 + 1] = colors[i].r;
        buffer[i * 3 + 2] = colors[i].b;
    }

    rmt_transmit_config_t tx_config = {
        .loop_count = 0
    };
    ESP_ERROR_CHECK(rmt_transmit(rmt_tx_channel, rmt_ws2812_encoder, buffer, sizeof(buffer), &tx_config));
    ESP_ERROR_CHECK(rmt_tx_wait_all_done(rmt_tx_channel, portMAX_DELAY));
}

void ws2812b_snake_effect_with_colors() {
    led_color_t colors[LED_COUNT] = {0};

    led_color_t snake_colors[] = {
        {255, 0, 0},   // Red
        {0, 255, 0},   // Green
        {0, 0, 255},   // Blue
    };

    int num_colors = sizeof(snake_colors) / sizeof(snake_colors[0]);

    for (int i = 0; i < LED_COUNT; i++) {
        memset(colors, 0, sizeof(colors)); 
        colors[i] = snake_colors[i % num_colors];

        uint8_t buffer[LED_COUNT * 3];
        for (int j = 0; j < LED_COUNT; j++) {
            buffer[j * 3] = colors[j].g;
            buffer[j * 3 + 1] = colors[j].r;
            buffer[j * 3 + 2] = colors[j].b;
        }

        rmt_transmit_config_t tx_config = {
            .loop_count = 0
        };

        ESP_ERROR_CHECK(rmt_transmit(rmt_tx_channel, rmt_ws2812_encoder, buffer, sizeof(buffer), &tx_config));
        ESP_ERROR_CHECK(rmt_tx_wait_all_done(rmt_tx_channel, portMAX_DELAY));

        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

void ws2812b_gradient_effect(led_color_t start_color, led_color_t end_color, int steps, int delay_ms) {
    led_color_t colors[LED_COUNT];

    float step_r = (float)(end_color.r - start_color.r) / (LED_COUNT - 1);
    float step_g = (float)(end_color.g - start_color.g) / (LED_COUNT - 1);
    float step_b = (float)(end_color.b - start_color.b) / (LED_COUNT - 1);

    for (int i = 0; i < LED_COUNT; i++) {
        colors[i].r = start_color.r + (int)(step_r * i);
        colors[i].g = start_color.g + (int)(step_g * i);
        colors[i].b = start_color.b + (int)(step_b * i);
    }

    while (true) {
        // Відправляємо кольори на стрічку
        ws2812b_send_pixels(colors, LED_COUNT);
        vTaskDelay(delay_ms / portTICK_PERIOD_MS);

        // Зсуваємо всі кольори вправо (останній колір переходить на перше місце)
        led_color_t last = colors[LED_COUNT - 1];
        for (int i = LED_COUNT - 1; i > 0; i--) {
            colors[i] = colors[i - 1];
        }
        colors[0] = last; // Переміщаємо останній елемент на початок
    }
}


void ws2812b_smooth_snake() {
    led_color_t colors[LED_COUNT] = {0}; // Масив кольорів
    
    led_color_t snake_colors[] = {
        {255, 0, 0},   // Червоний
        {0, 255, 0},   // Зелений
        {0, 0, 255}    // Синій
    };
    int num_colors = sizeof(snake_colors) / sizeof(snake_colors[0]);

    int position = 0; // Початкова позиція

    while (true) {
        // Оновлюємо кольори для ефекту змійки
        for (int i = 0; i < LED_COUNT; i++) {
            // Плавне згасання кольору (ефект сліду)
            colors[i].r = colors[i].r * 0.5;
            colors[i].g = colors[i].g * 0.5;
            colors[i].b = colors[i].b * 0.5;
        }

        // Додаємо новий "головний" колір змійки
        colors[position] = snake_colors[position % num_colors];

        // Відправляємо дані на стрічку
        ws2812b_send_pixels(colors, LED_COUNT);

        // Зсуваємо позицію змійки
        position = (position + 1) % LED_COUNT;

        // Час оновлення (чим менше, тим плавніше)
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}


void app_main(void) {
    ws2812b_init();

    ws2812b_gradient_effect((led_color_t){255, 0, 0}, (led_color_t){0, 0, 255}, LED_COUNT, 50);

    // while (true) {
    //     ws2812b_smooth_snake();
    // }
}
