#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

int main()
{
    stdio_init_all();
    cyw43_arch_init(); //initializing led for task 1
    printf("Hello, world!\n");
    int i = 1;
    
    while (true) {
        int time =0;
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, true);
        uart_putc(uart0,'H');
        uart_puts(uart0,"ello world!\n");
        printf("%d\n",i);
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, false);
        i++;
        sleep_ms(996); //around 4ms are lost in the loop 
    }
}
