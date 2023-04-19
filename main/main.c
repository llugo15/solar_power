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

#define DC_PIN 18
#define SP_PIN 5

static const char* TAG = "wifi_connect";

void app_main(void)
{   

    ESP_ERROR_CHECK(nvs_flash_init());
    wifi_init();
    ESP_ERROR_CHECK(wifi_connect_sta("TAMU_IoT", "Blah", 100000));
    
    vTaskDelay(2000/ portTICK_PERIOD_MS);
    printf("WIFI was initiated............\n\n");
    
    // call swtich control function and make sure to call post function within switch control
    // while (true) {
    //     vTaskDelay(2000/ portTICK_PERIOD_MS);
    //     switch_control(DC_PIN, SP_PIN);
    // }

    switch_control(DC_PIN, SP_PIN);

    // post_function();
    wifi_disconnect();

}


