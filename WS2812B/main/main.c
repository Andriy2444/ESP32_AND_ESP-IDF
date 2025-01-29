#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/rmt_tx.h"
#include "driver/gpio.h"
#include "esp_err.h"

// Конфігурація
#define LED_PIN 5       // GPIO для WS2812B
#define LED_COUNT 12    // Кількість світлодіодів
#define RMT_CHANNEL 0   // Канал RMT

// Структура кольору
typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} led_color_t;

// Ініціалізація RMT
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

// Відправлення кольорів
void ws2812b_send_pixels(led_color_t *colors, int count) {
    uint8_t buffer[count * 3];
    for (int i = 0; i < count; i++) {
        buffer[i * 3] = colors[i].g;  // WS2812B використовує GRB
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
        {255, 0, 0},   // Червоний
        {0, 255, 0},   // Зелений
        {0, 0, 255},   // Синій
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

void app_main(void) {
    ws2812b_init();

    while (true) {
        ws2812b_snake_effect_with_colors();
    }
}
