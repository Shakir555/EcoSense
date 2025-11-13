#ifndef ESP_WIFI_SERVER_H
#define ESP_WIFI_SERVER_H

#ifdef __cplusplus
extern "C" {
#endif

// ===== Includes =====
#include "esp_err.h"
#include "esp_http_server.h"

// ===== WiFi and HTTP Function Prototypes =====

// Initialize ESP32 WiFi Station
void wifi_init_sta(void);

// Start the HTTP Web Server
httpd_handle_t webserver_init(void);

// (Optional) Expose tags if you want to use same logging style
extern const char *WIFI_TAG;
extern const char *HTTP_TAG;

#ifdef __cplusplus
}
#endif

#endif // ESP_SERVER_H
