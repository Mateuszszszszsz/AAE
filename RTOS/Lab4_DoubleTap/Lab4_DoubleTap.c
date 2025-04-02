#include "FreeRTOS.h"
#include "hardware/i2c.h"
#include "math.h"
#include "pico/stdlib.h"
#include "queue.h"
#include "task.h"
#include <stdio.h>

#define GPIO_LED 16
#define SDA 14
#define SCL 15

#define SENSITIVITY 0.00390625f // For normal mode: 4g (range) / 2^10 (discrete levels)
#define SCALING 64.0f
#define THRESHOLD .8f

const int SENSOR_ADDR = 0x19;
const uint8_t CTRL_REG1_ADDR = 0x20;
const uint8_t CTRL_REG4_ADDR = 0x23;

QueueHandle_t dataQueue;

void init_sensor() {
  uint8_t buf[2];

  buf[0] = CTRL_REG1_ADDR;
  buf[1] = 0b01110100; // 400Hz, Low-power disabled, only Z-axis enabled
  i2c_write_blocking(i2c1, SENSOR_ADDR, buf, sizeof(buf), false);

  buf[0] = CTRL_REG4_ADDR;
  buf[1] = 0b10000000; // turn on BDU, normal (not high-resolution) mode, rest not covered in documentation
}

float read_sensor_data(uint8_t reg) {
  uint8_t lsb;
  uint8_t msb;
  uint16_t reading;
  i2c_write_blocking(i2c1, SENSOR_ADDR, &reg, 1, true);
  i2c_read_blocking(i2c1, SENSOR_ADDR, &lsb, 1, false);

  reg |= 0x01;
  i2c_write_blocking(i2c1, SENSOR_ADDR, &reg, 1, true);
  i2c_read_blocking(i2c1, SENSOR_ADDR, &msb, 1, false);

  reading = (msb << 8) | lsb;

  return (float)((int16_t)reading * SENSITIVITY) / SCALING;
}

void acceleration_reading(__unused void *params) // task 1
{
  i2c_init(i2c1, 400000);
  gpio_set_function(SDA, GPIO_FUNC_I2C);
  gpio_set_function(SCL, GPIO_FUNC_I2C);

  init_sensor();

  while (1) {
    float acceleration = read_sensor_data(0x2C); // Z-acceleration register
    xQueueSend(dataQueue, &acceleration, 100);
  }
}

void analyze_data(__unused void *params) {
  gpio_init(GPIO_LED);
  gpio_set_dir(GPIO_LED, GPIO_OUT);

  float last_acceleration = 0;
  float acceleration;
  TickType_t last_tap_time = 0;
  TickType_t current_time;

  const TickType_t TAP_MIN_SPACING = pdMS_TO_TICKS(50);
  const TickType_t TAP_TIMEOUT = pdMS_TO_TICKS(500);

  TickType_t led_on_timer = 0;

  while (1) {
    if (xQueueReceive(dataQueue, &acceleration, 100) == pdPASS) {
      current_time = xTaskGetTickCount();

      if (fabs(acceleration - last_acceleration) > THRESHOLD) {
        if ((current_time - last_tap_time) > TAP_MIN_SPACING) {

          if ((current_time - last_tap_time) < pdMS_TO_TICKS(TAP_TIMEOUT)) {
            printf("Double Tap Detected!\n");
            gpio_put(GPIO_LED, 1);
            led_on_timer = 2000 / portTICK_PERIOD_MS;
          }

          last_tap_time = current_time;
        }
      }

      last_acceleration = acceleration;
    }

    if (led_on_timer > 0)
    {
      if (--led_on_timer == 0)
      {
        gpio_put(GPIO_LED, 0);
      }
    }
  }
}

int main() {
  stdio_init_all();

  TaskHandle_t task_1_handle, task_2_handle;
  xTaskCreate(acceleration_reading, "DataAcquiring", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1,
              &task_1_handle);
  xTaskCreate(analyze_data, "DataAnalysis", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 2, &task_2_handle);

  dataQueue = xQueueCreate(512, sizeof(float));
  vTaskStartScheduler();
}