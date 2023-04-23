#include <stdio.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/dac.h"
#include "driver/adc.h"
#include "sensor.h"
#include "send_data.h"
#include "switch_control.h"

#define DC_PIN 18
#define SP_PIN 19

void Switch_ctor(Switch_control * const me)
{
    me -> sp_vol_num = 0.0;
    me -> dc_vol_num = 0.0;
    me -> battery_vol_num = 0.0;
    me -> sp_curr_num = 0.0;
    me -> dc_curr_num = 0.0;
    me -> battery_curr_num = 0.0;
    
}

void post_switch(Switch_control * const me)
{
        post_function(me -> sp_vol_num, me -> sp_curr_num, me ->battery_vol_num);

}

void switch_control(Switch_control * const me, gpio_num_t dc_pin, gpio_num_t solar_power_pin)
{
    Sensor solar_panel_vol, dc_vol, battery_vol;
    Sensor solar_panel_curr, dc_curr, battery_curr;
    double previous_power = 0.0;
    int count = 0;

    //Configuring the gpio pin outputs which control the switches.
    //The default setting is high, to close the switches. Allowing prower through
    gpio_pad_select_gpio(dc_pin);
    gpio_pad_select_gpio(solar_power_pin);
    gpio_set_direction(dc_pin, GPIO_MODE_OUTPUT);
    gpio_set_direction(solar_power_pin, GPIO_MODE_OUTPUT);
    gpio_set_level(dc_pin, 1);
    gpio_set_level(solar_power_pin, 1);

    //Voltage Sensors/Dividers input
    Sensor_ctor(&solar_panel_vol, 51.25, ADC1_CHANNEL_3);
    Sensor_ctor(&dc_vol, 12.6, ADC1_CHANNEL_0);
    Sensor_ctor(&battery_vol, 12.6, ADC2_CHANNEL_4); //ADC2_CHANNEL_9

    //Voltage Current/Current Resistors input
    Sensor_ctor(&solar_panel_curr, 0.0865, ADC2_CHANNEL_5);
    Sensor_ctor(&dc_curr, 1, ADC2_CHANNEL_6);
    Sensor_ctor(&battery_curr, 0.0865, ADC2_CHANNEL_7);

    while(true){
        
        me -> sp_vol_num = sensor_adc1(&solar_panel_vol);
        me -> dc_vol_num = sensor_adc1(&dc_vol);
        me -> battery_vol_num = sensor_adc1(&battery_vol);

        me -> sp_curr_num = sensor_current(&solar_panel_curr, 0.0173);
        me -> dc_curr_num = sensor_current(&dc_curr, 0.0503);
        me -> battery_curr_num = sensor_current(&battery_curr, 0.0173);
        //if battery is greater than 12.6V
        printf("Battery Voltage: %f\n",me -> battery_vol_num);
        if((me -> battery_vol_num) >= 11.80){
            //Solar panel switch is closed, battery switch is opened.
            gpio_set_level(dc_pin, 0);
            printf("Status: Battery is charged, switch opened from battery.\n Load is supplied.\n");
        } 
        else{
            //if solar power battery is greater than battery voltage 
            if((me -> sp_vol_num)> (me -> battery_vol_num)){

                int sp_power = (me -> sp_vol_num) * (me -> sp_curr_num);
                
                if(sp_power >= previous_power){
                    //The converter out voltage value will need to be changed to
                    //increase the dc voltage
                    converter_out(&dc_vol, 12.5);
                    previous_power = sp_power;
                    printf("Status: power at %d \n", sp_power);
                }
                else{
                    //The converter out value will need to keep the current voltage 
                    converter_out(&dc_vol, 0.0);
                    printf("Status: power at %d \n", sp_power);
                }
            }
            else{
                //Solar panel switch is opened, 
                //the load will be supplied by the battery
                gpio_set_level(solar_power_pin, 0);
            }
            if((me -> battery_vol_num) < 10.5){
                gpio_set_level(dc_pin, 0);
                printf("Status: Battery is low, there is not enough power to supply load.\n System has been disabled\n");
            }
            
        }
        if(count == 60){
            post_function(me -> sp_vol_num, me -> sp_curr_num, me ->battery_vol_num);
            count = 0;
        }
        else{
            count = count +1; 
            printf("Count: %d\n", count);
        }
    }
    
}
void main_switch_control(){
    Switch_control solar_switch;
    Switch_ctor(&solar_switch);
    switch_control(&solar_switch, DC_PIN, SP_PIN);
}