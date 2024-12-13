#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "stdio.h"

#define LED 15
#define BUTTON 16
const int buttonDebounceTime = 0;

unsigned long time = 0;
bool power = 0;
void handler(uint gpio, uint32_t events);


int main() {
    stdio_init_all();
    sleep_ms(5000); 
    printf("Starting...\n");

    //init le diody
    gpio_init(LED);
    gpio_set_dir(LED, GPIO_OUT);
    gpio_set_function(LED, GPIO_FUNC_PWM);

    //init tlacitka
    gpio_init(BUTTON);
    gpio_set_dir(BUTTON, GPIO_IN);
    gpio_pull_up(BUTTON);

    gpio_set_irq_enabled_with_callback(BUTTON, GPIO_IRQ_EDGE_RISE, true, &handler);
    
    uint slice_num = pwm_gpio_to_slice_num(LED);
    //uint channel = pwm_gpio_to_channel(slice_num);

    pwm_config config = pwm_get_default_config();
    pwm_config_set_wrap(&config, 1000);
    pwm_config_set_clkdiv(&config, 125);

    pwm_init(slice_num, &config, true);
    
    while(1){
        while (power)
        {
            for(int level = 0; level < 255; level++) {
                pwm_set_gpio_level(LED, level);  
               sleep_ms(5);      
            }
            for(int level = 255; level > 0; level--) {
                pwm_set_gpio_level(LED, level);
                sleep_ms(5);
            }

            pwm_set_gpio_level(LED, 0);
        }
        time = to_us_since_boot(get_absolute_time());
    }

    return 0;
}

void handler(uint gpio, uint32_t events){
    if(gpio == BUTTON && time - to_ms_since_boot(get_absolute_time()) >= buttonDebounceTime) {
        power = !power;
    }
}