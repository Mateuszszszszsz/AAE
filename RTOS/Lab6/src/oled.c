#pragma GCC optimize("O0")

#include <hardware/i2c.h>
#include <stdlib.h>
#include <string.h>

#include "font.h"
#include "oled.h"
#include "oled_cmds.h"

void oled_send_cmd(uint8_t cmd) {
  uint8_t buf[2] = {0x80, cmd}; // control byte + command data
  i2c_write_blocking(OLED_I2C_CHANNEL, OLED_I2C_ADDR, buf, 2, false);
}

void oled_send_cmd_stream(uint8_t *cmds, uint len) {
  for (int i = 0; i < len; i++) {
    oled_send_cmd(cmds[i]);
  }
}

void oled_send_buffer(uint8_t buffer[], int len) {
  uint8_t *tempbuf = malloc(len + 1);
  tempbuf[0] = 0x40; // control byte
  memcpy(tempbuf + 1, buffer, len);

  i2c_write_blocking(OLED_I2C_CHANNEL, OLED_I2C_ADDR, tempbuf, len + 1, false);

  free(tempbuf);
}

void oled_render(uint8_t *buffer, int bufferlen, uint8_t start_x, uint8_t end_x, uint8_t start_y, uint8_t end_y) {
  uint8_t cmds[] = {OLED_SET_COL_ADDR, start_x, end_x, OLED_SET_PAGE_ADDR, start_y, end_y};

  oled_send_cmd_stream(cmds, count_of(cmds));
  oled_send_buffer(buffer, bufferlen);
}

void oled_clear_area(uint start_x, uint end_x, uint start_y, uint end_y)
{
  uint buflen = (end_x - start_x) * (end_y - start_y);

  uint8_t buf[buflen];
  memset(buf, 0, buflen);
  oled_render(buf, buflen, start_x, end_x, start_y, end_y);
}

void oled_clear_screen() {
  uint8_t buf[OLED_BUF_LEN];
  memset(buf, 0, OLED_BUF_LEN);
  oled_render(buf, OLED_BUF_LEN, 0, OLED_WIDTH - 1, 0, OLED_HEIGHT - 1);
}

void oled_init() {
  uint8_t cmds[] = {
      OLED_SET_DISP, // turn off display
      /* memory mapping */
      OLED_SET_MEM_MODE, // set memory address mode
      0x00,              // horizontal addressing
      /* resolution and layout */
      OLED_SET_DISP_START_LINE,    // display start line to 0
      OLED_SET_SEG_REMAP | 0x01,   // set segment re-map, column address 127 is mapped to SEG0
      OLED_SET_MUX_RATIO,          // set multiplex ratio
      OLED_HEIGHT - 1,             // display height - 1
      OLED_SET_COM_OUT_DIR | 0x08, // set common output scan direction to bottom-up, COM[N-1] to COM0
      OLED_SET_DISP_OFFSET,        // set display offset
      0x00,                        // no offset
      OLED_SET_COM_PIN_CFG,        // set COM (common) pins hardware configuration. Board specific magic number.
      0x02,
      /* timing and driving scheme */
      OLED_SET_DISP_CLK_DIV, // set display clock divide ratio
      0x80,                  // div ratio of 1, standard freq
      OLED_SET_PRECHARGE,    // set pre-charge period
      0xF1,                  // Vcc internally generated on our board
      OLED_SET_VCOM_DESEL,   // set VCOMH deselect level
      0x30,                  // 0.83xVcc
      /* display */
      OLED_SET_CONTRAST, // set contrast control
      0xFF,
      OLED_SET_ENTIRE_ON,     // set entire display on to follow RAM content
      OLED_SET_NORM_DISP,     // set normal (not inverted) display
      OLED_SET_CHARGE_PUMP,   // set charge pump
      0x14,                   // Vcc internally generated on our board
      OLED_SET_SCROLL | 0x00, // deactivate horizontal scrolling if set. This is necessary as memory writes will corrupt
                              // if scrolling was enabled
      OLED_SET_DISP | 0x01,   // turn display on
  };

  oled_send_cmd_stream(cmds, count_of(cmds));

  oled_clear_screen();
}

uint get_font_index(char c) {
  if (c >= 'A' && c <= 'Z') {
    return c - 'A' + 1;
  } else if (c >= '0' && c <= '9') {
    return c - '0' + 27;
  } else {
    switch (c) {
    case '.':
      return 37;
    case ':':
      return 38;
    case '-':
      return 39;
    }
  }
  return 0;
}

void oled_render_char(char c, int x, int y) {
  uint font_idx = get_font_index(c);
  uint8_t *char_data = (font_6x8 + font_idx);

  oled_render(char_data, 6 * 8, x, x + 6, y, y + 8);
}

void oled_render_const_string(const char *string, uint8_t len, uint8_t start_x, uint8_t start_y) {
  if (start_x < 0 || start_x + len * 6 > OLED_WIDTH) {
    return;
  }

  uint buflen = len * 6;
  uint8_t buf[buflen];
  memset(buf, 0, buflen);

  for (int i = 0; i < len; i++) {
    uint font_idx = get_font_index(string[i]);

    for (int j = 0; j < 6; j++) {
      buf[i * 6 + j] = font_6x8[font_idx * 6 + j];
    }
  }

  oled_render(buf, buflen, start_x, start_x + len * 6, start_y, start_y + 8);
}

void oled_render_string(char *string, uint8_t len, uint8_t start_x, uint8_t start_y) {
  if (start_x < 0 || start_x + len * 6 > OLED_WIDTH) {
    return;
  }

  uint buflen = len * 6;
  uint8_t buf[buflen];
  memset(buf, 0, buflen);

  for (int i = 0; i < len; i++) {
    uint font_idx = get_font_index(string[i]);

    for (int j = 0; j < 6; j++) {
      buf[i * 6 + j] = font_6x8[font_idx * 6 + j];
    }
  }

  oled_render(buf, buflen, start_x, start_x + len * 6, start_y, start_y + 8);
}