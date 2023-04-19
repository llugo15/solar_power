#include <stdio.h>
#include <string.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_netif.h"
#include "esp_wifi.h"
#include "freertos/event_groups.h"

const static char * TAG = "WIFI";
static EventGroupHandle_t Wifi_events;
static const int CONNECTED_GOT_IP = BIT0;
static const int DISCONNECTED = BIT1;
esp_netif_t *esp_netif;

void event_handler(void *event_handler_arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    switch (event_id)
    {
    // case SYSTEM_EVENT_STA_START:
    case WIFI_EVENT_STA_START:
        ESP_LOGI(TAG, "connecting...");
        esp_wifi_connect();
        break;
    // case SYSTEM_EVENT_STA_CONNECTED:
    case WIFI_EVENT_STA_CONNECTED:
        ESP_LOGI(TAG, "connected");
        break;

    // case SYSTEM_EVENT_STA_DISCONNECTED:
    case WIFI_EVENT_STA_DISCONNECTED:
        ESP_LOGI(TAG, "disconnected");
        xEventGroupSetBits(Wifi_events, DISCONNECTED);
        break;
    case IP_EVENT_STA_GOT_IP:
        ESP_LOGI(TAG, "GOT_IP");
        xEventGroupSetBits(Wifi_events, CONNECTED_GOT_IP);
        break;

    default:
        break;
    }
}

void wifi_init(void){

    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    wifi_init_config_t wifi_init_config = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&wifi_init_config));
    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, event_handler, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, event_handler, NULL));
    ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));

}

esp_err_t wifi_connect_sta(const char* ssid, const char* pass, int timeout)
{
    Wifi_events = xEventGroupCreate();

    esp_netif = esp_netif_create_default_wifi_sta();

    wifi_config_t wifi_config;
    memset(&wifi_config, 0 , sizeof(wifi_config_t));
    strncpy((char *)wifi_config.sta.ssid, ssid, sizeof(wifi_config.sta.ssid) -1);
        
    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config);
    esp_wifi_start();
    
    EventBits_t result = xEventGroupWaitBits(Wifi_events, CONNECTED_GOT_IP | DISCONNECTED, pdTRUE, pdFALSE, pdMS_TO_TICKS(timeout));
    if (result == CONNECTED_GOT_IP)
        return ESP_OK;
    return ESP_FAIL;
}

void wifi_disconnect(void){
    ESP_LOGI(TAG, "**********DISCONNECTING*********");
    esp_wifi_disconnect();
    esp_wifi_stop();
    ESP_LOGI(TAG, "***********DISCONNECTING COMPLETE*********");
}