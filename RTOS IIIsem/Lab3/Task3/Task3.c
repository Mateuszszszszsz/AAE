#include <stdio.h>
#include "pico/stdlib.h"
#include "FreeRTOS.h" 
#include "task.h" 
#include "queue.h" 



#define GPIO_0 2
#define GPIO_1 3
#define GPIO_2 4
#define GPIO_3 5

QueueHandle_t coreQueue;
void blink_task(__unused void *params)
{
    bool led = false;
    gpio_init(GPIO_0);
    gpio_set_dir(GPIO_0,GPIO_OUT);
    while(true)
        {
            if(led)
            {
                vTaskDelay(pdMS_TO_TICKS(33));
            }
            else
            {
                vTaskDelay(pdMS_TO_TICKS(67));   
            }
            gpio_put(GPIO_0,led);
            led = !led;
        }
}

void button_task(__unused void *param)
{
    gpio_init(GPIO_1);
    gpio_set_dir(GPIO_1,GPIO_IN);
    gpio_pull_up(GPIO_1);
    vTaskDelay(pdMS_TO_TICKS(500));


    while(true)
    {
    uint16_t core = get_core_num(); 
    if(!gpio_get(GPIO_0)) //when button is pressed
    {
        while(xQueueSend(coreQueue,&core,100) == errQUEUE_FULL) //send message
        {}
        while(!gpio_get(GPIO_0)) //wait until the button is realeased
        {}
        //maybe wait to ensure?
    }
    }
}

void recieve_task(__unused void *param)
{
    gpio_init(GPIO_2);
    gpio_set_dir(GPIO_2,GPIO_OUT);
    gpio_init(GPIO_3);
    gpio_set_dir(GPIO_3,GPIO_OUT);
    while(true)
    {
        uint16_t core;
        if(xQueueReceive(coreQueue,&core,100)== pdPASS)
        {
            if(core == 0)
            {
                gpio_put(GPIO_2,true);
                vTaskDelay(pdMS_TO_TICKS(100));
                gpio_put(GPIO_2,false);
            }
            else if(core == 1)
            {
                gpio_put(GPIO_3,true);
                vTaskDelay(pdMS_TO_TICKS(100));
                gpio_put(GPIO_3,false);
            }
            else
            {
                gpio_put(GPIO_3,true);
                gpio_put(GPIO_2,true);
                vTaskDelay(pdMS_TO_TICKS(1000));
                gpio_put(GPIO_3,false);
                gpio_put(GPIO_2,true);
            }
        }
    }
}



int main()
{
stdio_init_all();
TaskHandle_t blinkH, buttH, revieceH;

xTaskCreate(blink_task,"blink",configMINIMAL_STACK_SIZE,NULL,tskIDLE_PRIORITY,blinkH);
xTaskCreate(button_task,"button",configMINIMAL_STACK_SIZE,NULL,tskIDLE_PRIORITY,buttH);
xTaskCreate(recieve_task,"recieve",configMINIMAL_STACK_SIZE,NULL,tskIDLE_PRIORITY,revieceH);

xQueueCreate(16,sizeof(uint16_t));
vTaskStartScheduler();
}
