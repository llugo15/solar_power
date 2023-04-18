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
        // ESP_LOGI(TAG, "Length=%d", evt->data_len);
        // printf("%.*s\n", evt->data_len, (char *)evt->data);
        chunk_payload_t *chunk_payload = evt->user_data;
        chunk_payload->buffer = realloc(chunk_payload->buffer, chunk_payload->buffer_index + evt->data_len + 1);
        memcpy(&chunk_payload->buffer[chunk_payload->buffer_index], (uint8_t *)evt->data, evt->data_len);
        chunk_payload->buffer_index = chunk_payload->buffer_index + evt->data_len;
        chunk_payload->buffer[chunk_payload->buffer_index] = 0;

        //printf("buffer******** %s\n",chunk_payload->buffer);
    }
    break;

    default:
        break;
    }
    return ESP_OK;
}

char *create_firebase_body()
{
    cJSON *jason_payload = cJSON_CreateObject();
    cJSON *voltage = cJSON_CreateArray();
    cJSON_AddItemToObject(jason_payload, "voltage", voltage);

    cJSON *voltage_0 = cJSON_CreateObject();
    cJSON_AddItemToArray(voltage, voltage_0);

    cJSON *to = cJSON_CreateArray();
    cJSON_AddItemToObject(voltage_0, "to", to);

    cJSON *to_0 = cJSON_CreateObject();
    cJSON_AddStringToObject(to_0, "email", "llugo@tamu.edu");
    cJSON_AddStringToObject(to_0, "name", "Lauren");
    cJSON_AddItemToArray(to, to_0);

    cJSON_AddStringToObject(voltage_0, "subject", "Hello, World!");

    cJSON *content = cJSON_CreateArray();
    cJSON_AddItemToObject(jason_payload, "content", content);

    cJSON *content_0 = cJSON_CreateObject();
    cJSON_AddStringToObject(content_0, "type", "text/html");
    cJSON_AddStringToObject(content_0, "value", "<h1>Heya!</h1>");
    cJSON_AddItemToArray(content, content_0);

    cJSON *from = cJSON_CreateObject();
    cJSON_AddItemToObject(jason_payload, "from", from);
    cJSON_AddStringToObject(from, "email", "llugo@tamu.edu");
    cJSON_AddStringToObject(from, "name", "Lauren");

    cJSON *reply_to = cJSON_CreateObject();
    cJSON_AddItemToObject(jason_payload, "reply_to", reply_to);
    cJSON_AddStringToObject(reply_to, "email", "llugo@tamu.edu");
    cJSON_AddStringToObject(reply_to, "name", "Lauren");

    char *payload_body = cJSON_Print(jason_payload);
    printf("body: %s\n", payload_body);
    cJSON_Delete(jason_payload);
    return payload_body;
}

void post_function()
{
    chunk_payload_t chunk_payload = {0};

    esp_http_client_config_t esp_http_client_config = {
        .url = "https://test-1ecd1-default-rtdb.firebaseio.com/preferences.json",
        .method = HTTP_METHOD_POST,
        .event_handler = on_client_data,
        .user_data = &chunk_payload,
        .cert_pem = (char *)cert};
    esp_http_client_handle_t client = esp_http_client_init(&esp_http_client_config);
    esp_http_client_set_header(client, "Content-Type", "application/json");
    //esp_http_client_set_header(client, "Authorization", "Bearer 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");
    char *payload_body = create_firebase_body();
    esp_http_client_set_post_field(client, payload_body, strlen(payload_body));

    esp_err_t err = esp_http_client_perform(client);
    if (err == ESP_OK)
    {
        ESP_LOGI(TAG, "HTTP GET status = %d\n", esp_http_client_get_status_code(client));
    }
    else
    {
        ESP_LOGE(TAG, "HTTP GET request failed: %s", esp_err_to_name(err));
    }
    if (chunk_payload.buffer != NULL)
    {
        free(chunk_payload.buffer);
    }
    if (payload_body != NULL)
    {
        free(payload_body);
    }
    esp_http_client_cleanup(client);
    wifi_disconnect();
}