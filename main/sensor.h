#ifndef _SENSOR_H
#define _SENSOR_H

typedef struct{
    double max_voltage;
    adc1_channel_t sensor_gpio;
}Sensor;

void Sensor_ctor(Sensor * const me, double max_voltage, adc1_channel_t sensor_gpio);
double sensor_adc1(Sensor* const me);
double sensor_adc2(Sensor* const me);
double sensor_current(Sensor* const me, double gain);
void converter_out(Sensor* const me, double updated_voltage);
//void switch_control( gpio_num_t dc_pin, gpio_num_t solar_power_pin);

#endif