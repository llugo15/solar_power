#include <stdio.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/dac.h"
#include "driver/adc.h"
#include "sensor.h"

void Sensor_ctor(Sensor * const me, double max_voltage, adc1_channel_t sensor_gpio)
{
    me->max_voltage = max_voltage;
    me->sensor_gpio = sensor_gpio;
}

int sensor_adc(Sensor* const me)
{
    adc1_config_width(ADC_WIDTH_12Bit);
    adc1_config_channel_atten(me->sensor_gpio, ADC_ATTEN_11db);
    while(true){
        int val= adc1_get_raw(me->sensor_gpio);
        val = (val*(me->max_voltage))/4096;
        vTaskDelay(5000/portTICK_PERIOD_MS);
        return val;
    }
}

void converter_out(Sensor* const me, double updated_voltage)
{
    dac_output_enable(DAC_CHANNEL_1);
    if(updated_voltage <= (me->max_voltage)){
            /*Will need to change the value to increase 
            by a specific voltage.*/
        dac_output_voltage(DAC_CHANNEL_1, 255);
    }
    else{
        dac_output_voltage(DAC_CHANNEL_1, 100);
    }
}

void switch_control(gpio_num_t dc_pin, gpio_num_t solar_power_pin)
{
    Sensor solar_panel_vol, dc_vol, battery_vol;
    double previous_power = 0.0;
    
    //Configuring the gpio pin outputs which control the switches.
    //The default setting is high, to close the switches.
    gpio_pad_select_gpio(dc_pin);
    gpio_pad_select_gpio(solar_power_pin);
    gpio_set_direction(dc_pin, GPIO_MODE_OUTPUT);
    gpio_set_direction(solar_power_pin, GPIO_MODE_OUTPUT);
    gpio_set_level(dc_pin, 1);
    gpio_set_level(solar_power_pin, 1);

    //Voltage Sensors/Dividers input
    Sensor_ctor(&solar_panel_vol, 51.25, ADC1_CHANNEL_0);
    Sensor_ctor(&dc_vol, 12.0, ADC1_CHANNEL_2);
    Sensor_ctor(&battery_vol, 12.6, ADC2_CHANNEL_5);

    //Voltage Current/Current Resistors input
    Sensor_ctor(&solar_panel_vol, 2.6, ADC1_CHANNEL_1);
    Sensor_ctor(&dc_vol, 2.6, ADC1_CHANNEL_3);
    Sensor_ctor(&battery_vol, 2.6, ADC2_CHANNEL_4);

    while(true){

        int sp_vol_num = sensor_adc(&solar_panel_vol);
        //int dc_vol_num = sensor_adc(&dc_vol);
        int battery_vol_num = sensor_adc(&battery_vol);

        int sp_curr_num = sensor_adc(&solar_panel_vol);
        //int dc_curr_num = sensor_adc(&dc_vol);
        //int battery_curr_num = sensor_adc(&battery_vol);
        
        //if battery is greater than 12.6V
        if(battery_vol_num >= 12.6){
            //Solar panel switch is closed, battery switch is opened.
            gpio_set_level(dc_pin, 0);
            printf("Status: Battery is charged, switch opened from battery.\n Load is supplied.");
        } 
        else{
            //if solar power battery is greater than battery voltage 
            if(sp_vol_num > battery_vol_num){

                int sp_power = sp_vol_num*sp_curr_num;
                
                if(sp_power >= previous_power){
                    //The converter out voltage value will need to be changed to
                    //increase the dc voltage
                    converter_out(&dc_vol, 12.5);
                    previous_power = sp_power;
                    printf("Status: power at %d", sp_power);
                }
                else{
                    //The converter out value will need to keep the current voltage 
                    converter_out(&dc_vol, 0.0);
                    printf("Status: power at %d", sp_power);
                }
            }
            else{
                //Solar panel switch is opened, 
                //the load will be supplied by the battery
                gpio_set_level(solar_power_pin, 0);
            }
            if(battery_vol_num < 10.5){
                gpio_set_level(dc_pin, 0);
                printf("Status: Battery is low, there is not enough power to supply load.\n System has been disabled");
            }
        }
    }
}