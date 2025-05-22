#pragma GCC optimize("O0")

#include "FreeRTOS.h"
#include "accelerometer.h"
#include "hardware/dma.h"
#include "hardware/i2c.h"
#include "math.h"
#include "microphone.h"
#include "oled.h"
#include "pico/malloc.h"
#include "pico/stdlib.h"
#include "queue.h"
#include "semphr.h"
#include "task.h"
#include <stdio.h>

// clang-format off
#define STRLEN(x)         sizeof(x) / sizeof(char)
#define UI_LIST_LEN       4

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
  LIST_CLAPPER            = 3,
  ACCELEROMETER           = 11,
  SYSTEM_INFO             = 12,
  CLAPPER                  = 13,
} UI_SCREEN;
// clang-format on

const char *HOME_TEXT = "HOME";

float accelerations[3];
uint clap_counter = 0;
uint16_t audio_buffer[AUDIO_BUFFER_SIZE];
SemaphoreHandle_t audioBufferMutex;
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
      xQueueSend(buttonsQueue, &pressed_button, 100);

      while (!gpio_get(pressed_button))
        ;
      vTaskDelay(pdMS_TO_TICKS(100));
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

bool detect_clap(size_t start, size_t end) {
  const size_t WINDOW_SIZE = 240;
  // const uint ENERGY_THRESHOLD = 10000;
  const float RISE_RATIO_THRESHOLD = 5.0f;
  const size_t STEP = 60;
  const uint COOLDOWN_TICKS = pdMS_TO_TICKS(200);

  static TickType_t last_clap_tick = 0;

  TickType_t now = xTaskGetTickCount();
  if (now - last_clap_tick < COOLDOWN_TICKS)
    return false;

  start %= AUDIO_BUFFER_SIZE;
  end %= AUDIO_BUFFER_SIZE;

  size_t buffer_length = (end + AUDIO_BUFFER_SIZE - start) % AUDIO_BUFFER_SIZE;

  float prev_energy = 0;
  size_t i = start;

  int delay_counter = 0;

  for (size_t count = 0; count + 2 * WINDOW_SIZE <= buffer_length; count += STEP) {
    float energy = 0;
    for (size_t j = 0; j < WINDOW_SIZE; j++) {
      size_t idx = (i + j) % AUDIO_BUFFER_SIZE;
      float sample = (float)audio_buffer[idx];
      energy += sample * sample;
    }

    float energy_ratio = energy / prev_energy;
    if (prev_energy > 0 && energy_ratio > RISE_RATIO_THRESHOLD) {
      last_clap_tick = now;
      return true;
    }

    prev_energy = energy;
    i = (i + STEP) % AUDIO_BUFFER_SIZE;
  }

  return false;
}

void clap_detection(__unused void *params) {
  int dma_chan = init_microphone(audio_buffer);

  while (1) {
    xSemaphoreTake(audioBufferMutex, portMAX_DELAY);
    __compiler_memory_barrier();
    size_t dma_write_index = dma_hw->ch[dma_chan].write_addr - (uintptr_t)audio_buffer;
    dma_write_index %= AUDIO_BUFFER_SIZE;

    size_t start = (dma_write_index + (AUDIO_BUFFER_SIZE / 5));
    size_t end = (start + (AUDIO_BUFFER_SIZE / 5) * 4) % AUDIO_BUFFER_SIZE;

    bool clap_detected = false;

    if (start < end) {
      clap_detected = detect_clap(start, end);
    } else {
      clap_detected = detect_clap(start, AUDIO_BUFFER_SIZE) || detect_clap(0, end);
    }
    xSemaphoreGive(audioBufferMutex);

    if (clap_detected) {
      clap_counter++;
    }
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
        screen = screen == 0 ? 3 : (screen - 1) % UI_LIST_LEN;
      } else if (button_pressed == BUTTON_UP && screen < 10) {
        screen = (screen + 1) % UI_LIST_LEN;
      }
    }

    if (screen != prev_screen || screen == ACCELEROMETER ||
        screen == CLAPPER) // update only if screen changed or on accelerometer screen
    {
      switch (screen) {
      case HOME:
        oled_clear_screen();
        oled_render_const_string(HOME_TEXT, STRLEN(HOME_TEXT), 1, 0);
        break;

      case LIST_ACCELEROMETER:
        oled_clear_screen();
        oled_render_const_string("ACCELEROMETER", STRLEN("ACCELEROMETER"), 1, 0);
        oled_render_const_string("OK TO ENTER", STRLEN("OK TO ENTER"), 1, 1);
        break;

      case LIST_SYSTEM:
        oled_clear_screen();
        oled_render_const_string("SYSTEM INFO", STRLEN("SYSTEM INFO"), 1, 0);
        oled_render_const_string("OK TO ENTER", STRLEN("OK TO ENTER"), 1, 1);
        break;

      case LIST_CLAPPER:
        oled_clear_screen();
        oled_render_const_string("CLAPPER", STRLEN("CLAPPER"), 1, 0);
        oled_render_const_string("OK TO ENTER", STRLEN("OK TO ENTER"), 1, 1);
        break;

      case ACCELEROMETER:
        if (prev_screen != ACCELEROMETER) {
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

      case CLAPPER:
        if (prev_screen != CLAPPER) {
          oled_clear_screen();
          oled_render_const_string("CLAPS: ", STRLEN("CLAPS: "), 1, 1);
        }
        char clap_count_str[5];
        sprintf(clap_count_str, "%d", clap_counter);
        oled_render_string(clap_count_str, STRLEN(clap_count_str), 6 * STRLEN("Claps: "), 1);
        break;
      }
    }

    prev_screen = screen;

    vTaskDelay(10);
  }
}

int main() {
  stdio_init_all();

  TaskHandle_t ui_task_handle, acc_read_task_handle, button_task_handle, clap_task_handle;
  xTaskCreate(button_presses, "ButtonHandler", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1,
              &button_task_handle);
  xTaskCreate(acceleration_reading, "AccelerationAcquisition", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 2,
              &acc_read_task_handle);
  xTaskCreate(clap_detection, "Clapper", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 4, &clap_task_handle);
  xTaskCreate(ui_handler, "UI", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 8, &ui_task_handle);

  audioBufferMutex = xSemaphoreCreateMutex();

  buttonsQueue = xQueueCreate(3, sizeof(uint8_t));
  vTaskStartScheduler();
}