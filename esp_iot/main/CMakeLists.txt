idf_component_register(
    SRCS "station_example_main.c"
    PRIV_REQUIRES esp_wifi nvs_flash esp_event esp_netif esp_http_server dht driver
    INCLUDE_DIRS "."
)