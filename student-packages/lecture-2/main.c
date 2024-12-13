// Pico SDK libraries
#include "pico/stdlib.h"
#include "pico/time.h"

// Standard libraries
#include <stdio.h>
#include <stdbool.h>

#define BUTTONd 20
#define BUTTONe 21

void handler(uint gpio, uint32_t events);
int counter = 0;
int lastCounter = 0;
int delayTime = 20;
unsigned long time = 0;

int main()
{
    // Initialize all standard I/O
    stdio_init_all();
    sleep_ms(5000);
    printf("Starting...\n");

    // Initialize GPIO pins
    gpio_init(BUTTONd);
    gpio_set_dir(BUTTONd, GPIO_IN);
    gpio_pull_up(BUTTONd);

    gpio_init(BUTTONe);
    gpio_set_dir(BUTTONe, GPIO_IN);
    gpio_pull_up(BUTTONe);

    // Enable interrupts for button pin
    gpio_set_irq_enabled_with_callback(BUTTONd, GPIO_IRQ_EDGE_RISE, true, &handler);
    gpio_set_irq_enabled_with_callback(BUTTONe, GPIO_IRQ_EDGE_RISE, true, &handler);

    while (1)
    {
        sleep_ms(100);
        if(lastCounter != counter) {
            printf("\r%i", counter);
            lastCounter = counter;
        }
        
        time = to_ms_since_boot(get_absolute_time());
    }

    return 0;
}

void handler(uint gpio, uint32_t events)
{
    if(gpio == BUTTONd && time - to_ms_since_boot(get_absolute_time()) >= delayTime) {
        counter--;
    }
    else if(gpio == BUTTONe && time - to_ms_since_boot(get_absolute_time()) >= delayTime) {
        counter++;
    }
} 