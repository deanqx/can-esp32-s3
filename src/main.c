#include "driver/gpio.h"

#include "freertos/FreeRTOS.h" // IWYU pragma: export
#include "freertos/task.h"

void app_main(void) {
  gpio_config_t io_conf = {
      .pin_bit_mask = (1ULL << GPIO_NUM_4),  // bit mask for the pin
      .mode = GPIO_MODE_OUTPUT,              // set as output
      .pull_up_en = GPIO_PULLUP_DISABLE,     // optional
      .pull_down_en = GPIO_PULLDOWN_DISABLE, // optional
      .intr_type = GPIO_INTR_DISABLE         // no interrupt
  };
  gpio_config(&io_conf);

  while (1) {
    gpio_set_level(GPIO_NUM_4, 1);
    vTaskDelay(1000 / portTICK_PERIOD_MS);

    gpio_set_level(GPIO_NUM_4, 0);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}
