idf_component_register(
    SRCS 
        "esp_main.c"
        "esp_dht.c"
        "esp_server_wifi.c"
        "esp_rainSensor.c"
        
    PRIV_REQUIRES 
        esp_wifi
        nvs_flash
        esp_event
        esp_netif
        esp_http_server
        dht
        driver
        esp_adc

    INCLUDE_DIRS 
        "."
        "header"
)
