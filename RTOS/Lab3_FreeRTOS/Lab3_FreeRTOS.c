#include "FreeRTOS.h"
#include "pico/stdlib.h"
#include "queue.h"
#include "task.h"
#include <stdio.h>

#define BUTTON_GPIO 2
#define LED_GPIO_2 3
#define LED_GPIO_0 4
#define LED_GPIO_1 5

QueueHandle_t msgQueue;

void blink_task(__unused void *params) {
  bool led_on = false;
  gpio_init(LED_GPIO_2);
  gpio_set_dir(LED_GPIO_2, GPIO_OUT);
  while (1) {
    if (led_on) {
      vTaskDelay(pdMS_TO_TICKS(33));
    } else {
      vTaskDelay(pdMS_TO_TICKS(67));
    }
    gpio_put(LED_GPIO_2, led_on);
    led_on = !led_on;
  }
}

void button_task(__unused void *params) {
  gpio_init(BUTTON_GPIO);
  gpio_set_dir(BUTTON_GPIO, GPIO_IN);
  gpio_pull_up(BUTTON_GPIO);
  vTaskDelay(pdMS_TO_TICKS(1000)); // wait for pull up

  while (1) {
    uint core = portGET_CORE_ID();
    if (!gpio_get(BUTTON_GPIO)) {
      while (xQueueSend(msgQueue, &core, 100) == errQUEUE_FULL)
        ;
      while (!gpio_get(BUTTON_GPIO))
        ;
      vTaskDelay(pdMS_TO_TICKS(100)); // trash but simple debouncing
    }
  }
}

void message_task(__unused void *params) {
  uint core;
  gpio_init(LED_GPIO_0);
  gpio_set_dir(LED_GPIO_0, GPIO_OUT);
  gpio_init(LED_GPIO_1);
  gpio_set_dir(LED_GPIO_1, GPIO_OUT);
  while (1) {
    if (xQueueReceive(msgQueue, &core, 100) == pdPASS) {
      if (core == 0) {
        gpio_put(LED_GPIO_0, true);
        vTaskDelay(pdMS_TO_TICKS(100));
        gpio_put(LED_GPIO_0, false);
      } else if (core == 1) {
        gpio_put(LED_GPIO_1, true);
        vTaskDelay(pdMS_TO_TICKS(100));
        gpio_put(LED_GPIO_1, false);
      }
    }
  }
}

int main() {
  stdio_init_all();

  TaskHandle_t blink_task_handle, button_task_handle, message_task_handle;
  xTaskCreate(blink_task, "BlinkThread", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, &blink_task_handle);
  xTaskCreate(button_task, "ButtonThread", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 2, &button_task_handle);
  xTaskCreate(message_task, "MessageThread", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 4,
              &message_task_handle);

  msgQueue = xQueueCreate(16, sizeof(uint));
  vTaskStartScheduler();
}
