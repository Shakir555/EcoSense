#ifndef ESP_RAINSENSOR_H
#define ESP_RAINSENSOR_H

#ifdef __cplusplus
extern "C" {
#endif

extern int do_state;
extern int ao_value;

void rainSensorConfig(void);
void rainSensorTask(void *pvParameters);

#ifdef __cplusplus
}
#endif

#endif
