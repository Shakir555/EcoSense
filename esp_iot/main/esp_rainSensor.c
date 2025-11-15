#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"

#include "esp_adc/adc_oneshot.h"
#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_cali_scheme.h"

#define TAG "RAIN_SENSOR"

// Pins
#define RAIN_DO_GPIO    26
#define RAIN_AO_GPIO    ADC_CHANNEL_6   // GPIO 34

// Globals
int do_state = 0;
int ao_value = 0;

// ADC handles
adc_oneshot_unit_handle_t adc1_handle;
adc_cali_handle_t adc_cali_handle = NULL;
bool adc_calibration_enabled = false;

void rainSensorConfig(void)
{
    ESP_LOGI(TAG, "Initializing Rain Sensor (NEW ADC API)...");

    // -----------------------
    // Configure DO pin
    // -----------------------
    gpio_config_t io_conf = {
        .mode = GPIO_MODE_INPUT,
        .pin_bit_mask = (1ULL << RAIN_DO_GPIO),
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    gpio_config(&io_conf);

    // -----------------------
    // ADC ONE-SHOT Setup
    // -----------------------
    adc_oneshot_unit_init_cfg_t init_config1 = {
        .unit_id = ADC_UNIT_1,
        .ulp_mode = ADC_ULP_MODE_DISABLE,
    };
    adc_oneshot_new_unit(&init_config1, &adc1_handle);

    adc_oneshot_chan_cfg_t config = {
        .bitwidth = ADC_BITWIDTH_12,
        .atten = ADC_ATTEN_DB_12,   // Full voltage range (0–3.3V)
    };
    adc_oneshot_config_channel(adc1_handle, RAIN_AO_GPIO, &config);

    // -----------------------
    // ADC Calibration
    // -----------------------
    adc_cali_line_fitting_config_t cali_config = {
        .unit_id = ADC_UNIT_1,
        .atten = ADC_ATTEN_DB_12,
        .bitwidth = ADC_BITWIDTH_12,
    };

    if (adc_cali_create_scheme_line_fitting(&cali_config, &adc_cali_handle) == ESP_OK) {
        adc_calibration_enabled = true;
        ESP_LOGI(TAG, "ADC Calibration: ENABLED");
    } else {
        adc_calibration_enabled = false;
        ESP_LOGW(TAG, "ADC Calibration: NOT SUPPORTED, using raw values");
    }

    ESP_LOGI(TAG, "Rain Sensor Config Complete");
}

void rainSensorTask(void *param)
{
    int raw_value = 0;

    while (1)
    {
        // Read DO
        do_state = gpio_get_level(RAIN_DO_GPIO);

        // Read AO (ADC)
        adc_oneshot_read(adc1_handle, RAIN_AO_GPIO, &raw_value);

        if (adc_calibration_enabled) {
            adc_cali_raw_to_voltage(adc_cali_handle, raw_value, &ao_value);
        } else {
            ao_value = raw_value;  // fallback raw
        }

        // Print digital state
        if (do_state == 0) {
            ESP_LOGW(TAG, "🌧️ Rain Detected!");
        } else {
            ESP_LOGI(TAG, "☀️ No Rain");
        }

        // Print analog wetness
        if (adc_calibration_enabled)
            ESP_LOGI(TAG, "AO Voltage (mV): %d", ao_value);
        else
            ESP_LOGI(TAG, "AO Raw (0–4095): %d", ao_value);

        vTaskDelay(pdMS_TO_TICKS(300));
    }
}
