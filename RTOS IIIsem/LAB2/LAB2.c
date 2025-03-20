#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pll.h"
#include "hardware/clocks.h"
#include "hardware/structs/pll.h"
#include "hardware/structs/clocks.h"
#include "hardware/xosc.h"


void print_clock_frequencies() {
    printf("System PLL: %u Hz\n", frequency_count_khz(CLOCKS_FC0_SRC_VALUE_PLL_SYS_CLKSRC_PRIMARY) * 1000);
    printf("USB PLL: %u Hz\n", frequency_count_khz(CLOCKS_FC0_SRC_VALUE_PLL_USB_CLKSRC_PRIMARY) * 1000);
    printf("ROSC: %u Hz\n", frequency_count_khz(CLOCKS_FC0_SRC_VALUE_ROSC_CLKSRC) * 1000);
    printf("clk_sys: %u Hz\n", clock_get_hz(clk_sys));
    printf("clk_peri: %u Hz\n", clock_get_hz(clk_peri));
    printf("clk_usb: %u Hz\n", clock_get_hz(clk_usb));
    printf("clk_adc: %u Hz\n", clock_get_hz(clk_adc));

}

void configure_clocks() { // configuring clk ref and clk peri to external oscilator
    xosc_init();
    clock_configure(clk_ref, CLOCKS_CLK_REF_CTRL_SRC_VALUE_XOSC_CLKSRC, 0, 12 * MHZ, 12 * MHZ);
    clock_configure(clk_peri, 0, CLOCKS_CLK_PERI_CTRL_AUXSRC_VALUE_XOSC_CLKSRC, 12 * MHZ, 12 * MHZ);
    stdio_init_all();
}

void switch_clock(char mode) //switching the frequency with usage of pll
{
    switch (mode)
    {
        case '0':
        clock_configure(clk_sys, CLOCKS_CLK_SYS_CTRL_SRC_VALUE_CLK_REF, 0, 12 * MHZ, 12 * MHZ);
        pll_deinit(pll_sys);
        stdio_init_all();
        printf("Stopped System PLL: %fMHz\n", frequency_count_mhz(CLOCKS_FC0_SRC_VALUE_PLL_SYS_CLKSRC_PRIMARY));
        print_clock_frequencies();
        break;
        case '1':
            printf("Switching to 18 MHZ");
            set_sys_clock_pll(882 * MHZ, 7, 7);
            stdio_init_all();
            printf("Changed System PLL Freguency to: %fMHz\n", frequency_count_mhz(CLOCKS_FC0_SRC_VALUE_PLL_SYS_CLKSRC_PRIMARY));
        break;
        case '2':
            printf("Switching to 60 MHZ");
            set_sys_clock_pll(1440 * MHZ, 12, 6);
            stdio_init_all();
            printf("Changed System PLL Freguency to: %fMHz\n", frequency_count_mhz(CLOCKS_FC0_SRC_VALUE_PLL_SYS_CLKSRC_PRIMARY));
        break;
        case '3':
            printf("Switching to 140 MHZ");
            set_sys_clock_pll(1440 * MHZ, 6, 3);
            stdio_init_all();
            printf("Changed System PLL Freguency to: %fMHz\n", frequency_count_mhz(CLOCKS_FC0_SRC_VALUE_PLL_SYS_CLKSRC_PRIMARY));
        break;
    }   

}

int main()
{
    stdio_init_all();
    print_clock_frequencies();
    configure_clocks();
    printf("Clocks setup\n");
    while (true) {
        print_clock_frequencies();
        while(true)
        {
            char mode = getchar();
            switch_clock(mode);
        }
    }
}
