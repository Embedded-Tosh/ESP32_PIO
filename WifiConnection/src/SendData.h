#ifndef SENDDATA_H
#define SENDDATA_H

// Function Declarations
void connectToWiFi();
void sendDataToServer(float temp, float humidity, bool isjson);
int sendFileOverHTTP(File file);

#endif // SENDDATA_H