#ifndef ESP_DHT_H
#define ESP_DHT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

extern float curTemp;
extern float curHumi;

void dht_task(void *pvParam);

#ifdef __cplusplus
}
#endif

#endif
