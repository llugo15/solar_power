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
#include "esp_http_client.h"
#include "esp_log.h"
#include "cJSON.h"

#define DC_PIN 18
#define SP_PIN 5

static const char* TAG = "wifi_connect";

extern const uint8_t certificate_pem[] asm("_binary_certificate_pem_start");

char *create_file()
{
    cJSON *jason_payload = cJSON_CreateObject();
    cJSON *personalizations = cJSON_CreateArray();
    cJSON_AddItemToObject(jason_payload, "personalizations", personalizations);

    cJSON *personalization_0 = cJSON_CreateObject();
    cJSON_AddItemToArray(personalizations, personalization_0);

    cJSON *to = cJSON_CreateArray();
    cJSON_AddItemToObject(personalization_0, "to", to);

    cJSON *to_0 = cJSON_CreateObject();
    cJSON_AddStringToObject(to_0, "email", "info@learnesp32.com");
    cJSON_AddStringToObject(to_0, "name", "Mair");
    cJSON_AddItemToArray(to, to_0);

    cJSON_AddStringToObject(personalization_0, "subject", "Hello, World!");

    cJSON *content = cJSON_CreateArray();
    cJSON_AddItemToObject(jason_payload, "content", content);

    cJSON *content_0 = cJSON_CreateObject();
    cJSON_AddStringToObject(content_0, "type", "text/html");
    cJSON_AddStringToObject(content_0, "value", "<h1>Heya!</h1>");
    cJSON_AddItemToArray(content, content_0);

    cJSON *from = cJSON_CreateObject();
    cJSON_AddItemToObject(jason_payload, "from", from);
    cJSON_AddStringToObject(from, "email", "info@learnesp32.com");
    cJSON_AddStringToObject(from, "name", "Mair");

    cJSON *reply_to = cJSON_CreateObject();
    cJSON_AddItemToObject(jason_payload, "reply_to", reply_to);
    cJSON_AddStringToObject(reply_to, "email", "info@learnesp32.com");
    cJSON_AddStringToObject(reply_to, "name", "Mair");

    char *payload_body = cJSON_Print(jason_payload);
    printf("body: %s\n", payload_body);
    cJSON_Delete(jason_payload);
    return payload_body;
}

esp_err_t client_event_get_handler(esp_http_client_event_handle_t evt)
{
    switch(evt -> event_id)
    {
        case HTTP_EVENT_ON_DATA: 
            printf("HTTP_EVENT_ON_DATA: %.*s\n", evt -> data_len, (char *)evt->data);
            break;

        default:
        break;
    }
    return ESP_OK;
    
}

void post_function()
{
    esp_http_client_config_t config_post = {
        .url = "https://test-1ecd1-default-rtdb.firebaseio.com/preferences.json",
        .method = HTTP_METHOD_POST,
        .cert_pem = (const char *)certificate_pem,
        .event_handler = client_event_get_handler};

    esp_http_client_event_handle_t client = esp_http_client_init(&config_post);
    esp_http_client_set_header(client, "Content-Type", "application/json");
    //esp_http_client_set_header(client, "Authorization", "AIzaSyDWNPZ93LIPdr12eY5ZbfoDWgYSEdfVL08");

    char *post_data = create_file(); // FIXME (what is being sent has to be jsons) 
    esp_http_client_set_post_field(client, post_data, strlen(&config_post));

    esp_err_t err = esp_http_client_perform(client);
    if (err == ESP_OK)
    {
        ESP_LOGI(TAG, "HTTP GET status = %d\n", esp_http_client_get_status_code(client));
    }
    else
    {
        ESP_LOGE(TAG, "HTTP GET request failed: %s", esp_err_to_name(err));
    }
    if (post_data != NULL)
    {
        free(post_data);
    }
    esp_http_client_cleanup(client);
}

void app_main(void)
{   

    ESP_ERROR_CHECK(nvs_flash_init());
    wifi_init();
    ESP_ERROR_CHECK(wifi_connect_sta("TAMU_IoT", "Blah", 100000));

    vTaskDelay(2000/ portTICK_PERIOD_MS);
    printf("WIFI was initiated............\n\n");

    post_function();
    wifi_disconnect();
    
    Sensor solar_panel_vol, dc_vol, battery_vol;
    double previous_power = 0.0;

    /*Configuring the gpio pin outputs which control the switches.
    The default setting is high, to close the switches.
    gpio_pad_select_gpio(DC_PIN);
    gpio_pad_select_gpio(SP_PIN);
    gpio_set_direction(DC_PIN, GPIO_MODE_OUTPUT);
    gpio_set_direction(SP_PIN, GPIO_MODE_OUTPUT);
    gpio_set_level(DC_PIN, 1);
    gpio_set_level(SP_PIN, 1);

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
            gpio_set_level(DC_PIN, 0);
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
                gpio_set_level(SP_PIN, 0);
            }
            if(battery_vol_num < 10.5){
                gpio_set_level(DC_PIN, 0);
                printf("Status: Battery is low, there is not enough power to supply load.\n System has been disabled");
            }
        }
    }*/
}


