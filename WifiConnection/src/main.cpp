// #include "main.h"

// const char *Bdate = __DATE__;
// const char *Btime = __TIME__;

// void InitInfo(void);

// void setup()
// {
//     Serial.begin(115200);
//     delay(1);
//     InitInfo();
//     Serial.println("Connecting to WiFi...");

//     WiFi.begin(SSID, PASSWORD);
//     while (WiFi.status() != WL_CONNECTED)
//     {
//         delay(500);
//         Serial.print(".");
//     }
//     Serial.println("\nConnected to WiFi");

//     if (!SPIFFS.begin(true))
//     {
//         Serial.println("An Error has occurred while mounting SPIFFS");
//         return;
//     }

// }

// void loop()
// {
//     // Empty loop
//     checkUpdate();
//     delay(5000);
//     float temperature;
//     float humidity;

//     /* Measure temperature and humidity.  If the functions returns
//        true, then a measurement is available. */
//     if (measure_environment(&temperature, &humidity))
//     {
//         Serial.print("T = ");
//         Serial.print(temperature, 1);
//         Serial.print(" deg. C, H = ");
//         Serial.print(humidity, 1);
//         Serial.println("%");
//     }
// }

// void InitInfo(void)
// {
//     Serial.println(F("---HTTP OTA--- "));
//     Serial.println(String(Btime) + " " + String(Bdate));
//     Serial.print("FW_Version ");
//     Serial.println(FIRMWARE_VERSION);
//     Serial.println(F(" INIT---"));
//     Serial.print(F("esp32 "));
//     Serial.println(ESP.getSdkVersion());
//     Serial.printf("arduino-esp32 v%d.%d.%d\n", int(ESP_ARDUINO_VERSION_MAJOR), int(ESP_ARDUINO_VERSION_MINOR), int(ESP_ARDUINO_VERSION_PATCH)); // availeable since v2.0.0

//     Serial.print(F("CPU:   "));
//     Serial.print(ESP.getChipModel());
//     Serial.print(F(" rev."));
//     Serial.print(ESP.getChipRevision());
//     Serial.print(F(", "));
//     Serial.print(ESP.getChipCores());
//     Serial.print(F(" core(s)"));
//     Serial.print(F(", "));
//     Serial.print(ESP.getCpuFreqMHz());
//     Serial.print(F("MHz."));
//     Serial.print(F("FLASH: "));
//     Serial.print((ESP.getFlashChipSize() / 1024) / 1024);
//     Serial.print(F("MB, Mode "));
//     Serial.print(ESP.getFlashChipMode());
//     switch (ESP.getFlashChipMode())
//     {
//     // missing: Octal modes
//     case FM_QIO:
//         Serial.print(F(" (QIO)"));
//         break;
//     case FM_QOUT:
//         Serial.print(F(" (QOUT)"));
//         break;
//     case FM_DIO:
//         Serial.print(F(" (DIO)"));
//         break;
//     case FM_DOUT:
//         Serial.print(F(" (DOUT)"));
//         break;
//     default:
//         break;
//     }
//     Serial.print(F(", speed "));
//     Serial.print(ESP.getFlashChipSpeed() / 1000000);
//     Serial.println(F("MHz."));
//     Serial.print(F("heap "));
//     Serial.println(ESP.getFreeHeap());
// }

// #include <Arduino.h>
// #include <WiFi.h>

// WiFi credentials and server settings
// const char* ssid = "YourAp";
// const char* password = "1122334466";

// void setup()
// {
//   Serial.begin(115200);
//   Wifi_Init();
// }

// void loop()
// {
//   float temp, humi;
//   if (WifiisConnected())
//   {
//     if (measure_environment(&temp, &humi))
//       sendDataToServer(temp, humi, false); // Simulated temperature and humidity
//   }
//   else
//   {
//     Serial.println("Wifi Not Connected");
//     delay(1000);
//   }
//   // delay(1000); // Post data every 10 seconds
// }

// Example function that uses the WiFi connection status to perform different tasks
// void checkWiFiAndPerformTask()
// {
//   if (WifiisConnected())
//   {
//     Serial.println("WiFi is connected, performing tasks...");
//     // Add tasks that require WiFi connection
//   }
//   else
//   {
//     Serial.println("WiFi is not connected, limited functionality.");
//     // Add tasks that can be performed without WiFi connection
//   }
// }
// void setup()
// {
//   Serial.begin(115200);
//   pinMode(LED_BUILTIN, OUTPUT);
//   Wifi_Init();
//   Serial.println("Connecting to WiFi...");
// }

// void loop()
// {
//   // Example function call that could behave differently based on WiFi status
//   checkWiFiAndPerformTask();
//   delay(1000);
// }

// #include "FS.h"
// #include "SD.h"
// #include "SPI.h"

// File root;
// String sPathName;
// uint64_t iVolumeSize;
// SdVolume volume;

// boolean PrintAccessData(char *cFileName, String sPath);
// void printDirectory(File dir, int numTabs);

// void setup()
// {
//   Serial.begin(115200);
//   if (!SD.begin())
//   {
//     Serial.println("Card Mount Failed");
//     return;
//   }
//   uint8_t cardType = SD.cardType();

//   if (cardType == CARD_NONE)
//   {
//     Serial.println("No SD card attached");
//     return;
//   }

//   Serial.print("SD Card Type: ");
//   if (cardType == CARD_MMC)
//   {
//     Serial.println("MMC");
//   }
//   else if (cardType == CARD_SD)
//   {
//     Serial.println("SDSC");
//   }
//   else if (cardType == CARD_SDHC)
//   {
//     Serial.println("SDHC");
//   }
//   else
//   {
//     Serial.println("UNKNOWN");
//   }

//   iVolumeSize = SD.cardSize() / (1024 * 1024);
//   Serial.printf("SD Card Size: %lluMB\n", iVolumeSize);

//   root = SD.open("/");

//   printDirectory(root, 0);
// }

// void loop()
// {
// }

// boolean PrintAccessData(char *cFileName, String sPath)
// {
//   byte bFileTypeLen = 0; // Length of the extension name to help align the size numbers later

//   // open the root directory
//   // if (!root.openRoot(&volume))
//   // {
//   //   Serial.print(" openRoot failed");
//   //   file.close();
//   //   root.close();
//   //   return false;
//   // }

//   // open a file
//   File file = SD.open(cFileName, FILE_READ);
//   if (!file)
//   { // Probably need to change &root to point to the next location / directory. Does not work with path/file
//     Serial.print(" file.open failed. Can't handle subfolder calls\t\t");
//     file.close();
//     root.close();
//     return false;
//   }

//   dir_t d;
//   if (!file.dirEntry(&d))
//   { // Test with a folder name?
//     Serial.println("file.dirEntry failed");
//     file.close();
//     root.close();
//     return false;
//   }

//   Serial.print("\t");
//   file.printFatDate(d.creationDate);
//   Serial.print(" ");
//   file.printFatTime(d.creationTime);
//   Serial.print("\t");
//   file.printFatDate(d.lastWriteDate);
//   Serial.print(" ");
//   file.printFatTime(d.lastWriteTime);
//   Serial.print("\t");
//   // Serial.print(GetFileExtension(cFileName, bFileTypeLen));

//   for (byte i = 0; i < (22 - bFileTypeLen); i++)
//   { // "Comma Separated Values" is the longest file name at 22 characters. Adjust if required.
//     Serial.print(F(" "));
//   }

//   file.close();
//   root.close();
// }

// void printDirectory(File dir, int numTabs)
// {
//   long iFileSize = 0;
//   long iSizeTally = 0;
//   int iFileCount = 0;
//   // Begin at the start of the directory
//   dir.rewindDirectory();

//   while (true)
//   {
//     File entry = dir.openNextFile();
//     if (!entry)
//     {
//       // no more files
//       // Serial.println("**nomorefiles**");
//       break;
//     }
//     for (uint8_t i = 0; i < numTabs; i++)
//     {
//       Serial.print('\t'); // we'll have a nice indentation
//     }
//     // Print the 8.3 name
//     const char *cFileName;    // Create a pointer char variable - for the FileName
//     cFileName = entry.name(); // Assign the first entry of entry.name() to the FileName pointer created
//     Serial.print(cFileName);
//     // Serial.print(entry.name());
//     // Recurse for directories, otherwise print the file size
//     if (entry.isDirectory())
//     {
//       Serial.println("/");
//       sPathName = entry.name();
//       printDirectory(entry, numTabs + 1);
//     }
//     else
//     {
//       // files have sizes, directories do not
//       // PrintAccessData(cFileName, sPathName);
//       // Serial.print("\t\t");
//       // Serial.println(entry.size(), DEC);
//       Serial.print(F("\t"));
//       if (entry.size() > 1024 * 1024)
//       { // > 1 MB
//         Serial.print(entry.size() / 1024.0 / 1024.0, 2);
//         Serial.println(F(" MB"));
//       }
//       else if (entry.size() > 1024)
//       { // > 1kB
//         if (entry.size() > 102400)
//         { // > 100 kb
//           Serial.print(entry.size() / 1024.0, 0);
//         }
//         else
//         {
//           Serial.print(entry.size() / 1024.0, 2);
//         }
//         Serial.println(F(" kB"));
//       }
//       else
//       { // bytes
//         Serial.print(entry.size(), DEC);
//         Serial.println(F(" bytes"));
//       }
//       // Files file complete blocks of 512 bytes even if smaller than 512
//       iFileSize = int(entry.size() / 512) * 512; // Number of whole blocks
//       if (entry.size() % 512 > 0)
//       { // Any partial blocks
//         iFileSize += 512;
//       }
//       iSizeTally += iFileSize;
//       iFileCount++;
//     }
//     entry.close();
//   }
//   Serial.print(F("  Estimated total disk usage: "));
//   if (iSizeTally > 1024 * 1024)
//   { // > 1 MB
//     Serial.print(iSizeTally / 1024.0 / 1024.0, 2);
//     Serial.print(F(" MB"));
//   }
//   else if (iSizeTally > 1024)
//   { // > 1 kB
//     if (iSizeTally > 102400)
//     { // > 100 kB
//       Serial.print(iSizeTally / 1024.0, 0);
//     }
//     else
//     {
//       Serial.print(iSizeTally / 1024.0, 2);
//     }
//     Serial.print(F(" kB"));
//   }
//   else
//   { // bytes
//     Serial.print(iSizeTally, 0);
//     Serial.print(F(" bytes"));
//   }
//   Serial.print(F(" ("));
//   Serial.print(float(iSizeTally) / iVolumeSize, 3);
//   Serial.print(F("%"));
//   Serial.print(F(")"));
//   Serial.print(F(" in "));
//   Serial.print(iFileCount);
//   Serial.println(F(" files"));
// }

// #include "FS.h"
// #include "SD.h"
// #include "SPI.h"
// #include <time.h>
// #include <WiFi.h>

// const char *ssid = "YourAp";
// const char *password = "1122334466";

// long timezone = 1;
// byte daysavetime = 1;
// File root;

// void listDir(fs::FS &fs, const char *dirname, uint8_t levels)
// {
//   Serial.printf("Listing directory: %s\n", dirname);

//   File root = fs.open(dirname);
//   if (!root)
//   {
//     Serial.println("Failed to open directory");
//     return;
//   }
//   if (!root.isDirectory())
//   {
//     Serial.println("Not a directory");
//     return;
//   }

//   File file = root.openNextFile();
//   while (file)
//   {
//     if (file.isDirectory())
//     {
//       Serial.print("  DIR : ");
//       Serial.print(file.name());
//       time_t t = file.getLastWrite();
//       struct tm *tmstruct = localtime(&t);
//       Serial.printf("  LAST WRITE: %d-%02d-%02d %02d:%02d:%02d\n", (tmstruct->tm_year) + 1900, (tmstruct->tm_mon) + 1, tmstruct->tm_mday, tmstruct->tm_hour, tmstruct->tm_min, tmstruct->tm_sec);
//       if (levels)
//       {
//         listDir(fs, file.path(), levels - 1);
//       }
//     }
//     else
//     {
//       Serial.print("  FILE: ");
//       Serial.print(file.name());
//       Serial.print("  SIZE: ");
//       Serial.print(file.size());
//       time_t t = file.getLastWrite();
//       struct tm *tmstruct = localtime(&t);
//       Serial.printf("  LAST WRITE: %d-%02d-%02d %02d:%02d:%02d\n", (tmstruct->tm_year) + 1900, (tmstruct->tm_mon) + 1, tmstruct->tm_mday, tmstruct->tm_hour, tmstruct->tm_min, tmstruct->tm_sec);
//     }
//     file = root.openNextFile();
//   }
// }

// void createDir(fs::FS &fs, const char *path)
// {
//   Serial.printf("Creating Dir: %s\n", path);
//   if (fs.mkdir(path))
//   {
//     Serial.println("Dir created");
//   }
//   else
//   {
//     Serial.println("mkdir failed");
//   }
// }

// void removeDir(fs::FS &fs, const char *path)
// {
//   Serial.printf("Removing Dir: %s\n", path);
//   if (fs.rmdir(path))
//   {
//     Serial.println("Dir removed");
//   }
//   else
//   {
//     Serial.println("rmdir failed");
//   }
// }

// void readFile(fs::FS &fs, const char *path)
// {
//   Serial.printf("Reading file: %s\n", path);

//   File file = fs.open(path);
//   if (!file)
//   {
//     Serial.println("Failed to open file for reading");
//     return;
//   }

//   Serial.print("Read from file: ");
//   while (file.available())
//   {
//     Serial.write(file.read());
//   }
//   file.close();
// }

// void writeFile(fs::FS &fs, const char *path, const char *message)
// {
//   Serial.printf("Writing file: %s\n", path);

//   File file = fs.open(path, FILE_WRITE);
//   if (!file)
//   {
//     Serial.println("Failed to open file for writing");
//     return;
//   }
//   if (file.print(message))
//   {
//     Serial.println("File written");
//   }
//   else
//   {
//     Serial.println("Write failed");
//   }
//   file.close();
// }

// void appendFile(fs::FS &fs, const char *path, const char *message)
// {
//   Serial.printf("Appending to file: %s\n", path);

//   File file = fs.open(path, FILE_APPEND);
//   if (!file)
//   {
//     Serial.println("Failed to open file for appending");
//     return;
//   }
//   if (file.print(message))
//   {
//     Serial.println("Message appended");
//   }
//   else
//   {
//     Serial.println("Append failed");
//   }
//   file.close();
// }

// void renameFile(fs::FS &fs, const char *path1, const char *path2)
// {
//   Serial.printf("Renaming file %s to %s\n", path1, path2);
//   if (fs.rename(path1, path2))
//   {
//     Serial.println("File renamed");
//   }
//   else
//   {
//     Serial.println("Rename failed");
//   }
// }

// void deleteFile(fs::FS &fs, const char *path)
// {
//   Serial.printf("Deleting file: %s\n", path);
//   if (fs.remove(path))
//   {
//     Serial.println("File deleted");
//   }
//   else
//   {
//     Serial.println("Delete failed");
//   }
// }

// void printDirectory(File dir, int numTabs)
// {

//   while (true)
//   {

//     File entry = dir.openNextFile();

//     if (!entry)
//     {

//       // no more files

//       break;
//     }

//     for (uint8_t i = 0; i < numTabs; i++)
//     {

//       Serial.print('\t');
//     }

//     Serial.print(entry.name());
//     Serial.print("  ");
//     Serial.print(entry.getLastWrite());

//     if (entry.isDirectory())
//     {

//       Serial.println("/");

//       printDirectory(entry, numTabs + 1);
//     }
//     else
//     {

//       // files have sizes, directories do not

//       Serial.print("\t\t");

//       Serial.println(entry.size(), DEC);
//     }

//     entry.close();
//   }
// }

// void setup()
// {
//   Serial.begin(115200);
//   // We start by connecting to a WiFi network
//   Serial.println();
//   Serial.println();
//   Serial.print("Connecting to ");
//   Serial.println(ssid);

//   WiFi.begin(ssid, password);

//   while (WiFi.status() != WL_CONNECTED)
//   {
//     delay(500);
//     Serial.print(".");
//   }
//   Serial.println("WiFi connected");
//   Serial.println("IP address: ");
//   Serial.println(WiFi.localIP());
//   Serial.println("Contacting Time Server");
//   configTime(3600 * timezone, daysavetime * 3600, "time.nist.gov", "0.pool.ntp.org", "1.pool.ntp.org");
//   struct tm tmstruct;
//   delay(2000);
//   tmstruct.tm_year = 0;
//   getLocalTime(&tmstruct, 5000);
//   Serial.printf("\nNow is : %d-%02d-%02d %02d:%02d:%02d\n", (tmstruct.tm_year) + 1900, (tmstruct.tm_mon) + 1, tmstruct.tm_mday, tmstruct.tm_hour, tmstruct.tm_min, tmstruct.tm_sec);
//   Serial.println("");

//   if (!SD.begin())
//   {
//     Serial.println("Card Mount Failed");
//     return;
//   }
//   uint8_t cardType = SD.cardType();

//   if (cardType == CARD_NONE)
//   {
//     Serial.println("No SD card attached");
//     return;
//   }

//   Serial.print("SD Card Type: ");
//   if (cardType == CARD_MMC)
//   {
//     Serial.println("MMC");
//   }
//   else if (cardType == CARD_SD)
//   {
//     Serial.println("SDSC");
//   }
//   else if (cardType == CARD_SDHC)
//   {
//     Serial.println("SDHC");
//   }
//   else
//   {
//     Serial.println("UNKNOWN");
//   }

//   uint64_t cardSize = SD.cardSize() / (1024 * 1024);
//   Serial.printf("SD Card Size: %lluMB\n", cardSize);

//   root = SD.open("/");

//   printDirectory(root, 0);

//   Serial.println("done!");

//   // listDir(SD, "/", 0);
//   // removeDir(SD, "/mydir");
//   // createDir(SD, "/mydir");
//   // deleteFile(SD, "/hello.txt");
//   // writeFile(SD, "/hello.txt", "Hello ");
//   // appendFile(SD, "/hello.txt", "World!\n");
//   // listDir(SD, "/", 0);
// }

// void loop()
// {
// }

// #include <SPI.h>
// #include <mySD.h>

// ext::File root;

// void printDirectory(ext::File dir, int numTabs);
// // change this to match your SD shield or module;
// //     Arduino Ethernet shield: pin 4
// //     Adafruit SD shields and modules: pin 10
// //     Sparkfun SD shield: pin 8
// const int chipSelect = 5;

// void setup()
// {
//   // Open serial communications and wait for port to open:
//   Serial.begin(115200);
//   while (!Serial)
//   {
//     ; // wait for serial port to connect. Needed for Leonardo only
//   }

//   Serial.print("Initializing SD card...");
//   // On the Ethernet Shield, CS is pin 4. It's set as an output by default.
//   // Note that even if it's not used as the CS pin, the hardware SS pin
//   // (10 on Arduino Uno boards, 53 on the Mega) must be left as an output
//   // or the SD library functions will not work.
//   pinMode(SS, OUTPUT);

//   if (!SD.begin(chipSelect, MOSI_PIN, MISO_PIN, SCK_PIN))
//   {
//     Serial.println("initialization failed!");
//     return;
//   }
//   Serial.println("initialization done.");

//   root = SD.open("/");

//   printDirectory(root, 0);

//   Serial.println("done!");
// }

// void loop()
// {
//   // nothing happens after setup finishes.
// }

// void printDirectory(ext::File dir, int numTabs)
// {
//   // Begin at the start of the directory
//   dir.rewindDirectory();

//   while (true)
//   {
//     ext::File entry = dir.openNextFile();
//     if (!entry)
//     {
//       // no more files
//       // Serial.println("**nomorefiles**");
//       break;
//     }
//     for (uint8_t i = 0; i < numTabs; i++)
//     {
//       Serial.print('\t'); // we'll have a nice indentation
//     }
//     // Print the 8.3 name
//     Serial.print(entry.name());
//     // Recurse for directories, otherwise print the file size
//     if (entry.isDirectory())
//     {
//       Serial.println("/");
//       printDirectory(entry, numTabs + 1);
//     }
//     else
//     {
//       // files have sizes, directories do not
//       Serial.print("\t\t");
//       Serial.println(entry.size(), DEC);
//     }
//     entry.close();
//   }
// }
// #include "main.h"

// void printLocalTime()
// {
//   struct tm timeinfo;
//   if (!getLocalTime(&timeinfo))
//   {
//     Serial.println("Failed to obtain time");
//     return;
//   }
//   Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
// }

// void setup()
// {
//   Serial.begin(115200);
//   Wifi_Init();
//   sdCard_Init();
//   if (SdIsAvailable)
//     printDirList();
// }

// void loop()
// {
//   delay(1000);
//   if (isTimeReady())
//     printLocalTime();
// }

// #include "main.h"
// #include <SD.h>

// const int SDChipSelect = 5; // Chip select pin for SD card
// File rootDir;               // Root directory File object
// int deletedFileCount = 0, deletedFolderCount = 0, failCount = 0;
// int response = -1;
// const String rootPath = "/"; // Root path as a constant

// void deleteRecursively(File dir, int numTabs);

// void setup()
// {
//   Serial.begin(115200);
//   Wifi_Init();
//   if (!SD.begin(SDChipSelect))
//   {
//     Serial.println("Card failed, or not present");
//     return;
//   }
//   Serial.println("Card initialized.");

//   rootDir = SD.open(rootPath);
//   if (rootDir)
//   {
//     deleteRecursively(rootDir, 0);
//   }
//   else
//   {
//     Serial.println("Failed to open the root directory.");
//   }

//   Serial.print("Deleted ");
//   Serial.print(deletedFileCount);
//   Serial.print(" files and ");
//   Serial.print(deletedFolderCount);
//   Serial.print(" folders from SD card.");

//   if (failCount > 0)
//   {
//     Serial.print(" Failed to delete ");
//     Serial.print(failCount);
//     Serial.println(" items.");
//   }
// }

// void loop()
// {
//   // Intentionally empty. Setup runs once.
// }

// void deleteRecursively(File dir, int numTabs)
// {

//   while (true)
//   {

//     File entry = dir.openNextFile();

//     if (!entry)
//     {

//       if (numTabs == 0)
//         Serial.println("**Done**");
//       // no more files

//       break;
//     }
//     if (entry.isDirectory())
//     {

//       deleteRecursively(entry, numTabs + 1);
//       String fileNamePt = entry.path();
//       Serial.println(fileNamePt);
//       String SysVolInfo = "System Volume Information";
//       fileNamePt.toLowerCase();
//       SysVolInfo.toLowerCase();
//       if (fileNamePt.indexOf(SysVolInfo) == -1)
//       {
//         if (SD.rmdir(entry.path()))
//         {
//           Serial.print("Deleted folder: ");
//           Serial.println(entry.path());
//           deletedFolderCount++;
//         }
//         else
//         {
//           Serial.print("Unable to delete folder: ");
//           Serial.println(entry.path());
//           failCount++;
//         }
//       }
//     }
//     else
//     {
//       String fileNamePt = entry.path();
//       String SysVolInfo = "System Volume Information";
//       fileNamePt.toLowerCase();
//       SysVolInfo.toLowerCase();

//       Serial.println(fileNamePt);
//       if (fileNamePt.indexOf(SysVolInfo) == -1)
//       {
//         unsigned long startTime, endTime, elapsedTime;
//         startTime = micros();
//         if (WifiConnected())
//           response = sendFileOverHTTP(entry);
//         else
//         {
//           Serial.print("Wifi Not Connected");
//           response = -1;
//         }
//         endTime = micros();
//         Serial.print(" ");
//         Serial.print((float)(endTime - startTime) / 1000000.0);
//         Serial.println(" Sec");
//         if (response > 0)
//         {
//           if (SD.remove(entry.path()))
//           {
//             Serial.print("Deleted file: ");
//             Serial.println(entry.path());
//             deletedFileCount++;
//           }
//           else
//           {
//             Serial.print("Failed to delete file: ");
//             Serial.println(entry.path());
//             failCount++;
//           }
//         }
//       }
//     }
//     entry.close();
//   }
// }

// #include "main.h"
// #include "SD.h"

// int last_minute = -1; // Variable to store the last minute
// char folderName[50];
// float sData[9] = {0.0};
// void saveData(char *folderName, float *data, uint16_t len);

// void setup()
// {
//   // Initialize serial communication
//   Serial.begin(115200);
//   pinMode(LED_BUILTIN, OUTPUT);
//   DHT_Init();
//   Wifi_Init();
//   sdCard_Init();
//   while (!WifiConnected())
//     ;
//   Serial.println("Wifi Connected");
//   Serial.println(isTimeReady());
//   while (!isTimeReady())
//     ;
//   Serial.println("Time Ready");
//   struct tm now;
//   while (!getLocalTime(&now))
//   {
//     delay(1000);
//   }
//   Serial.println("Time Get");
//   while (!getLocalTime(&now))
//     ;
//   memset(folderName, 0, 50);
//   sprintf(folderName, "/%02d-%02d-%02d/%02d/%02d.txt", now.tm_year + 1900, now.tm_mon + 1, now.tm_mday, now.tm_hour, now.tm_min);

//   // Check if the folder exists, if not create it
//   if (SdIsAvailable())
//   {
//     StatusCode result = createPathAndFile(folderName);
//     if (result == SUCCESS)
//     {
//       Serial.println("File and directories created successfully.");
//     }
//     else if (result == DIR_CREATION_FAILED)
//     {
//       Serial.println("Error creating directories.");
//     }
//     else if (result == FILE_CREATION_FAILED)
//     {
//       Serial.println("Error creating file.");
//     }
//   }
// }

// void loop()
// {
//   // Get current time
//   struct tm now;
//   if (isTimeReady())
//   {

//     if (getLocalTime(&now))
//     {
//       // Check if the minute is a multiple of 5
//       if (now.tm_min % 5 == 0 && last_minute != now.tm_min)
//       {
//         // Update the last minute
//         last_minute = now.tm_min;

//         // Create folder name based on current time
//         memset(folderName, 0, 50);
//         sprintf(folderName, "/%02d-%02d-%02d/%02d/%02d.txt", now.tm_year + 1900, now.tm_mon + 1, now.tm_mday, now.tm_hour, now.tm_min);

//         // Check if the folder exists, if not create it
//         if (SdIsAvailable())
//         {
//           StatusCode result = createPathAndFile(folderName);
//           if (result == SUCCESS)
//           {
//             Serial.println("File and directories created successfully.");
//           }
//           else if (result == DIR_CREATION_FAILED)
//           {
//             Serial.println("Error creating directories.");
//           }
//           else if (result == FILE_CREATION_FAILED)
//           {
//             Serial.println("Error creating file.");
//           }
//         }
//       }
//     }
//   }
//   // Save data to SD card
//   if (measure_environment(&sData[6], &sData[7], &sData[8]))
//   // if (DHT_MeasureData(&sData[6], &sData[7], &sData[8]))
//   {
//     sData[0] = now.tm_year + 1900;
//     sData[1] = now.tm_mon + 1;
//     sData[2] = now.tm_mday;
//     sData[3] = now.tm_hour;
//     sData[4] = now.tm_min;
//     sData[5] = now.tm_sec;
//     Serial.printf("%S, %.2f, %.2f, %.2f\n", folderName, sData[6], sData[7], sData[8]);
//     if (SdIsAvailable())
//       saveData(folderName, sData, 9);
//   }

//   // Optional: You can perform other tasks here without using delay()
// }

// void saveData(char *folderName, float *data, uint16_t len)
// {
//   // Open file for writing
//   File dataFile = SD.open(folderName, FILE_APPEND);

//   // Check if the file opened successfully
//   if (dataFile)
//   {
//     // Write data to the file
//     dataFile.printf("%.0f-%.0f-%.0f %.0f:%.0f:%.0f, %.2f, %.2f, %.2f\n", data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7], sData[8]);
//     // for (int i = 0; i < len; i++)
//     // {
//     //   dataFile.print(data[i]);
//     //   dataFile.print(", "); // Add delimiter between values
//     // }
//     // dataFile.println(); // Add newline at the end of data set

//     // Close the file
//     dataFile.close();
//     Serial.print(folderName);
//     Serial.println(" Data saved to SD card.");
//     digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
//   }
//   else
//   {
//     // If the file didn't open, print an error
//     Serial.print("Error opening file for writing.");
//     Serial.println(folderName);
//   }
// }

// #include "main.h"
// #include "SD.h"

// void setup() {
//   // Initialize serial communication
//   Serial.begin(115200);

//   sdCard_Init();

//   if(SD.mkdir("/2024-05-15/23-48")){
//         Serial.println("Dir created");
//     } else {
//         Serial.println("mkdir failed");
//     }

//   printDirList();

// }

// void loop() {}

// #include <SPI.h>
// #include <SD.h>

// const int chipSelect = 5; // Change this if you connected CS to a different pin

// // Status codes

// void setup()
// {
//   Serial.begin(115200);
//   while (!Serial)
//   {
//     // Wait for Serial Monitor to open
//   }

//   Serial.print("Initializing SD card...");

//   if (!SD.begin(chipSelect))
//   {
//     Serial.println("Initialization failed!");
//     return;
//   }
//   Serial.println("Initialization done.");

//   // // Create the full path with the file
//   // StatusCode result = createPathAndFile("/2024/05/16/12/34.txt");
//   // if (result == SUCCESS)
//   // {
//   //   Serial.println("File and directories created successfully.");
//   // }
//   // else if (result == DIR_CREATION_FAILED)
//   // {
//   //   Serial.println("Error creating directories.");
//   // }
//   // else if (result == FILE_CREATION_FAILED)
//   // {
//   //   Serial.println("Error creating file.");
//   // }
// }

// void loop()
// {
//   // Nothing to do here
// }

#include <Arduino.h>
#include <Ds1302.h>

// DS1302 RTC instance
Ds1302 rtc(27, 25, 26);

const static char *WeekDays[] =
    {
        "Monday",
        "Tuesday",
        "Wednesday",
        "Thursday",
        "Friday",
        "Saturday",
        "Sunday"};

void setup()
{
  Serial.begin(115200);

  // initialize the RTC
  rtc.init();

  // test if clock is halted and set a date-time (see example 2) to start it
  if (rtc.isHalted())
  {
    Serial.println("RTC is halted. Setting time...");

    Ds1302::DateTime dt = {
        .year = 17,
        .month = Ds1302::MONTH_OCT,
        .day = 3,
        .hour = 4,
        .minute = 51,
        .second = 30,
        .dow = Ds1302::DOW_TUE};

    rtc.setDateTime(&dt);
  }
}

void loop()
{
  // get the current time
  Ds1302::DateTime now;
  rtc.getDateTime(&now);

  static uint8_t last_second = 0;
  if (last_second != now.second)
  {
    last_second = now.second;

    Serial.print("20");
    Serial.print(now.year); // 00-99
    Serial.print('-');
    if (now.month < 10)
      Serial.print('0');
    Serial.print(now.month); // 01-12
    Serial.print('-');
    if (now.day < 10)
      Serial.print('0');
    Serial.print(now.day); // 01-31
    Serial.print(' ');
    Serial.print(WeekDays[now.dow - 1]); // 1-7
    Serial.print(' ');
    if (now.hour < 10)
      Serial.print('0');
    Serial.print(now.hour); // 00-23
    Serial.print(':');
    if (now.minute < 10)
      Serial.print('0');
    Serial.print(now.minute); // 00-59
    Serial.print(':');
    if (now.second < 10)
      Serial.print('0');
    Serial.print(now.second); // 00-59
    Serial.println();
  }

  delay(100);
}
