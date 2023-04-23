#include <stdio.h>
#include <string.h>
#include "connect.h"
#include "esp_http_client.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "cJSON.h"

static const char *TAG = "REST";
extern const uint8_t cert[] asm("_binary_cert_cer_start");

typedef struct chunk_payload_t
{
    uint8_t *buffer;
    int buffer_index;
} chunk_payload_t;

esp_err_t on_client_data(esp_http_client_event_t *evt)
{
    switch (evt->event_id)
    {
    case HTTP_EVENT_ON_DATA:
    {
        chunk_payload_t *chunk_payload = evt->user_data;
        chunk_payload->buffer = realloc(chunk_payload->buffer, chunk_payload->buffer_index + evt->data_len + 1);
        memcpy(&chunk_payload->buffer[chunk_payload->buffer_index], (uint8_t *)evt->data, evt->data_len);
        chunk_payload->buffer_index = chunk_payload->buffer_index + evt->data_len;
        chunk_payload->buffer[chunk_payload->buffer_index] = 0;
    }
    break;

    default:
        break;
    }
    return ESP_OK;
}

char *solarPanelData(double sp_vol, double sp_curr) { // make sure to put in the float values of voltageData and currentData
 
    char voltageString[10];
    char currentString[10];

    sprintf(voltageString, "%f", sp_vol);
    sprintf(currentString, "%f", sp_curr);
    
    // making JSON object, reandom id (parent), voltage (child node), current (child node)
    cJSON *jason_payload = cJSON_CreateObject(); 
    cJSON_AddStringToObject(jason_payload, "voltage", voltageString);
    cJSON_AddStringToObject(jason_payload, "current", currentString);

    char *payload_body = cJSON_Print(jason_payload);
    printf("body: %s\n", payload_body);
    cJSON_Delete(jason_payload);
    return payload_body;
}

// bool connectESP(string MAC) {
// }

void post_function(double sp_vol, double sp_curr, double bat_vol) {
    /* eventually going to be passing data through here, additionally need to remember to keep passing data every minute or so */
    chunk_payload_t chunk_payload = {0};

    esp_http_client_config_t config = {
        .url = "https://test-1ecd1-default-rtdb.firebaseio.com",
        .event_handler = on_client_data,
        .user_data = &chunk_payload,
        .disable_auto_redirect = true,
        .cert_pem = (char *)cert};
    esp_http_client_handle_t client = esp_http_client_init(&config);

    // POST
    char *payload_body = solarPanelData(sp_vol, sp_curr);
    esp_http_client_set_url(client, "https://test-1ecd1-default-rtdb.firebaseio.com/newuser@gmail/solarpanel.json");
    esp_http_client_set_method(client, HTTP_METHOD_POST);
    esp_http_client_set_header(client, "Content-Type", "application/json");
    esp_http_client_set_post_field(client, payload_body, strlen(payload_body));
    esp_err_t err = esp_http_client_perform(client);
    if (err == ESP_OK) {
        ESP_LOGI(TAG, "HTTP GET status = %d\n", esp_http_client_get_status_code(client));
    } else {
        ESP_LOGE(TAG, "HTTP POST request failed: %s", esp_err_to_name(err));
    }


    // dynamically writing batery voltage into a JSON object for backend 
    char str[4]; // creating a empty string for the battery voltage 
    
    // converting voltage from int to *char and initializing the body and end to create JSON object
    sprintf(str, "%f", bat_vol);
    char *body = "{\"Battery\":";
    char *end = "}";

    // creating a holder *char and concatenating the JSON object
    char* name_with_extension;
    name_with_extension = malloc(strlen(body) + strlen(end) + strlen(str) + 1); /* make space for the new string (should check the return value ...) */
    strcpy(name_with_extension, body); /* copy name into the new var */
    strcat(name_with_extension, str); /* add the extension */
    strcat(name_with_extension, end); /* add the extension */

    printf("Output: %s\n", name_with_extension);

    // PATCH
    esp_http_client_set_url(client, "https://test-1ecd1-default-rtdb.firebaseio.com/newuser@gmail.json");
    esp_http_client_set_method(client, HTTP_METHOD_PATCH);
    esp_http_client_set_post_field(client, name_with_extension, strlen(name_with_extension));
    err = esp_http_client_perform(client);
    if (err == ESP_OK) {
        ESP_LOGI(TAG, "HTTP GET status = %d\n", esp_http_client_get_status_code(client));
    } else {
        ESP_LOGE(TAG, "HTTP PATCH request failed: %s", esp_err_to_name(err));
    }

    // this has to happen at the end to free up the data
    free(name_with_extension);
    if (chunk_payload.buffer != NULL)
    {
        free(chunk_payload.buffer);
    }
    if (payload_body != NULL)
    {
        free(payload_body);
    }
    esp_http_client_cleanup(client);
}