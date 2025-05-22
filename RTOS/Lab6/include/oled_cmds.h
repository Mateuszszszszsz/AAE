// clang-format off
#define OLED_SET_DISP                       0xAE
#define OLED_SET_MEM_MODE                   0x20
#define OLED_SET_DISP_START_LINE            0x40
#define OLED_SET_SEG_REMAP                  0xA0
#define OLED_SET_MUX_RATIO                  0xA8
#define OLED_SET_COM_OUT_DIR                0xC0
#define OLED_SET_DISP_START_LINE            0x40
#define OLED_SET_CONTRAST                   0x81
#define OLED_SET_CHARGE_PUMP                0x8D
#define OLED_SET_SEG_REMAP                  0xA0
#define OLED_SET_ENTIRE_ON                  0xA4
#define OLED_SET_ALL_ON                     0xA5
#define OLED_SET_NORM_DISP                  0xA6
#define OLED_SET_INV_DISP                   0xA7
#define OLED_SET_MUX_RATIO                  0xA8
#define OLED_SET_DISP                       0xAE
#define OLED_SET_COM_OUT_DIR                0xC0
#define OLED_SET_COM_OUT_DIR_FLIP           0xC0
#define OLED_SET_DISP_OFFSET                0xD3
#define OLED_SET_DISP_CLK_DIV               0xD5
#define OLED_SET_PRECHARGE                  0xD9
#define OLED_SET_COM_PIN_CFG                0xDA
#define OLED_SET_VCOM_DESEL                 0xDB
#define OLED_WRITE_MODE                     0xFE
#define OLED_READ_MODE                      0xFF
#define OLED_SET_SCROLL                     0x2E
#define OLED_SET_COL_ADDR                   0x21
#define OLED_SET_PAGE_ADDR                  0x22

#define OLED_PAGE_HEIGHT                    8
#define OLED_NUM_PAGES                      (OLED_HEIGHT / OLED_PAGE_HEIGHT)
#define OLED_BUF_LEN                        (OLED_NUM_PAGES * OLED_WIDTH)
// clang-format on