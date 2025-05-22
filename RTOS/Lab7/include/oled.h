#pragma once

// clang-format off
#define OLED_WIDTH          128
#define OLED_HEIGHT         32
#define OLED_I2C_ADDR       0x3C
#define OLED_I2C_CHANNEL    i2c1
// clang-format on

void oled_init();
void oled_render(uint8_t *buffer, int bufferlen, uint8_t start_x, uint8_t end_x, uint8_t start_y, uint8_t end_y);
void oled_render_const_string(const char *string, uint8_t len, uint8_t start_pos, uint8_t line);
void oled_render_string(char *string, uint8_t len, uint8_t start_pos, uint8_t line);
void oled_render_char(char c, int x, int y);
void oled_clear_area(uint start_x, uint end_x, uint start_y, uint end_y);
void oled_clear_screen();