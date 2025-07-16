/*
 * CAN Bus library for the ESP32-S3
 */

#include "driver/gpio.h"
#include "driver/gptimer.h"
#include "esp_attr.h"
#include "esp_err.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h" // IWYU pragma: export
#include "freertos/queue.h"
#include "freertos/task.h"
#include "soc/gpio_num.h"
#include <stdint.h>

#define CAN_HIGH GPIO_NUM_4
#define CAN_LOW GPIO_NUM_5

static const char *TAG = "can_library";

static gptimer_handle_t gptimer = NULL;

static QueueHandle_t busy = NULL;

static uint8_t led_on = false;

static bool IRAM_ATTR isr_timer(gptimer_handle_t timer,
                                const gptimer_alarm_event_data_t *edata,
                                void *user_data) {
    if (led_on) {
        gpio_set_level(GPIO_NUM_4, 0);
        led_on = false;
    } else {
        gpio_set_level(GPIO_NUM_4, 1);
        led_on = true;
    }

    BaseType_t high_task_awoken = pdFALSE;

    // Finish task
    int evt = 1;
    xQueueSendFromISR(busy, &evt, &high_task_awoken);

    // return whether we need to yield at the end of ISR
    return (high_task_awoken == pdTRUE);
}

void can_init(void) {
    // -- Setup GPIO
    gpio_config_t io_conf = {.pin_bit_mask = 1ULL << CAN_HIGH | 1ULL << CAN_LOW,
                             .mode = GPIO_MODE_OUTPUT,
                             .pull_up_en = GPIO_PULLUP_DISABLE,
                             .pull_down_en = GPIO_PULLDOWN_DISABLE,
                             .intr_type = GPIO_INTR_DISABLE};

    gpio_config(&io_conf);

    /* -- Setup timer
     *
     * Reference
     * https://github.com/espressif/esp-idf/blob/master/examples/peripherals/timer_group/gptimer/main/gptimer_example_main.c
     */

    // Only one timer at a time allowed for CAN Bus
    busy = xQueueCreate(1, sizeof(QueueHandle_t));
    if (!busy) {
        ESP_LOGE(TAG, "Creating queue failed");
        return;
    }

    ESP_LOGI(TAG, "Create timer handle");
    gptimer_config_t timer_config = {
        .clk_src = GPTIMER_CLK_SRC_DEFAULT,
        .direction = GPTIMER_COUNT_UP,
        .resolution_hz = 1000000, // 1 MHz: 1 tick = 1 us
    };
    ESP_ERROR_CHECK(gptimer_new_timer(&timer_config, &gptimer));

    gptimer_event_callbacks_t callbacks = {
        .on_alarm = isr_timer,
    };
    ESP_ERROR_CHECK(
        gptimer_register_event_callbacks(gptimer, &callbacks, busy));

    ESP_LOGI(TAG, "Enable timer");
    ESP_ERROR_CHECK(gptimer_enable(gptimer));

    ESP_LOGI(TAG, "Start timer, auto-reload at alarm event");
    gptimer_alarm_config_t alarm_config = {
        .reload_count = 0,
        .alarm_count = 1000000, // every second
        .flags.auto_reload_on_alarm = true,
    };
    ESP_ERROR_CHECK(gptimer_set_alarm_action(gptimer, &alarm_config));
}

void can_start(void) {
    ESP_ERROR_CHECK(gptimer_set_raw_count(gptimer, 0));
    ESP_ERROR_CHECK(gptimer_start(gptimer));
}

void can_wait(void) {
    int evt;
    while (true) {
        if (xQueueReceive(busy, &evt, portMAX_DELAY)) {
            return;
        }
    }
}

void can_stop(void) {
    ESP_LOGI(TAG, "Stop timer");
    ESP_ERROR_CHECK(gptimer_stop(gptimer));
}
