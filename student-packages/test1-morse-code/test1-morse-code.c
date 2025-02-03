#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"
#include "pico/time.h"
#include <ctype.h>

// TODO: Include necessary libraries

// TODO: Define your pin configuration
// Choose appropriate pins for:
// - LED output (must support PWM)
// - Potentiometer input (must support ADC - GPIO26-29)
// Define appropriate PWM wrap value for brightness control

// Timing definitions (in milliseconds)
#define DOT_DURATION 200
#define DASH_DURATION (DOT_DURATION * 3)
#define SYMBOL_SPACE (DOT_DURATION)
#define LETTER_SPACE (DOT_DURATION * 3)
#define WORD_SPACE (DOT_DURATION * 7)
#define MAX_INPUT 100
#define LED 15

uint16_t pot_result = 0;
unsigned long time = 0;
char morse_char = ' ';

// Morse code lookup table (DO NOT MODIFY)
const char *MORSE_CODE[] = {
    ".-",   // A
    "-...", // B
    "-.-.", // C
    "-..",  // D
    ".",    // E
    "..-.", // F
    "--.",  // G
    "....", // H
    "..",   // I
    ".---", // J
    "-.-",  // K
    ".-..", // L
    "--",   // M
    "-.",   // N
    "---",  // O
    ".--.", // P
    "--.-", // Q
    ".-.",  // R
    "...",  // S
    "-",    // T
    "..-",  // U
    "...-", // V
    ".--",  // W
    "-..-", // X
    "-.--", // Y
    "--.."  // Z
};

// Function declarations
void init_pwm(void);
void init_adc(void);
uint16_t read_brightness(void);
void set_led_brightness(uint16_t brightness);
void display_dot(void);
void display_dash(void);
void display_character(char c);
bool read_line(char *buffer, int max_length);
void clear_input_buffer(void);
uint16_t map_value(uint16_t value, uint16_t in_min, uint16_t in_max, 
                   uint16_t out_min, uint16_t out_max) {
    return (value - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

int main()
{
    stdio_init_all();
    sleep_ms(2000); // Wait for USB serial to initialize

    // TODO: Initialize hardware - implement init_pwm and init_adc functions
    init_pwm();
    init_adc();

    printf("Morse Code LED Controller\n");
    printf("Use potentiometer to control LED brightness\n\n");

    char input[MAX_INPUT];

    while (true)
    {
        printf("Enter text to convert to Morse code (max %d chars): ", MAX_INPUT - 1);

        if (read_line(input, MAX_INPUT))
        {
            printf("Converting: %s\n", input);

            for (int i = 0; input[i] != '\0'; i++)
            {
                char abc = (char)toupper(input[i]);
                display_character(abc);
                if (morse_char == '.') {
                    display_dot();
                    sleep_ms(LETTER_SPACE);
                } else if (morse_char == '-')
                {
                    display_dash();
                    sleep_ms(LETTER_SPACE);
                } else if (morse_char == ' ') {
                    sleep_ms(WORD_SPACE);
                }
                
            }

            printf("\nConversion complete!\n\n");
        }

        clear_input_buffer(); // Clear any remaining input
        sleep_ms(100);
    }

    return 0;
}

// TODO: Implement PWM initialization
// Configure LED pin for PWM output with appropriate frequency
void init_pwm(void)
{
    // Your implementation here:
    // 1. Choose appropriate GPIO pin for LED
    // 2. Set up GPIO for PWM
    // 3. Configure PWM slice with appropriate frequency
    // 4. Set appropriate wrap value for desired resolution
    // 5. Enable PWM

    gpio_init(LED);
    gpio_set_dir(LED, GPIO_OUT);
    gpio_set_function(LED, GPIO_FUNC_PWM);

    uint slice_num = pwm_gpio_to_slice_num(LED);
    pwm_config config = pwm_get_default_config();
    pwm_config_set_wrap(&config, 1000);
    pwm_config_set_clkdiv(&config, 125);

    pwm_init(slice_num, &config, true);
}

// TODO: Implement ADC initialization
// Configure ADC for potentiometer reading
void init_adc(void)
{
    // Your implementation here:
    // 1. Choose appropriate ADC input pin (GPIO26-29)
    // 2. Initialize ADC hardware
    // 3. Set up GPIO for ADC
    // 4. Select correct ADC input channel

    adc_init();
    adc_gpio_init(26);  
    adc_select_input(0);
}

// TODO: Implement potentiometer reading
// Read ADC value and scale it to PWM range
uint16_t read_brightness(void)
{
    // Your implementation here:
    // 1. Read ADC value
    // 2. Scale from ADC range (0-4095) to your chosen PWM range
    // 3. Return scaled value

    pot_result = adc_read();

    pot_result = map_value(pot_result, 0, 4095, 0, 255);    
    return 0;
}

// TODO: Implement LED brightness control
// Set LED brightness using PWM
void set_led_brightness(uint16_t brightness)
{
    // Your implementation here:
    // Set PWM duty cycle based on brightness value

    pwm_set_gpio_level(LED, pot_result);    
}

// TODO: Implement Morse code dot
void display_dot(void)
{
    // Your implementation here:
    // 1. Read current brightness
    // 2. Turn LED on with current brightness
    // 3. Wait for dot duration
    // 4. Turn LED off
    // 5. Wait for symbol space

    set_led_brightness(read_brightness());

    time = to_ms_since_boot(get_absolute_time());
    if (to_ms_since_boot(get_absolute_time()) - time <= DOT_DURATION)
    pwm_set_gpio_level(LED, 0);  
}

// TODO: Implement Morse code dash
void display_dash(void)
{
    // Your implementation here:
    // 1. Read current brightness
    // 2. Turn LED on with current brightness
    // 3. Wait for dash duration
    // 4. Turn LED off
    // 5. Wait for symbol space

    set_led_brightness(read_brightness());

    time = to_ms_since_boot(get_absolute_time());
    if (to_ms_since_boot(get_absolute_time()) - time <= DASH_DURATION)
    pwm_set_gpio_level(LED, 0); 
}

// TODO: Implement character display function
// Handle uppercase letters, lowercase letters, and spaces
void display_character(char c)
{
    // Your implementation here:
    // 1. Handle spaces (use WORD_SPACE)
    // 2. Convert lowercase letters to uppercase
    // 3. Verify character is a valid letter (A-Z)
    // 4. Get morse code pattern from MORSE_CODE array
    const char *pattern = MORSE_CODE[c - 'A'];
    printf("%s ", pattern); // Sample code to print morse code pattern
    // 5. Display pattern using display_dot and display_dash
    // 6. Add appropriate letter space after each character

    morse_char = pattern;
}

// Function to clear input buffer (DO NOT MODIFY)
void clear_input_buffer(void)
{
    while (getchar_timeout_us(0) != PICO_ERROR_TIMEOUT)
    {
        // Clear any remaining characters
    }
}

// Function to read input (DO NOT MODIFY)
bool read_line(char *buffer, int max_length)
{
    int index = 0;
    int c;

    while (index < max_length - 1)
    {
        c = getchar_timeout_us(50000);

        if (c == PICO_ERROR_TIMEOUT)
        {
            if (index == 0)
                continue;
            break;
        }

        if (c == '\r' || c == '\n')
        {
            if (index > 0)
                break;
            continue;
        }

        putchar(c);
        buffer[index++] = (char)c;
    }

    buffer[index] = '\0';
    putchar('\n');
    return index > 0;
}