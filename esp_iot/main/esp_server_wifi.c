// Libraries
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_http_server.h"
#include "nvs_flash.h"
#include "lwip/err.h"
#include "lwip/sys.h"
#include "lwip/ip4_addr.h"

#include "header/esp_dht.h"

// WiFi Configuration
#define EXAMPLE_ESP_WIFI_SSID      CONFIG_ESP_WIFI_SSID
#define EXAMPLE_ESP_WIFI_PASS      CONFIG_ESP_WIFI_PASSWORD
#define EXAMPLE_ESP_MAXIMUM_RETRY  CONFIG_ESP_MAXIMUM_RETRY

// WiFi Globals
#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT      BIT1

#define WIFI_TAG "ESP WIFI SERVER"
#define HTTP_TAG "ESP HTTP SERVER"

static EventGroupHandle_t s_wifi_event_group;
static int s_retry_num = 0;

// WiFi Event Handler
static void event_handler(void *arg, esp_event_base_t event_base,
                          int32_t event_id, void *event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START)
    {
        esp_wifi_connect();
    }
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED)
    {
        if (s_retry_num < EXAMPLE_ESP_MAXIMUM_RETRY)
        {
            esp_wifi_connect();
            s_retry_num++;
            ESP_LOGI(WIFI_TAG, "Retrying WiFi connection...");
        }
        else
        {
            xEventGroupSetBits(s_wifi_event_group, WIFI_FAIL_BIT);
        }
        ESP_LOGI(WIFI_TAG, "Connection failed.");
    }
    else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP)
    {
        ip_event_got_ip_t *event = (ip_event_got_ip_t *) event_data;
        ESP_LOGI(WIFI_TAG, "Got IP: " IPSTR, IP2STR(&event->ip_info.ip));
        s_retry_num = 0;
        xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
    }
}

// ============================================================================
//                    WiFi Init STA + Static IP (Correct)
// ============================================================================

void wifi_init_sta(void)
{
    s_wifi_event_group = xEventGroupCreate();
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();

    // WiFi Init
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler, NULL));

    wifi_config_t wifi_config = {
        .sta = {
            .ssid = EXAMPLE_ESP_WIFI_SSID,
            .password = EXAMPLE_ESP_WIFI_PASS
        },
    };

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_LOGI(WIFI_TAG, "wifi_init_sta finished.");

    // Wait For Connection
    EventBits_t bits = xEventGroupWaitBits(
        s_wifi_event_group,
        WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
        pdFALSE,
        pdFALSE,
        portMAX_DELAY
    );

    if (bits & WIFI_CONNECTED_BIT)
    {
        ESP_LOGI(WIFI_TAG, "Connected to SSID:%s", EXAMPLE_ESP_WIFI_SSID);
    }
    else if (bits & WIFI_FAIL_BIT)
    {
        ESP_LOGI(WIFI_TAG, "Failed to connect to SSID:%s", EXAMPLE_ESP_WIFI_SSID);
    }
}

// ============================================================================
//                               HTTP HANDLERS
// ============================================================================

// /hello
static esp_err_t string_get_handler(httpd_req_t *req)
{
    const char *resp_str = "Hi from ESP32!";
    
    httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
    httpd_resp_send(req, resp_str, HTTPD_RESP_USE_STRLEN);

    ESP_LOGI(HTTP_TAG, "Sent /hello response");
    return ESP_OK;
}

// /data (DHT11 sensor)
static esp_err_t dht_get_handler(httpd_req_t *req)
{
    char dht_json[128];

    httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
    httpd_resp_set_type(req, "application/json");

    snprintf(dht_json, sizeof(dht_json),
        "{\"temperature\": %.1f, \"humidity\": %.1f}",
        curTemp, curHumi);

    httpd_resp_send(req, dht_json, HTTPD_RESP_USE_STRLEN);

    ESP_LOGI(HTTP_TAG, "Sent DHT JSON: %s", dht_json);
    return ESP_OK;
}

// URI Routing
static const httpd_uri_t string_uri = {
    .uri = "/hello",
    .method = HTTP_GET,
    .handler = string_get_handler
};

static const httpd_uri_t dht_uri = {
    .uri = "/data",
    .method = HTTP_GET,
    .handler = dht_get_handler
};

// HTTP Server Init
httpd_handle_t webserver_init(void)
{
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    httpd_handle_t server = NULL;

    ESP_LOGI(HTTP_TAG, "Starting HTTP server on port %d", config.server_port);

    if (httpd_start(&server, &config) == ESP_OK)
    {
        httpd_register_uri_handler(server, &string_uri);
        httpd_register_uri_handler(server, &dht_uri);
        return server;
    }

    ESP_LOGE(HTTP_TAG, "Failed to start HTTP server");
    return NULL;
}
