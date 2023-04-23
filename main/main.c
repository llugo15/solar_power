#include <stdio.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/dac.h"
#include "driver/adc.h"
#include "sensor.h"
#include "connect.h"
#include "nvs_flash.h"
#include "string.h"
#include "send_data.h"
#include "switch_control.h"

static const char* TAG = "wifi_connect";

void app_main(void)
{   

    ESP_ERROR_CHECK(nvs_flash_init());
    wifi_init();
    ESP_ERROR_CHECK(wifi_connect_sta("TAMU_IoT", "Holleman2200!", 100000));

    vTaskDelay(2000/ portTICK_PERIOD_MS);
    printf("WIFI was initiated............\n\n");
    post_function(12.0,4.0,5.0);
    //main_switch_control();
    
    wifi_disconnect();
}


