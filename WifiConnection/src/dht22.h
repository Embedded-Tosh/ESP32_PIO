#ifndef DHT22_H
#define DHT22_H

void DHT_Init(void);
bool measure_environment(float *temperature, float *humidity, float *heatIdx);
bool DHT_MeasureData(float *temp, float *humi, float *heatIdx);

#endif // DHT22_H
