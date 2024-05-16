#include "main.h"
#include "SendData.h"

// Root CA Certificate
const char *rootCACertificate = R"EOF(
-----BEGIN CERTIFICATE-----
MIIFVzCCAz+gAwIBAgINAgPlk28xsBNJiGuiFzANBgkqhkiG9w0BAQwFADBHMQsw
CQYDVQQGEwJVUzEiMCAGA1UEChMZR29vZ2xlIFRydXN0IFNlcnZpY2VzIExMQzEU
MBIGA1UEAxMLR1RTIFJvb3QgUjEwHhcNMTYwNjIyMDAwMDAwWhcNMzYwNjIyMDAw
MDAwWjBHMQswCQYDVQQGEwJVUzEiMCAGA1UEChMZR29vZ2xlIFRydXN0IFNlcnZp
Y2VzIExMQzEUMBIGA1UEAxMLR1RTIFJvb3QgUjEwggIiMA0GCSqGSIb3DQEBAQUA
A4ICDwAwggIKAoICAQC2EQKLHuOhd5s73L+UPreVp0A8of2C+X0yBoJx9vaMf/vo
27xqLpeXo4xL+Sv2sfnOhB2x+cWX3u+58qPpvBKJXqeqUqv4IyfLpLGcY9vXmX7w
Cl7raKb0xlpHDU0QM+NOsROjyBhsS+z8CZDfnWQpJSMHobTSPS5g4M/SCYe7zUjw
TcLCeoiKu7rPWRnWr4+wB7CeMfGCwcDfLqZtbBkOtdh+JhpFAz2weaSUKK0Pfybl
qAj+lug8aJRT7oM6iCsVlgmy4HqMLnXWnOunVmSPlk9orj2XwoSPwLxAwAtcvfaH
szVsrBhQf4TgTM2S0yDpM7xSma8ytSmzJSq0SPly4cpk9+aCEI3oncKKiPo4Zor8
Y/kB+Xj9e1x3+naH+uzfsQ55lVe0vSbv1gHR6xYKu44LtcXFilWr06zqkUspzBmk
MiVOKvFlRNACzqrOSbTqn3yDsEB750Orp2yjj32JgfpMpf/VjsPOS+C12LOORc92
wO1AK/1TD7Cn1TsNsYqiA94xrcx36m97PtbfkSIS5r762DL8EGMUUXLeXdYWk70p
aDPvOmbsB4om3xPXV2V4J95eSRQAogB/mqghtqmxlbCluQ0WEdrHbEg8QOB+DVrN
VjzRlwW5y0vtOUucxD/SVRNuJLDWcfr0wbrM7Rv1/oFB2ACYPTrIrnqYNxgFlQID
AQABo0IwQDAOBgNVHQ8BAf8EBAMCAYYwDwYDVR0TAQH/BAUwAwEB/zAdBgNVHQ4E
FgQU5K8rJnEaK0gnhS9SZizv8IkTcT4wDQYJKoZIhvcNAQEMBQADggIBAJ+qQibb
C5u+/x6Wki4+omVKapi6Ist9wTrYggoGxval3sBOh2Z5ofmmWJyq+bXmYOfg6LEe
QkEzCzc9zolwFcq1JKjPa7XSQCGYzyI0zzvFIoTgxQ6KfF2I5DUkzps+GlQebtuy
h6f88/qBVRRiClmpIgUxPoLW7ttXNLwzldMXG+gnoot7TiYaelpkttGsN/H9oPM4
7HLwEXWdyzRSjeZ2axfG34arJ45JK3VmgRAhpuo+9K4l/3wV3s6MJT/KYnAK9y8J
ZgfIPxz88NtFMN9iiMG1D53Dn0reWVlHxYciNuaCp+0KueIHoI17eko8cdLiA6Ef
MgfdG+RCzgwARWGAtQsgWSl4vflVy2PFPEz0tv/bal8xa5meLMFrUKTX5hgUvYU/
Z6tGn6D/Qqc6f1zLXbBwHSs09dR2CQzreExZBfMzQsNhFRAbd03OIozUhfJFfbdT
6u9AWpQKXCBfTkBdYiJ23//OYb2MI3jSNwLgjt7RETeJ9r/tSQdirpLsQBqvFAnZ
0E6yove+7u7Y/9waLd64NnHi/Hm3lCXRSHNboTXns5lndcEZOitHTtNCjv0xyBZm
2tIMPNuzjsmhDYAPexZ3FL//2wmUspO8IFgV6dtxQ/PeEMMA3KgqlbbC1j+Qa3bb
bP6MvPJwNQzcmRk13NfIRmPVNnGuV/u3gm3c
-----END CERTIFICATE-----
)EOF";

const char *serverName = "https://script.google.com/macros/s/AKfycbxUSI8JCdB2GdLZfurnpGKGOOOGtbMEeUm4mxKgJl-Hml9e1JDD1h4ZqdPZA-CG3Bo0Uw/exec";

void sendDataToServerPOST(float temp, float humidity)
{
  // Create an HTTPClient object
  HTTPClient https;

  // Start the HTTPS connection
  if (https.begin(serverName, rootCACertificate))
  {
    // Add a header for sending POST data in JSON format
    https.addHeader("Content-Type", "application/json");

    // Use ArduinoJson to build the JSON object
    JsonDocument doc; // Adjust size as needed
    doc["temp"] = temp;
    doc["humidity"] = humidity;
    String httpRequestData;
    serializeJson(doc, httpRequestData); // Serialize the JSON document to a String

    // Send the POST request
    int httpResponseCode = https.POST(httpRequestData);

    // Handle redirections (HTTP status code 302 or 307)
    if (httpResponseCode == HTTP_CODE_FOUND)
    {
      String newLocation = https.getLocation();
      if (newLocation.length() > 0)
      {
        https.end();                    // Close the current HTTPS connection
        https.begin(newLocation);       // Open the new URL
        httpResponseCode = https.GET(); // Attempt to GET the new location
      }
    }

    // Process response
    if (httpResponseCode > 0)
    {
      String response = https.getString();
      Serial.println(httpResponseCode);
      Serial.println(response);
    }
    else
    {
      Serial.print("Error on sending POST: ");
      Serial.println(httpResponseCode);
    }

    // End the HTTPS session
    https.end();
  }
  else
  {
    Serial.println("HTTPS Connection failed");
  }
}

void sendDataToServerGET(float temp, float humidity)
{
  // Create an HTTPClient object
  HTTPClient https;

  String serverPath = String(serverName) + "?temp=" + temp + "&humi=" + humidity;
  // Start the HTTPS connection
  if (https.begin(serverPath, rootCACertificate))
  {
    // Add a header for sending GET data in JSON format
    https.addHeader("Content-Type", "application/x-www-form-urlencoded");

    // Send the POST request
    int httpResponseCode = https.GET();

    // Handle redirections (HTTP status code 302 or 307)
    if (httpResponseCode == HTTP_CODE_FOUND)
    {
      String newLocation = https.getLocation();
      if (newLocation.length() > 0)
      {
        https.end();                    // Close the current HTTPS connection
        https.begin(newLocation);       // Open the new URL
        httpResponseCode = https.GET(); // Attempt to GET the new location
      }
    }

    // Process response
    if (httpResponseCode > 0)
    {
      String response = https.getString();
      Serial.println(httpResponseCode);
      Serial.println(response);
    }
    else
    {
      Serial.print("Error on sending GET: ");
      Serial.println(httpResponseCode);
    }

    // End the HTTPS session
    https.end();
  }
  else
  {
    Serial.println("HTTPS Connection failed");
  }
}

void sendDataToServer(float temperature, float humidity, bool sendAsJson)
{
  if (sendAsJson)
  {
    sendDataToServerPOST(temperature, humidity);
  }
  else
  {
    sendDataToServerGET(temperature, humidity);
  }
}

String getContentType(String filename)
{
  if (filename.endsWith(".html") || filename.endsWith(".htm"))
    return "text/html";
  else if (filename.endsWith(".txt"))
    return "text/plain";
  else if (filename.endsWith(".css"))
    return "text/css";
  else if (filename.endsWith(".js"))
    return "application/javascript";
  else if (filename.endsWith(".png"))
    return "image/png";
  else if (filename.endsWith(".gif"))
    return "image/gif";
  else if (filename.endsWith(".jpg") || filename.endsWith(".jpeg"))
    return "image/jpeg";
  else if (filename.endsWith(".ico"))
    return "image/x-icon";
  else if (filename.endsWith(".xml"))
    return "text/xml";
  else if (filename.endsWith(".pdf"))
    return "application/pdf";
  else if (filename.endsWith(".zip"))
    return "application/zip";
  else if (filename.endsWith(".gz"))
    return "application/x-gzip";
  else if (filename.endsWith(".c"))
    return "text/x-c";
  else if (filename.endsWith(".h"))
    return "text/x-h";
  else if (filename.endsWith(".o") || filename.endsWith(".bin") || filename.endsWith(".scvd") || filename.endsWith(".uvproj.saved_uv4") || filename.endsWith(".uvopt.bak"))
    return "application/octet-stream";
  return "text/plain"; // Default MIME type
}

int sendFileOverHTTP(File file)
{
  if (!file)
  {
    Serial.println("File is not open.");
    return -1; // Return error code indicating file is not open
  }

  HTTPClient https;
  https.begin(serverName, rootCACertificate); // Initialize HTTPClient with the URL
  String contentType = getContentType(file.name());
  https.addHeader("Content-Type", contentType); // Set the content type dynamically

  Serial.print("Sending file: " + String(file.name()) + " ");
  int httpResponseCode = https.sendRequest("POST", &file, file.size()); // Send the file

  // Handle redirections (HTTP status code 302 or 307)
  if (httpResponseCode == HTTP_CODE_FOUND)
  {
    String newLocation = https.getLocation();
    if (newLocation.length() > 0)
    {
      https.end();                    // Close the current HTTPS connection
      https.begin(newLocation);       // Open the new URL
      httpResponseCode = https.GET(); // Attempt to GET the new location
    }
  }

  // Process response
  if (httpResponseCode > 0)
  {
    String response = https.getString();
    Serial.print(httpResponseCode);
    Serial.print(" ");
    Serial.print(response);
  }
  else
  {
    Serial.print("Error on sending GET: ");
    Serial.print(httpResponseCode);
  }
  https.end(); // Close the HTTP connection

  return httpResponseCode; // Return HTTP response code or error code
}
