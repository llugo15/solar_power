#include <stdio.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/dac.h"
#include "driver/adc.h"
#include "sensor.h"
#include "send_data.h"

// setting the current max_voltage and sensor channel
void Sensor_ctor(Sensor * const sensor, double max_voltage, adc1_channel_t sensor_gpio)
{
    sensor->max_voltage = max_voltage;
    sensor->sensor_gpio = sensor_gpio;
}

// Takes in AC input and converting it to DC output for sensors
int sensor_adc(Sensor* const sensor)
{
    adc1_config_width(ADC_WIDTH_MAX);
    adc1_config_channel_atten(sensor->sensor_gpio, ADC_ATTEN_DB_6);
    // getting data from AC channel every 5 seconds and returning that value
    while(true){
        int val = adc1_get_raw(sensor->sensor_gpio);
        val = (val*(sensor->max_voltage))/4096;
        vTaskDelay(5000/portTICK_PERIOD_MS);
        return val;
    }
}

// setting the DC output 
void converter_out(Sensor* const sensor, double updated_voltage)
{   
    dac_output_enable(DAC_CHANNEL_1);
    if(updated_voltage <= (sensor->max_voltage)){
            /*Will need to change the value to increase 
            by a specific voltage.*/
        dac_output_voltage(DAC_CHANNEL_1, 255);
    }
    // decreases the dc voltage 
    else{
        dac_output_voltage(DAC_CHANNEL_1, 100);
    }
}

// this function controls the switches 
void switch_control(gpio_num_t dc_pin, gpio_num_t solar_power_pin)
{
    // initalizing difference sensors and keeping track of the past power
    Sensor solar_panel_vol, dc_vol, battery_vol;
    double previous_power = 0.0;
    
    /*Configuring the gpio pin outputs which control the switches.
    The default setting is high, to close the switches. */
    esp_rom_gpio_pad_select_gpio(dc_pin);
    esp_rom_gpio_pad_select_gpio(solar_power_pin);
    gpio_set_direction(dc_pin, GPIO_MODE_OUTPUT);
    gpio_set_direction(solar_power_pin, GPIO_MODE_OUTPUT);
    gpio_set_level(dc_pin, 1);
    gpio_set_level(solar_power_pin, 1);

    // Voltage Sensors/Dividers input
    Sensor_ctor(&solar_panel_vol, 51.25, ADC1_CHANNEL_0); 
    Sensor_ctor(&dc_vol, 12.0, ADC1_CHANNEL_3);
    Sensor_ctor(&battery_vol, 12.6, ADC2_CHANNEL_8);
    
    // Voltage Current/Current Resistors input
    Sensor_ctor(&solar_panel_vol, 2.6, ADC2_CHANNEL_9);
    Sensor_ctor(&dc_vol, 2.6, ADC1_CHANNEL_3);
    Sensor_ctor(&battery_vol, 2.6, ADC2_CHANNEL_4);

    while(true){

        int sp_vol_num = sensor_adc(&solar_panel_vol); // DC
        int battery_vol_num = sensor_adc(&battery_vol);
    
        int sp_curr_num = sensor_adc(&solar_panel_vol); // AC
        
        // if battery >= 12.6 -> fully charged
        if(battery_vol_num >= 12.6){
            // Solar panel switch is closed, battery switch is opened.
            gpio_set_level(dc_pin, 0);
            printf("Status: Battery is charged, switch opened from battery.\n Load is supplied.");
        } 
        else{
            //if solar power battery is greater than battery voltage 
            if(sp_vol_num > battery_vol_num){

                int sp_power = sp_vol_num*sp_curr_num;
                
                if(sp_power >= previous_power){
                    /* The converter out voltage value will need to be changed to
                    increase the dc voltage */
                    converter_out(&dc_vol, 12.5); 
                    previous_power = sp_power;
                    printf("Status: power at %d", sp_power);
                }
                else{
                    // The converter out value will need to keep the current voltage 
                    converter_out(&dc_vol, 0.0);
                    printf("Status: power at %d", sp_power);
                }
            }
            else{
                // Solar panel switch is opened, 
                // the load will be supplied by the battery
                gpio_set_level(solar_power_pin, 0);
            }
            if(battery_vol_num < 10.5){
                gpio_set_level(dc_pin, 0);
                printf("Status: Battery is low, there is not enough power to supply load.\n System has been disabled");
            }
        }
    }
}