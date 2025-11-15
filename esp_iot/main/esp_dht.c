// Libraries
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "dht.h"
#include "esp_log.h"


// DHT Pin
#define DHT_PIN 25

// DHT Tag
#define DHT_TAG "DHT-11"

float curTemp = 0.0f;
float curHumi = 0.0f;

// DHT 11 Sensor Task
void dht_task(void *pvParam)
{
    float temperature = 0;
    float humidity = 0;

    while(1)
    {
        if (dht_read_float_data(DHT_TYPE_DHT11, DHT_PIN, &humidity, &temperature) == ESP_OK)
        {
            curTemp = temperature;
            curHumi = humidity;
            ESP_LOGI(DHT_TAG, "Temp: %.1f°C  Humidity: %.1f%%", temperature, humidity);
        }
        else
        {
            ESP_LOGI(DHT_TAG, "Failed to Read Data");
        }
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}