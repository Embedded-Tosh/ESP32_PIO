
#include "main.h"
#include <WiFi.h>

const char *ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 16200;
const int daylightOffset_sec = 3600;

volatile bool isConnected = false; // Global flag to denote WiFi connection status
volatile bool TimeReady = false;

bool isTimeReady(void)
{
    return TimeReady;
}

bool WifiConnected(void)
{
    return isConnected;
}

void onWiFiReady(WiFiEvent_t event, WiFiEventInfo_t info)
{
    // Serial.println("WiFi interface ready");
    isConnected = false;
    // updateLED();
}

void onWiFiStart(WiFiEvent_t event, WiFiEventInfo_t info)
{
    // Serial.println("WiFi client started");
    isConnected = false;
    // updateLED();
}

void onWiFiStop(WiFiEvent_t event, WiFiEventInfo_t info)
{
    // Serial.println("WiFi client stopped");
    isConnected = false;
    // updateLED();
}

void onWiFiDisconnected(WiFiEvent_t event, WiFiEventInfo_t info)
{
    isConnected = false;
    // updateLED();
    //  Serial.print("Disconnected from WiFi. Reason: ");
    //  Serial.println(info.wifi_sta_disconnected.reason);
    //  Attempt to reconnect
    WiFi.disconnect(true);
    WiFi.begin(SSID, PASSWORD);
}

void onWiFiConnected(WiFiEvent_t event, WiFiEventInfo_t info)
{
    // isConnected = true;
    // updateLED();
    //  Serial.println("Connected to WiFi network");
}

void onWiFiGotIP(WiFiEvent_t event, WiFiEventInfo_t info)
{
    // Serial.print("Obtained IP address: ");
    // Serial.println(IPAddress(info.got_ip.ip_info.ip.addr));
    if (!TimeReady)
    {
        configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
        TimeReady = true;
    }
    isConnected = true;
}

void onWiFiLostIP(WiFiEvent_t event, WiFiEventInfo_t info)
{
    // Serial.println("Lost IP address and IP address is reset to 0");
    isConnected = false;
    // updateLED();
}

void Wifi_Init(void)
{
    WiFi.disconnect(true);
    delay(1000);
    WiFi.onEvent(onWiFiReady, WiFiEvent_t::ARDUINO_EVENT_WIFI_READY);
    WiFi.onEvent(onWiFiStart, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_START);
    WiFi.onEvent(onWiFiStop, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_STOP);
    WiFi.onEvent(onWiFiConnected, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_CONNECTED);
    WiFi.onEvent(onWiFiDisconnected, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_DISCONNECTED);
    WiFi.onEvent(onWiFiLostIP, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_LOST_IP);
    WiFi.onEvent(onWiFiGotIP, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_GOT_IP);
    WiFi.begin(SSID, PASSWORD);
    // Serial.println("Connecting to WiFi...");
}
