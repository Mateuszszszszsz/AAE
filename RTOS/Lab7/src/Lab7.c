#pragma GCC optimize("O0")

#include "FreeRTOS.h"
#include "accelerometer.h"
#include "clap.h"
#include "hardware/dma.h"
#include "hardware/i2c.h"
#include "hardware/pwm.h"
#include "math.h"
#include "oled.h"
#include "pico/malloc.h"
#include "pico/stdlib.h"
#include "queue.h"
#include "semphr.h"
#include "synth.h"
#include "task.h"
#include <stdio.h>

// clang-format off
#define STRLEN(x)         sizeof(x) / sizeof(char)

#define BUTTON_UP         18
#define BUTTON_OK         19
#define BUTTON_DOWN       20

#define SENSOR_SDA        12
#define SENSOR_SCL        13
#define OLED_SDA          14
#define OLED_SCL          15

#define SYNTH_PWM         0

typedef enum UI_SCREEN {
  HOME                    = 0,
  LIST_ACCELEROMETER      = 1,
  LIST_SYSTEM             = 2,
  LIST_CLAPPER            = 3,
  LIST_SYNTH              = 4,
  ACCELEROMETER           = 11,
  SYSTEM_INFO             = 12,
  CLAPPER                 = 13,
  SYNTH                   = 14,
} UI_SCREEN;

#define UI_LIST_LEN       5
// clang-format on

TaskHandle_t ui_task_handle, acc_read_task_handle, button_task_handle, clap_task_handle, synth_task_handle;

UI_SCREEN screen = HOME;

float accelerations[3];

uint clap_counter = 0;

uint16_t audio_buffer[AUDIO_BUFFER_SIZE];
SemaphoreHandle_t audioBufferMutex;

uint frequencies[] = {200, 400, 600, 800, 1000, 1200, 1400, 1600, 1800, 2000};
uint8_t selected_frequency_idx = 0;
SemaphoreHandle_t adcMutex;

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

void clap_detection(__unused void *params) {
  int dma_chan = clap_init_microphone(audio_buffer);

  while (1) {
    xSemaphoreTake(audioBufferMutex, portMAX_DELAY);
    __compiler_memory_barrier();
    size_t dma_write_index = dma_hw->ch[dma_chan].write_addr - (uintptr_t)audio_buffer;
    dma_write_index %= AUDIO_BUFFER_SIZE;

    size_t start = (dma_write_index + (AUDIO_BUFFER_SIZE / 5));
    size_t end = (start + (AUDIO_BUFFER_SIZE / 5) * 4) % AUDIO_BUFFER_SIZE;

    bool clap_detected = false;

    clap_detected = clap_detect(audio_buffer, start, end);
    xSemaphoreGive(audioBufferMutex);

    if (clap_detected) {
      clap_counter++;
    }
  }
}

void synth(__unused void *params) {
  init_synth(SYNTH_PWM);

  uint8_t current_frequency_idx;

  while (1) {
    uint16_t adc_raw = adc_read();
    synth_set_volume((float)adc_raw / 4095.0f);

    if (current_frequency_idx != selected_frequency_idx) {
      synth_set_frequency(frequencies[selected_frequency_idx]);
    }
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}

UI_SCREEN handle_button_press(UI_SCREEN current_screen) {
  uint8_t button_pressed = 0;
  UI_SCREEN next_screen = current_screen;

  if (xQueueReceive(buttonsQueue, &button_pressed, 50) == pdTRUE) {
    switch (button_pressed) {
    case BUTTON_OK:
      if (current_screen != HOME) {
        next_screen = (current_screen + 10) % 20;
      }

      switch (current_screen) {
      case CLAPPER:
        clap_suspend();
        vTaskDelay(pdMS_TO_TICKS(100));
        vTaskSuspend(clap_task_handle);
        break;
      case SYNTH:
        synth_suspend();
        vTaskDelay(pdMS_TO_TICKS(100));
        vTaskSuspend(synth_task_handle);
        break;
      default:
        break;
      }

      switch (next_screen) {
      case CLAPPER:
        clap_resume();
        vTaskDelay(pdMS_TO_TICKS(100));
        vTaskResume(clap_task_handle);
        break;
      case SYNTH:
        synth_resume();
        vTaskDelay(pdMS_TO_TICKS(100));
        vTaskResume(synth_task_handle);
        break;
      default:
        break;
      }
      break;

    case BUTTON_DOWN:
      if (current_screen < 10) {
        next_screen = current_screen == HOME ? UI_LIST_LEN - 1 : (current_screen - 1) % UI_LIST_LEN;
      } else if (current_screen == SYNTH) {
        selected_frequency_idx = selected_frequency_idx == 0 ? 9 : (selected_frequency_idx - 1);
      }
      break;

    case BUTTON_UP:
      if (current_screen < 10) {
        next_screen = (current_screen + 1) % UI_LIST_LEN;
      } else if (current_screen == SYNTH) {
        selected_frequency_idx = (selected_frequency_idx + 1) % 10;
      }
      break;

    default:
      next_screen = current_screen;
    }
  }

  return next_screen;
}

void ui_handler(__unused void *params) {
  i2c_init(i2c1, 400000);
  gpio_set_function(OLED_SDA, GPIO_FUNC_I2C);
  gpio_set_function(OLED_SCL, GPIO_FUNC_I2C);

  oled_init();

  UI_SCREEN prev_screen = -1;

  while (1) {
    vTaskDelay(pdMS_TO_TICKS(50));

    screen = handle_button_press(screen);

    if (screen != prev_screen || screen == ACCELEROMETER || screen == CLAPPER || screen == SYNTH) {
      switch (screen) {
      case HOME:
        oled_clear_screen();
        oled_render_const_string("HOME", STRLEN("HOME"), 1, 0);
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

      case LIST_SYNTH:
        oled_clear_screen();
        oled_render_const_string("SYNTH", STRLEN("SYNTH"), 1, 0);
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
        sprintf(x, "%7.4f G", accelerations[0]);
        sprintf(y, "%7.4f G", accelerations[1]);
        sprintf(z, "%7.4f G", accelerations[2]);

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

      case SYNTH:
        if (prev_screen != SYNTH) {
          oled_clear_screen();
          oled_render_const_string("SYNTH", STRLEN("SYNTH"), 1, 0);
          oled_render_const_string("FREQ:", STRLEN("FREQ:"), 1, 1);
          oled_render_const_string("HZ", STRLEN("HZ") + 1, 6 * (STRLEN("FREQ:") + 4), 1);
          oled_render_const_string("VOL:", STRLEN("VOL:"), 1, 2);
          oled_render_const_string("%", STRLEN("%") + 1, 6 * (STRLEN("VOL:") + 3), 2);
        }

        char freq_str[4];
        char vol_str[3];
        sprintf(freq_str, "%4d", synth_get_frequency());
        sprintf(vol_str, "%3d", synth_get_volume());
        oled_render_string(freq_str, STRLEN(freq_str), 6 * STRLEN("FREQ:"), 1);
        oled_render_string(vol_str, STRLEN(vol_str), 6 * STRLEN("VOL:"), 2);
      }
    }

    prev_screen = screen;

    vTaskDelay(10);
  }
}

int main() {
  stdio_init_all();

  xTaskCreate(button_presses, "ButtonHandler", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1,
              &button_task_handle);
  xTaskCreate(acceleration_reading, "AccelerationAcquisition", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 2,
              &acc_read_task_handle);
  xTaskCreate(clap_detection, "Clapper", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 4, &clap_task_handle);
  xTaskCreate(synth, "Synth", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 4, &synth_task_handle);
  xTaskCreate(ui_handler, "UI", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 8, &ui_task_handle);

  vTaskSuspend(clap_task_handle);
  vTaskSuspend(synth_task_handle);

  audioBufferMutex = xSemaphoreCreateMutex();
  adcMutex = xSemaphoreCreateMutex();

  buttonsQueue = xQueueCreate(3, sizeof(uint8_t));
  vTaskStartScheduler();
}