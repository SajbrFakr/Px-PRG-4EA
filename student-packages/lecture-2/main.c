// Pico SDK libraries
#include "pico/stdlib.h"

// Standard libraries
#include <stdio.h>
#include <stdbool.h>

#define BUTTONd 20
#define BUTTONe 21

void handler(uint gpio, uint32_t events);

int main()
{
    // Initialize all standard I/O
    stdio_init_all();
    sleep_ms(1000);
    printf("Starting...\n");

    // Initialize GPIO pins
    gpio_init(BUTTONd);
    gpio_set_dir(BUTTONd, GPIO_OUT);

    gpio_init(BUTTONe);
    gpio_set_dir(BUTTONe, GPIO_IN);

    // Enable interrupts for button pin
    gpio_set_irq_enabled_with_callback(BUTTONd, GPIO_IRQ_EDGE_RISE, true, &handler);
    gpio_set_irq_enabled_with_callback(BUTTONe, GPIO_IRQ_EDGE_RISE, true, &handler);

    // Initialize variables
    int counter = 0;
    int lastCounter = counter;

    // Printing counter state if there is a change
    if(lastCounter != counter){
        printf(\ncounter);
    }

    return 0;
}

void handler(uint gpio, uint32_t events)
{
    if(gpio == BUTTONd) counter--;
    if(gpio == BUTTONe) counter++;
    lastCounter = counter;
}