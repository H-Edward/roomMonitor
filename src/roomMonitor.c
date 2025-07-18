// Author: Edward Hawkes
// Credits: lib/*/credits.txt

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "pico/multicore.h"

#include "user_variables.h"

#include "../lib/scd41/scd4x_i2c.h"
#include "../lib/scd41/sensirion_common.h"
#include "../lib/scd41/sensirion_i2c_hal.h"
#include "../lib/tm1637/tm1637.h"


// raw data from the sensor
uint16_t co2_meaningful = 0; // ppm is provided by the sensor and is therefore 'meaningful'
int32_t temperature = 0;
int32_t humidity = 0;

// Variables which store a modified value of the sensor data ^
// lcd & print friendly
int32_t temperature_meaningful = 0;
int32_t humidity_meaningful = 0;

void make_data_meaningful(int32_t *temperature, int32_t *humidity, int32_t *temperature_meaningful, int32_t *humidity_meaningful);

void core1_main() { // core 1 will handle the display general i/o logic
    tm1637_init(CLK_TM1637, DIO_TM1637); // Use GPIO 2 for CLK and GPIO 3 for DIO
    tm1637_set_brightness(4);
    tm1637_clear_display();
    
    // For checking whether its time to update the data on the display

    int prev_co2 = -1, prev_temp = -1, prev_humidity = -1;

    // by default, the display will be turned off
    uint8_t current_display = 3; // 0 = temperature, 1 = humidity, 2 = co2, 3 = powered off
    bool display_on = false;

    // used for debouncing the buttons
    uint64_t last_button_press_time = 0;


    // Init buttons
    gpio_init(TEMP_BUTTON);
    gpio_set_dir(TEMP_BUTTON, GPIO_IN);
    gpio_pull_up(TEMP_BUTTON);

    gpio_init(HUM_BUTTON);
    gpio_set_dir(HUM_BUTTON, GPIO_IN);
    gpio_pull_up(HUM_BUTTON);

    gpio_init(CO2_BUTTON);
    gpio_set_dir(CO2_BUTTON, GPIO_IN);
    gpio_pull_up(CO2_BUTTON);

    gpio_init(PWR_BUTTON);
    gpio_set_dir(PWR_BUTTON, GPIO_IN);
    gpio_pull_up(PWR_BUTTON);

    gpio_init(CO2_LED);
    gpio_set_dir(CO2_LED, GPIO_OUT);


    uint64_t last_update_time = to_ms_since_boot(get_absolute_time()); // needs to be 64 bit to prevent overflow

    while (true) {
        sleep_ms(50); // to prevent the core from hogging the CPU and to prevent the display from flickering when pressing a button
        uint64_t now = to_ms_since_boot(get_absolute_time());

        if (co2_meaningful > WARNNG_CO2_THRESHOLD) { // if the CO2 ppm is above the threshold, turn on the LED
            gpio_put(CO2_LED, 1);
        } else {
            gpio_put(CO2_LED, 0);
        }


        if (now - last_button_press_time > DEBOUNCE_DELAY) {
            if (gpio_get(TEMP_BUTTON) == 0) {
                current_display = 0;
                last_button_press_time = now;
                // make sure prev is not the same as the current value, since the display will not update
                prev_temp = -1;
            } else if (gpio_get(HUM_BUTTON) == 0) {
                current_display = 1;
                last_button_press_time = now;

                prev_humidity = -1;
            } else if (gpio_get(CO2_BUTTON) == 0) {
                current_display = 2;
                last_button_press_time = now;
                prev_co2 = -1;
            } else if (gpio_get(PWR_BUTTON) == 0) {
                current_display = 3;
                tm1637_clear_display();
                last_button_press_time = now;
            }
        }

        if (current_display == 0 && prev_temp != temperature_meaningful) {
            prev_temp = temperature_meaningful;
            // printf("Temperature: %d °C\n", temperature_meaningful);
            tm1637_display_number_without_leading_zero(temperature_meaningful);

        } else if (current_display == 1 && prev_humidity != humidity_meaningful) {
            prev_humidity = humidity_meaningful;
            // printf("Humidity: %d RH%%\n", humidity_meaningful);
            tm1637_display_number_without_leading_zero(humidity_meaningful);

        } else if (current_display == 2 && prev_co2 != co2_meaningful) {
            prev_co2 = co2_meaningful;
            // printf("CO2: %d ppm\n", co2_meaningful);
            tm1637_display_number_without_leading_zero(co2_meaningful);
        }
    }
}


int main() { // core 0 will handle the sensor logic and signaling core 1 to show ppm warnings (led)
    stdio_init_all();

    // Init SCD41
    sensirion_i2c_hal_init();
    scd4x_wake_up();
    scd4x_stop_periodic_measurement();
    scd4x_reinit();
    scd4x_start_periodic_measurement();

    // Spin up core1 

    multicore_launch_core1(core1_main);

    // Allow some time for the sensor to start up and produce reliable data
    sleep_ms(5000);
    while (true) {
            int16_t error = scd4x_read_measurement(&co2_meaningful, &temperature, &humidity);
            make_data_meaningful(&temperature, &humidity, &temperature_meaningful, &humidity_meaningful);

            sleep_ms(5000);
        }

}

void make_data_meaningful(int32_t *temperature, int32_t *humidity, int32_t *temperature_meaningful, int32_t *humidity_meaningful) {
    // co2 is already ppm and does not need to be modified

    *temperature_meaningful = *temperature / 1000;

    *humidity_meaningful = *humidity / 1000;
    return;    
}


