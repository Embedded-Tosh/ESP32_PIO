#include "main.h"
#include "DHT_Async.h"
#include "DHT.h"

/* Uncomment according to your sensortype. */
// #define DHT_SENSOR_TYPE DHT_TYPE_11
// #define DHT_SENSOR_TYPE DHT_TYPE_21
#define DHT_SENSOR_TYPE DHT_TYPE_22

#define MeasureTime 1000ul

static const int DHT_SENSOR_PIN = 4;
DHT_Async dht_sensor(DHT_SENSOR_PIN, DHT_SENSOR_TYPE);

#define DHTTYPE DHT22

DHT dht(DHT_SENSOR_PIN, DHTTYPE);

float convertCtoF(float c) { return c * 1.8 + 32; }
float convertFtoC(float f) { return (f - 32) * 0.55555; }

float computeHeatIndex(float temperature, float percentHumidity,
                       bool isFahrenheit)
{
    float hi;

    if (!isFahrenheit)
        temperature = convertCtoF(temperature);

    hi = 0.5 * (temperature + 61.0 + ((temperature - 68.0) * 1.2) +
                (percentHumidity * 0.094));

    if (hi > 79)
    {
        hi = -42.379 + 2.04901523 * temperature + 10.14333127 * percentHumidity +
             -0.22475541 * temperature * percentHumidity +
             -0.00683783 * pow(temperature, 2) +
             -0.05481717 * pow(percentHumidity, 2) +
             0.00122874 * pow(temperature, 2) * percentHumidity +
             0.00085282 * temperature * pow(percentHumidity, 2) +
             -0.00000199 * pow(temperature, 2) * pow(percentHumidity, 2);

        if ((percentHumidity < 13) && (temperature >= 80.0) &&
            (temperature <= 112.0))
            hi -= ((13.0 - percentHumidity) * 0.25) *
                  sqrt((17.0 - abs(temperature - 95.0)) * 0.05882);

        else if ((percentHumidity > 85.0) && (temperature >= 80.0) &&
                 (temperature <= 87.0))
            hi += ((percentHumidity - 85.0) * 0.1) * ((87.0 - temperature) * 0.2);
    }

    return isFahrenheit ? hi : convertFtoC(hi);
}

/*
 * Poll for a measurement, keeping the state machine alive.  Returns
 * true if a measurement is available.
 */
bool measure_environment(float *temperature, float *humidity, float *heatIdx)
{
    static unsigned long measurement_timestamp = millis();

    /* Measure once every four seconds. */
    if (millis() - measurement_timestamp > MeasureTime)
    {
        if (dht_sensor.measure(temperature, humidity))
        {
            measurement_timestamp = millis();
            *heatIdx = computeHeatIndex(*temperature, *humidity, false);
            return (true);
        }
    }
    return (false);
}

void DHT_Init(void)
{
    dht.begin();
}

bool DHT_MeasureData(float *temp, float *humi, float *heatIdx)
{
    static unsigned long measurement_timestampDHT = millis();

    /* Measure once every four seconds. */
    if (millis() - measurement_timestampDHT > MeasureTime)
    {
        // Reading temperature or humidity takes about 250 milliseconds!
        // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
        float h = dht.readHumidity();
        // Read temperature as Celsius (the default)
        float t = dht.readTemperature();

        // Check if any reads failed and exit early (to try again).
        if (isnan(h) || isnan(t))
        {
            *temp = 0;
            *humi = 0;
            *heatIdx = 0;
            return false;
        }
        else
        {
            *temp = t;
            *humi = h;
        }
        // Compute heat index in Celsius (isFahreheit = false)
        float hic = dht.computeHeatIndex(t, h, false);
        *heatIdx = hic;
    }
    return true;
}