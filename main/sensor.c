#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/dac.h"
#include "driver/adc.h"
#include "sensor.h"
#include "send_data.h"


void Sensor_ctor(Sensor * const me, double max_voltage, adc1_channel_t sensor_gpio)
{
    me->max_voltage = max_voltage;
    me->sensor_gpio = sensor_gpio;
}

double sensor_adc1(Sensor* const me)
{
    adc1_config_width(ADC_WIDTH_12Bit);
    adc1_config_channel_atten(me->sensor_gpio, ADC_ATTEN_DB_11);
    while(true){
        double val= adc1_get_raw(me->sensor_gpio);
        val = (val*(me->max_voltage))/4096;
        vTaskDelay(500/portTICK_PERIOD_MS);
        return val;
    }
}

double sensor_adc2(Sensor* const me)
{
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc2_config_channel_atten(me->sensor_gpio, ADC_ATTEN_11db);
    while(true){
        int val;
        adc2_get_raw(me->sensor_gpio,ADC_WIDTH_12Bit, &val);
        val = (val*(me->max_voltage))/4095;
        //printf("Voltage value: %f\n", val);
        vTaskDelay(100/portTICK_PERIOD_MS);
        return val;
    }
}
double sensor_current(Sensor* const me, double gain)
{
    adc1_config_width(ADC_WIDTH_12Bit);
    adc1_config_channel_atten(me->sensor_gpio, ADC_ATTEN_DB_11);
    while(true){
        double val= adc1_get_raw(me->sensor_gpio);
        val = (val*(me->max_voltage))/4096;
        double current = val/gain;
        vTaskDelay(500/portTICK_PERIOD_MS);
        return current;
    }
}

void converter_out(Sensor* const me, double updated_voltage)
{
    dac_output_enable(DAC_CHANNEL_1);
    int dac_out = (int)ceil((updated_voltage/3.3)*255);
    dac_output_voltage(DAC_CHANNEL_1, dac_out);
    printf("Dac_output: %f\n", updated_voltage);
}

