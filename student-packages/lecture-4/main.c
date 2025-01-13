#include "hardware/adc.h"
#include "hardware/pwm.h"
#include "hardware/gpio.h"

#define LED 15
#define potenciometr 16

uint16_t map_value(uint16_t value, uint16_t in_min, uint16_t in_max, 
                   uint16_t out_min, uint16_t out_max) {
    return (value - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

int main(){
    adc_init();
    adc_gpio_init(26);  // Initialize GPIO26 for ADC use
    adc_select_input(0); // Select ADC0 input

    gpio_init(LED);
    gpio_set_dir(LED, GPIO_OUT);
    gpio_set_function(LED, GPIO_FUNC_PWM);

    uint slice_num = pwm_gpio_to_slice_num(LED);
    pwm_config config = pwm_get_default_config();
    pwm_config_set_wrap(&config, 1000);
    pwm_config_set_clkdiv(&config, 125);
    pwm_init(slice_num, &config, true);

    uint16_t result = adc_read();

    result = map_value(result, 0, 4095, 0, 255);
    pwm_set_gpio_level(LED, result);
}