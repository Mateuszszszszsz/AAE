#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"


int main()
{
    stdio_init_all();
    cyw43_arch_init();

    uint32_t X = 0;

    while (true) {
        uart_putc(uart0, 'H');
        uart_puts(uart0, "ello world!\n");
        printf("Counter: %d\n", X++);
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, true);
        sleep_ms(100);
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, false);
        sleep_ms(900);
    }
}
