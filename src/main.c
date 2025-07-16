#include "can.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h" // IWYU pragma: export
#include "freertos/task.h"

void app_main(void) {
    can_init();
    can_start();
    can_wait();

    ESP_LOGI("app_main", "Timer answered");

    while (1) {
    }
}
