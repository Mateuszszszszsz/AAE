#include "hardware/clocks.h"
#include "hardware/pll.h"
#include "hardware/xosc.h"
#include "pico/stdlib.h"
#include <stdio.h>

int main() {
  stdio_init_all();

  printf("\nSystem PLL clock frequency: %fMHz\n", frequency_count_mhz(CLOCKS_FC0_SRC_VALUE_PLL_SYS_CLKSRC_PRIMARY));
  printf("USB PLL clock frequency: %fMHz\n", frequency_count_mhz(CLOCKS_FC0_SRC_VALUE_PLL_USB_CLKSRC_PRIMARY));
  printf("ROSC clock frequency: %fMHz\n", frequency_count_mhz(CLOCKS_FC0_SRC_VALUE_ROSC_CLKSRC));
  printf("CLK SYS clock frequency: %fMHz\n", frequency_count_mhz(CLOCKS_FC0_SRC_VALUE_CLK_SYS));
  printf("CLK PERI clock frequency: %fMHz\n", frequency_count_mhz(CLOCKS_FC0_SRC_VALUE_CLK_PERI));
  printf("CLK USB clock frequency: %fMHz\n", frequency_count_mhz(CLOCKS_FC0_SRC_VALUE_CLK_USB));
  printf("CLK ADC clock frequency: %fMHz\n", frequency_count_mhz(CLOCKS_FC0_SRC_VALUE_CLK_ADC));
  printf("CLK XOSC clock frequency: %fMHz\n", frequency_count_mhz(CLOCKS_FC0_SRC_VALUE_XOSC_CLKSRC));

  xosc_init();

  printf("XOSC initialized\n");

  // Configure clk_ref to 12MHz from XOSC(external crystal oscillator)
  clock_configure(clk_ref, CLOCKS_CLK_REF_CTRL_SRC_VALUE_XOSC_CLKSRC, 0, 12 * MHZ, 12 * MHZ);

  // Configure clk_peri to 12MHz
  clock_configure(clk_peri, 0, CLOCKS_CLK_PERI_CTRL_AUXSRC_VALUE_XOSC_CLKSRC, 12 * MHZ, 12 * MHZ);

  stdio_init_all();

  printf("\nSystem PLL clock frequency: %fMHz\n", frequency_count_mhz(CLOCKS_FC0_SRC_VALUE_PLL_SYS_CLKSRC_PRIMARY));
  printf("USB PLL clock frequency: %fMHz\n", frequency_count_mhz(CLOCKS_FC0_SRC_VALUE_PLL_USB_CLKSRC_PRIMARY));
  printf("ROSC clock frequency: %fMHz\n", frequency_count_mhz(CLOCKS_FC0_SRC_VALUE_ROSC_CLKSRC));
  printf("CLK SYS clock frequency: %fMHz\n", frequency_count_mhz(CLOCKS_FC0_SRC_VALUE_CLK_SYS));
  printf("CLK PERI clock frequency: %fMHz\n", frequency_count_mhz(CLOCKS_FC0_SRC_VALUE_CLK_PERI));
  printf("CLK USB clock frequency: %fMHz\n", frequency_count_mhz(CLOCKS_FC0_SRC_VALUE_CLK_USB));
  printf("CLK ADC clock frequency: %fMHz\n", frequency_count_mhz(CLOCKS_FC0_SRC_VALUE_CLK_ADC));

  while (true) {
    char pll_div = getchar();

    switch (pll_div) {
    case '0':
      clock_configure(clk_sys, CLOCKS_CLK_SYS_CTRL_SRC_VALUE_CLK_REF, 0, 12 * MHZ, 12 * MHZ);
      pll_deinit(pll_sys);
      stdio_init_all();
      printf("Stopped System PLL: %fMHz\n", frequency_count_mhz(CLOCKS_FC0_SRC_VALUE_PLL_SYS_CLKSRC_PRIMARY));
      break;
    case '1': // 18MHz
      set_sys_clock_pll(882 * MHZ, 7, 7);
      stdio_init_all();
      printf("Changed System PLL to: %fMHz\n", frequency_count_mhz(CLOCKS_FC0_SRC_VALUE_PLL_SYS_CLKSRC_PRIMARY));
      break;
    case '2': // 40 MHz
      set_sys_clock_pll(1440 * MHZ, 6, 6);
      stdio_init_all();
      printf("Changed System PLL to: %fMHz\n", frequency_count_mhz(CLOCKS_FC0_SRC_VALUE_PLL_SYS_CLKSRC_PRIMARY));
      break;
    case '3': // 80MHz
      set_sys_clock_pll(1440 * MHZ, 6, 3);
      stdio_init_all();
      printf("Changed System PLL to: %fMHz\n", frequency_count_mhz(CLOCKS_FC0_SRC_VALUE_PLL_SYS_CLKSRC_PRIMARY));
      break;
      default:
      break;
    }
  }
}
