#pragma GCC optimize("O0")

#include "FreeRTOS.h"
#include "accelerometer.h"
#include "hardware/i2c.h"
#include "math.h"
#include "oled.h"
#include "pico/stdlib.h"
#include "queue.h"
#include "pico/malloc.h"
#include "task.h"
#include <stdio.h>

// clang-format off
#define STRLEN(x)         sizeof(x) / sizeof(char)
#define UI_LIST_LEN       3

#define GPIO_LED          16
#define BUTTON_UP         18
#define BUTTON_OK         19
#define BUTTON_DOWN       20

#define SENSOR_SDA        12
#define SENSOR_SCL        13
#define OLED_SDA          14
#define OLED_SCL          15

typedef enum UI_SCREEN {
  HOME                    = 0,
  LIST_ACCELEROMETER      = 1,
  LIST_SYSTEM             = 2,
  ACCELEROMETER           = 11,
  SYSTEM_INFO             = 12,
} UI_SCREEN;
// clang-format on

const char *HOME_TEXT = "HOME";

float accelerations[3];
QueueHandle_t dataQueue;
QueueHandle_t buttonsQueue;

void button_presses(__unused void *params) {
  gpio_init(BUTTON_DOWN);
  gpio_init(BUTTON_OK);
  gpio_init(BUTTON_UP);
  gpio_set_dir(BUTTON_DOWN, GPIO_IN);
  gpio_set_dir(BUTTON_UP, GPIO_IN);
  gpio_set_dir(BUTTON_OK, GPIO_IN);
  gpio_pull_up(BUTTON_DOWN);
  gpio_pull_up(BUTTON_UP);
  gpio_pull_up(BUTTON_OK);

  vTaskDelay(pdMS_TO_TICKS(1000));

  uint8_t pressed_button;

  while (1) {
    pressed_button = 0;

    if (!gpio_get(BUTTON_DOWN)) {
      pressed_button = BUTTON_DOWN;
    } else if (!gpio_get(BUTTON_UP)) {
      pressed_button = BUTTON_UP;
    } else if (!gpio_get(BUTTON_OK)) {
      pressed_button = BUTTON_OK;
    }

    if (pressed_button != 0) {
      if (xQueueSend(buttonsQueue, &pressed_button, 100) == pdPASS) {
        printf("DUPA");
      }
      while (!gpio_get(pressed_button))
        ;
      vTaskDelay(pdMS_TO_TICKS(50));
    }
  }
}

void acceleration_reading(__unused void *params) {
  i2c_init(i2c0, 400000);
  gpio_set_function(SENSOR_SDA, GPIO_FUNC_I2C);
  gpio_set_function(SENSOR_SCL, GPIO_FUNC_I2C);

  init_accelerometer();

  while (1) {
    read_xyz(accelerations);
    vTaskDelay(10);
  }
}

void ui_handler(__unused void *params) {
  i2c_init(i2c1, 400000);
  gpio_set_function(OLED_SDA, GPIO_FUNC_I2C);
  gpio_set_function(OLED_SCL, GPIO_FUNC_I2C);

  oled_init();

  UI_SCREEN screen = HOME;
  UI_SCREEN prev_screen = -1;

  uint8_t button_pressed;

  while (1) {
    vTaskDelay(pdMS_TO_TICKS(50));

    button_pressed = 0;

    if (xQueueReceive(buttonsQueue, &button_pressed, 50) == pdTRUE) {
      if (button_pressed == BUTTON_OK && screen != HOME) {
        screen = (screen + 10) % 20; // OK goes into screen and goes back from it
      } else if (button_pressed == BUTTON_DOWN && screen < 10) {
        screen = screen == 0 ? 2 : (screen - 1) % UI_LIST_LEN;
      } else if (button_pressed == BUTTON_UP && screen < 10) {
        screen = (screen + 1) % UI_LIST_LEN;
      }
    }

    if (screen != prev_screen || screen == ACCELEROMETER) // update only if screen changed or on accelerometer screen
    {
    switch (screen) {
    case HOME:
      oled_clear_screen();
      oled_render_const_string(HOME_TEXT, STRLEN(HOME_TEXT), 1, 0);
      break;

    case LIST_ACCELEROMETER:
      oled_clear_screen();
      oled_render_const_string("ACCELEROMETER", STRLEN("ACCELEROMETER"), 1, 0);
      // oled_render_string("OK TO ENTER", STRLEN("OK TO ENTER"), 1, 16);
      break;

    case LIST_SYSTEM:
      oled_clear_screen();
      oled_render_const_string("SYSTEM INFO", STRLEN("SYSTEM INFO"), 1, 0);
      // oled_render_string("OK TO ENTER", STRLEN("OK TO ENTER"), 1, 16);
      break;

    case ACCELEROMETER:
      if (prev_screen != ACCELEROMETER)
      {
        oled_clear_screen();
        oled_render_const_string("ACCELERATION:", STRLEN("ACCELERATION:"), 1, 0);
        oled_render_const_string("X: ", STRLEN("X: "), 1, 1);
        oled_render_const_string("Y: ", STRLEN("Y: "), 1, 2);
        oled_render_const_string("Z: ", STRLEN("Z: "), 1, 3);
      }

      char x[10];
      char y[10];
      char z[10];
      sprintf(x, "%7.4f g", accelerations[0]);
      sprintf(y, "%7.4f g", accelerations[1]);
      sprintf(z, "%7.4f g", accelerations[2]);

      oled_render_string(x, STRLEN(x), 21, 1);
      oled_render_string(y, STRLEN(y), 21, 2);
      oled_render_string(z, STRLEN(z), 21, 3);
      break;

    case SYSTEM_INFO:
      oled_clear_screen();
      char system_info[21];
      sprintf(system_info, "SDK V.%s", PICO_SDK_VERSION_STRING);
      oled_render_string(system_info, STRLEN(system_info), 1, 0);
      break;
    }
    }

    prev_screen = screen;

    vTaskDelay(10);
  }
}

int main() {
  stdio_init_all();

  TaskHandle_t ui_task_handle, acc_read_task_handle, button_task_handle;
  xTaskCreate(ui_handler, "UI", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, &ui_task_handle);
  xTaskCreate(acceleration_reading, "AccelerationAcquisition", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 2,
              &acc_read_task_handle);
  xTaskCreate(button_presses, "ButtonHandler", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 4,
              &button_task_handle);

  dataQueue = xQueueCreate(512, sizeof(float));
  buttonsQueue = xQueueCreate(3, sizeof(uint8_t));
  vTaskStartScheduler();
}