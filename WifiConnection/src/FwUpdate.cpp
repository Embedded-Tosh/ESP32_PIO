#include "main.h"
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <Update.h>

const char *rot = R"EOF(
-----BEGIN CERTIFICATE-----
MIIDjjCCAnagAwIBAgIQAzrx5qcRqaC7KGSxHQn65TANBgkqhkiG9w0BAQsFADBh
MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3
d3cuZGlnaWNlcnQuY29tMSAwHgYDVQQDExdEaWdpQ2VydCBHbG9iYWwgUm9vdCBH
MjAeFw0xMzA4MDExMjAwMDBaFw0zODAxMTUxMjAwMDBaMGExCzAJBgNVBAYTAlVT
MRUwEwYDVQQKEwxEaWdpQ2VydCBJbmMxGTAXBgNVBAsTEHd3dy5kaWdpY2VydC5j
b20xIDAeBgNVBAMTF0RpZ2lDZXJ0IEdsb2JhbCBSb290IEcyMIIBIjANBgkqhkiG
9w0BAQEFAAOCAQ8AMIIBCgKCAQEAuzfNNNx7a8myaJCtSnX/RrohCgiN9RlUyfuI
2/Ou8jqJkTx65qsGGmvPrC3oXgkkRLpimn7Wo6h+4FR1IAWsULecYxpsMNzaHxmx
1x7e/dfgy5SDN67sH0NO3Xss0r0upS/kqbitOtSZpLYl6ZtrAGCSYP9PIUkY92eQ
q2EGnI/yuum06ZIya7XzV+hdG82MHauVBJVJ8zUtluNJbd134/tJS7SsVQepj5Wz
tCO7TG1F8PapspUwtP1MVYwnSlcUfIKdzXOS0xZKBgyMUNGPHgm+F6HmIcr9g+UQ
vIOlCsRnKPZzFBQ9RnbDhxSJITRNrw9FDKZJobq7nMWxM4MphQIDAQABo0IwQDAP
BgNVHRMBAf8EBTADAQH/MA4GA1UdDwEB/wQEAwIBhjAdBgNVHQ4EFgQUTiJUIBiV
5uNu5g/6+rkS7QYXjzkwDQYJKoZIhvcNAQELBQADggEBAGBnKJRvDkhj6zHd6mcY
1Yl9PMWLSn/pvtsrF9+wX3N3KjITOYFnQoQj8kVnNeyIv/iPsGEMNKSuIEyExtv4
NeF22d+mQrvHRAiGfzZ0JFrabA0UWTW98kndth/Jsw1HKj2ZL7tcu7XUIOGZX1NG
Fdtom/DzMNU+MeKNhJ7jitralj41E6Vf8PlwUHBHQRFXGU7Aj64GxJUTFy8bJZ91
8rGOmaFvE7FBcf6IKshPECBV1/MUReXgRPTqh5Uykw7+U0b6LJ3/iyK5S9kJRaTe
pLiaWN0bfVKfjllDiIGknibVb63dDcY3fe0Dkhvld1927jyNxF1WW6LZZm6zNTfl
MrY=
-----END CERTIFICATE-----
)EOF";

const char *headerKeys[] = {"Last-Modified"};
const size_t headerKeysCount = sizeof(headerKeys) / sizeof(char *);

HTTPClient http;

void headerinit(void)
{
    http.collectHeaders(headerKeys, headerKeysCount);
}

void checkUpdate()
{
    if (WiFi.status() == WL_CONNECTED)
    {
        Serial.println("Checking update");

        String url = update_url;
        http.begin(url, rot);
        int httpCode = http.GET();
        if (httpCode > 0)
        { // Check for the returning code
            String response = http.getString();
            int headers = http.headers();
            String headerValue = http.header(headers - 1);
            Serial.println(headerValue);

            JsonDocument doc; // Adjust size as needed
            deserializeJson(doc, response);
            JsonObject obj = doc.as<JsonObject>();
            String version = obj["version"];
            String url_update = obj["url"];
            Serial.println(version);
            if (version.toDouble() > FIRMWARE_VERSION)
            {
                Serial.println("Update Available");
                Serial.println(url_update);
                if (!downloadAndPrepareUpdate(url_update))
                    Serial.println("Update Failed");
                else
                    Serial.println("Update Success");
            }
            else
            {
                Serial.println("No Update Available");
            }
        }
        else
        {
            Serial.println("Error on HTTP request");
        }
        http.end(); // Free the resources
    }
    else
    {
        Serial.println("Not connected to WiFi");
    }
}

bool downloadAndPrepareUpdate(String url_update)
{

    http.begin(url_update, rot);
    int httpCode = http.GET();

    if (httpCode == HTTP_CODE_OK)
    {
        int totalLen = http.getSize();
        if (totalLen > 0)
        {
            File updateFile = SPIFFS.open(filenamepath, FILE_WRITE);
            if (!updateFile)
            {
                Serial.println("Failed to open file for writing");
                http.end();
                return false;
            }

            WiFiClient *stream = http.getStreamPtr();
            uint8_t buff[1024] = {0};
            int downloaded = 0;
            float lastPercentage = 0.0;
            unsigned long lastTime = millis();

            while (http.connected() && (totalLen > 0 || totalLen == -1))
            {
                size_t size = stream->available();
                if (size)
                {
                    int c = stream->readBytes(buff, ((size > sizeof(buff)) ? sizeof(buff) : size));
                    updateFile.write(buff, c);
                    downloaded += c;
                    lastTime = millis(); // Reset the timeout timer
                    if (totalLen > 0)
                    {
                        totalLen -= c;
                    }
                    float currentPercentage = (downloaded * 100.0 / http.getSize());
                    if (fabs(currentPercentage - lastPercentage) >= 0.01)
                    {
                        lastPercentage = currentPercentage;
                        Serial.printf("Download progress: %.2f%%\n", lastPercentage);
                    }
                }
                else if (millis() - lastTime > 15000)
                {
                    Serial.println("Connection timed out");
                    updateFile.close();
                    http.end();
                    return false;
                }
            }
            updateFile.close();
            if (downloaded == http.getSize())
            {
                Serial.println("Download completed");
                http.end();
                return performUpdate();
            }
            else
            {
                Serial.println("Download failed or incomplete");
                http.end();
                return false;
            }
        }
        else
        {
            Serial.println("Invalid content length received");
            http.end();
            return false;
        }
    }
    else
    {
        Serial.printf("Failed to download file with error: %d\n", httpCode);
        http.end();
        return false;
    }
}

bool performUpdate()
{
    File updateFile = SPIFFS.open(filenamepath, FILE_READ);
    if (!updateFile)
    {
        Serial.println("Failed to open file for reading");
        return false;
    }

    size_t totalLength = updateFile.size();
    if (Update.begin(totalLength))
    {
        size_t written = Update.writeStream(updateFile);
        if (written == totalLength)
        {
            Serial.println("Written successfully");
            if (Update.end(true))
            {
                Serial.println("OTA done! Rebooting...");
                ESP.restart();
                return true; // Though we won't get here due to the restart
            }
            else
            {
                Serial.printf("Error Occurred. Error #: %u\n", Update.getError());
                updateFile.close();
                return false;
            }
        }
        else
        {
            Serial.printf("Error during writing flash. Written only %u bytes.\n", written);
            updateFile.close();
            return false;
        }
    }
    else
    {
        Serial.println("Not enough space to begin OTA");
        updateFile.close();
        return false;
    }
}
