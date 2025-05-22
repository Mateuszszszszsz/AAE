#include <hardware/i2c.h>
#include "FreeRTOS.h"
#include "task.h"

#include "accelerometer.h"

// clang-format off
#define SENSOR_ADDR     0x19
#define CTRL_REG1_ADDR  0x20
#define CTRL_REG4_ADDR  0x23
// clang-format on

void init_accelerometer() {
  uint8_t buf[2];

  buf[0] = CTRL_REG1_ADDR;
  buf[1] = 0b00110111; // 400Hz, Low-power disabled, enable all axes
  i2c_write_blocking(i2c0, SENSOR_ADDR, buf, sizeof(buf), false);
  
  buf[0] = CTRL_REG4_ADDR;
  buf[1] = 0b10000000; // turn on BDU, normal (not high-resolution) mode
  i2c_write_blocking(i2c0, SENSOR_ADDR, buf, sizeof(buf), false);
}

float read_sensor_data(uint8_t reg) {
  uint8_t lsb;
  uint8_t msb;
  uint16_t reading;
  i2c_write_blocking(i2c0, SENSOR_ADDR, &reg, 1, true);
  vTaskDelay(pdTICKS_TO_MS(10));
  i2c_read_blocking(i2c0, SENSOR_ADDR, &lsb, 1, false);

  reg |= 0x01;
  i2c_write_blocking(i2c0, SENSOR_ADDR, &reg, 1, true);
  vTaskDelay(pdTICKS_TO_MS(10));
  i2c_read_blocking(i2c0, SENSOR_ADDR, &msb, 1, false);

  reading = (msb << 8) | lsb;

  return (float)((int16_t)reading * SENSITIVITY) / SCALING;
}

void read_xyz(float *destination) {
  float a_x = read_sensor_data(0x28);
  float a_y = read_sensor_data(0x2A);
  float a_z = read_sensor_data(0x2C);

  destination[0] = a_x;
  destination[1] = a_y;
  destination[2] = a_z;
}
