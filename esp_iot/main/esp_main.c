// Libraries
#include "header/esp_dht.h"
#include "header/esp_server_wifi.h"

void app_main() {
    wifi_init_sta();
    webserver_init();
    xTaskCreate(dht_task, "dht_task", 2048, NULL, 5, NULL);
}
