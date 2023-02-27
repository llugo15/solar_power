#ifndef send_data_h
#define send_data_h

#include "esp_err.h"
#include "esp_http_client.h"

esp_err_t on_client_data(esp_http_client_event_t *evt);
char *create_firebase_body();
void post_function();

#endif