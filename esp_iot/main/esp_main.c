// Libraries
#include "header/esp_dht.h"
#include "header/esp_server_wifi.h"
#include "header/esp_rainSensor.h"

#include "nvs_flash.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define TAG "APP_MAIN"

void app_main(void)
{
    esp_err_t ret = nvs_flash_init();

    // Fix NVS initialization
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_LOGW(TAG, "NVS partition was truncated or version changed. Erasing...");
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
    ESP_LOGI(TAG, "NVS initialized successfully");

    // ==== WIFI START ====
    ESP_LOGI(TAG, "Starting WiFi...");
    wifi_init_sta();

    // ==== HTTP SERVER START ====
    ESP_LOGI(TAG, "Starting web server...");
    webserver_init();

    // Rain Sensor Config
    ESP_LOGI(TAG, "Configuring Rain Sensor..");
    rainSensorConfig();

    // ==== DHT TASK START ====
    ESP_LOGI(TAG, "Starting DHT11 task...");
    xTaskCreate(
        dht_task,       
        "dht_task",     
        4096,           
        NULL,
        4,
        NULL
    );

    ESP_LOGI(TAG, "Configuring Rain Sensor...");
    rainSensorConfig();

    ESP_LOGI(TAG, "Starting Rain Sensor Task...");
    xTaskCreate(rainSensorTask, "rain_task", 4096, NULL, 5, NULL);
}
