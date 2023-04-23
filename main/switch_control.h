#ifndef _SWITCH_CONTROL_H
#define _SWITCH_CONTROL_H

typedef struct{
    double sp_vol_num;
    double dc_vol_num;
    double battery_vol_num;
    double sp_curr_num;
    double dc_curr_num;
    double battery_curr_num;
}Switch_control;

void Switch_ctor(Switch_control * const me);
void post(Switch_control * const me);
void switch_control(Switch_control * const me, gpio_num_t dc_pin, gpio_num_t solar_power_pin);
void main_switch_control();

#endif