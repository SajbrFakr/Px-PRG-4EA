#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "stdio.h"

#define LED 0

int main() {
    stdio_init_all();
    sleep_ms(5000); 
    printf("Starting...\n");

    gpio_init(LED);
    gpio_set_dir(LED, GPIO_OUT);
    gpio_set_function(LED, GPIO_FUNC_PWM);
    
    //uint channel = pwm_gpio_to_channel(LED);
    uint slice_num = pwm_gpio_to_slice_num(LED);

    pwm_config config = pwm_get_default_config();
    pwm_config_set_wrap(&config, 1000);
    pwm_config_set_clkdiv(&config, 125);

    pwm_init(slice_num, &config, true);

    while (1)
    {
        for(int level = 0; level < 255; level++) {
            pwm_set_gpio_level(LED, level);        
        }
        for(int level = 255; level > 0; level--) {
            pwm_set_gpio_level(LED, level);
        }
    }
    
}