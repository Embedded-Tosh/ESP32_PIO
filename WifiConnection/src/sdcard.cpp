
#include "main.h"
#include "FS.h"
#include "SD.h"
#include "SPI.h"
#include "sdcard.h"

OperationResult result = {0, 0, 0, -1};
bool sdInitStatus = false;
File root;

bool SdIsAvailable(void)
{
    return sdInitStatus;
}

void createDir(const char *path)
{
    Serial.printf("Creating Dir: %s\n", path);
    if (SD.mkdir(path))
    {
        Serial.println("Dir created");
    }
    else
    {
        Serial.println("mkdir failed");
    }
}

void removeDir(const char *path)
{
    Serial.printf("Removing Dir: %s\n", path);
    if (SD.rmdir(path))
    {
        Serial.println("Dir removed");
    }
    else
    {
        Serial.println("rmdir failed");
    }
}

void readFile(const char *path)
{
    Serial.printf("Reading file: %s\n", path);

    File file = SD.open(path);
    if (!file)
    {
        Serial.println("Failed to open file for reading");
        return;
    }

    Serial.print("Read from file: ");
    while (file.available())
    {
        Serial.write(file.read());
    }
    file.close();
}

void writeFile(const char *path, const char *message)
{
    Serial.printf("Writing file: %s\n", path);

    File file = SD.open(path, FILE_WRITE);
    if (!file)
    {
        Serial.println("Failed to open file for writing");
        return;
    }
    if (file.print(message))
    {
        Serial.println("File written");
    }
    else
    {
        Serial.println("Write failed");
    }
    file.close();
}

void appendFile(const char *path, const char *message)
{
    Serial.printf("Appending to file: %s\n", path);

    File file = SD.open(path, FILE_APPEND);
    if (!file)
    {
        Serial.println("Failed to open file for appending");
        return;
    }
    if (file.print(message))
    {
        Serial.println("Message appended");
    }
    else
    {
        Serial.println("Append failed");
    }
    file.close();
}

void renameFile(const char *path1, const char *path2)
{
    Serial.printf("Renaming file %s to %s\n", path1, path2);
    if (SD.rename(path1, path2))
    {
        Serial.println("File renamed");
    }
    else
    {
        Serial.println("Rename failed");
    }
}

void deleteFile(const char *path)
{
    Serial.printf("Deleting file: %s\n", path);
    if (SD.remove(path))
    {
        Serial.println("File deleted");
    }
    else
    {
        Serial.println("Delete failed");
    }
}

void printDirectory(File dir, int numTabs)
{
    // Begin at the start of the directory
    dir.rewindDirectory();

    while (true)
    {
        File entry = dir.openNextFile();
        if (!entry)
        {
            // no more files
            if (numTabs == 0)
                Serial.println("** Done **");
            return;
        }
        for (uint8_t i = 0; i < numTabs; i++)
        {
            Serial.print('\t'); // we'll have a nice indentation
        }
        // Print the 8.3 name
        const char *cFileName;    // Create a pointer char variable - for the FileName
        cFileName = entry.name(); // Assign the first entry of entry.name() to the FileName pointer created
        Serial.print(cFileName);
        // Recurse for directories, otherwise print the file size
        if (entry.isDirectory())
        {
            Serial.print("/\t");
            time_t t = entry.getLastWrite();
            struct tm *tmstruct = localtime(&t);
            Serial.printf("%d-%02d-%02d %02d:%02d:%02d\n", (tmstruct->tm_year) + 1900, (tmstruct->tm_mon) + 1, tmstruct->tm_mday, tmstruct->tm_hour, tmstruct->tm_min, tmstruct->tm_sec);
            printDirectory(entry, numTabs + 1);
        }
        else
        {
            // Files have sizes, directories do not
            Serial.print(F("\t"));
            time_t t = entry.getLastWrite();
            struct tm *tmstruct = localtime(&t);
            Serial.printf("%d-%02d-%02d %02d:%02d:%02d\t", (tmstruct->tm_year) + 1900, (tmstruct->tm_mon) + 1, tmstruct->tm_mday, tmstruct->tm_hour, tmstruct->tm_min, tmstruct->tm_sec);

            if (entry.size() > 1024 * 1024)
            { // > 1 MB
                Serial.print(entry.size() / 1024.0 / 1024.0, 2);
                Serial.println(F(" MB"));
            }
            else if (entry.size() > 1024)
            { // > 1kB
                if (entry.size() > 102400)
                { // > 100 kb
                    Serial.print(entry.size() / 1024.0, 0);
                }
                else
                {
                    Serial.print(entry.size() / 1024.0, 2);
                }
                Serial.println(F(" kB"));
            }
            else
            { // bytes
                Serial.print(entry.size(), DEC);
                Serial.println(F(" bytes"));
            }
            for (uint8_t i = 0; i < numTabs; i++)
            {
                Serial.print('\t'); // we'll have a nice indentation
            }
            unsigned long startTime, endTime, elapsedTime;
            startTime = micros();
            if (WifiConnected())
                sendFileOverHTTP(entry);
            else
                Serial.print("Wifi Not Connected");
            endTime = micros();
            Serial.print(" ");
            Serial.print((float)(endTime - startTime) / 1000000.0);
            Serial.println(" Sec");
        }
        entry.close();
    }
}

//============================================================================================================//
void SendFileOverHTTPandDeleteFileFolders(File dir, int numTabs)
{

    while (true)
    {

        File entry = dir.openNextFile();

        if (!entry)
        {

            if (numTabs == 0)
                Serial.println("**Done**");
            // no more files

            break;
        }
        if (entry.isDirectory())
        {

            SendFileOverHTTPandDeleteFileFolders(entry, numTabs + 1);
            String fileNamePt = entry.path();
            Serial.println(fileNamePt);
            String SysVolInfo = "System Volume Information";
            fileNamePt.toLowerCase();
            SysVolInfo.toLowerCase();
            if (fileNamePt.indexOf(SysVolInfo) == -1)
            {
                if (SD.rmdir(entry.path()))
                {
                    Serial.print("Deleted folder: ");
                    Serial.println(entry.path());
                    result.deletedFolderCount++;
                }
                else
                {
                    Serial.print("Unable to delete folder: ");
                    Serial.println(entry.path());
                    result.failCount++;
                }
            }
        }
        else
        {
            String fileNamePt = entry.path();
            String SysVolInfo = "System Volume Information";
            fileNamePt.toLowerCase();
            SysVolInfo.toLowerCase();

            Serial.println(fileNamePt);
            if (fileNamePt.indexOf(SysVolInfo) == -1)
            {
                unsigned long startTime, endTime, elapsedTime;
                startTime = micros();
                if (WifiConnected())
                    result.response = sendFileOverHTTP(entry);
                else
                {
                    Serial.print("Wifi Not Connected");
                    result.response = -1;
                }
                endTime = micros();
                Serial.print(" ");
                Serial.print((float)(endTime - startTime) / 1000000.0);
                Serial.println(" Sec");
                if (result.response > 0)
                {
                    if (SD.remove(entry.path()))
                    {
                        Serial.print("Deleted file: ");
                        Serial.println(entry.path());
                        result.deletedFileCount++;
                    }
                    else
                    {
                        Serial.print("Failed to delete file: ");
                        Serial.println(entry.path());
                        result.failCount++;
                    }
                }
            }
        }
        entry.close();
    }
}

//============================================================================================================//

void printDirList(void)
{
    root = SD.open("/");
    printDirectory(root, 0);
}

//============================================================================================================//
StatusCode createPathAndFile(const char *fullPath)
{
    String tempPath = String(fullPath);
    String currentPath = "";

    int index = 0;
    while (true)
    {
        int nextSlash = tempPath.indexOf('/', index);
        if (nextSlash == -1)
        {
            break;
        }
        currentPath = tempPath.substring(0, nextSlash);
        if (!SD.exists(currentPath))
        {
            if (!SD.mkdir(currentPath))
            {
                return DIR_CREATION_FAILED;
            }
        }
        index = nextSlash + 1;
    }

    // Create the file
    File file = SD.open(fullPath, FILE_APPEND);
    if (file)
    {
        file.close();
        return SUCCESS;
    }
    else
        return FILE_CREATION_FAILED;
}

//============================================================================================================//

//============================================================================================================//
void sdCard_Init(void)
{
    int attemptCount = 0;

    while (attemptCount < 10)
    {
        if (!SD.begin())
        {
            Serial.printf("Card Mount Failed, Retry...(%d/10)\n", attemptCount + 1);
            sdInitStatus = false;
        }
        else
        {
            Serial.println("Card Mount Successful");
            sdInitStatus = true;
            break;
        }
        attemptCount++;
        delay(1000);
    }

    if (!sdInitStatus)
    {
        Serial.println("SD card initialization failed.");
        return;
    }

    uint8_t cardType = SD.cardType();

    if (cardType == CARD_NONE)
    {
        Serial.println("No SD card attached");
        sdInitStatus = false;
        return;
    }

    Serial.print("SD Card Type: ");
    switch (cardType)
    {
    case CARD_MMC:
        Serial.println("MMC");
        break;
    case CARD_SD:
        Serial.println("SDSC");
        break;
    case CARD_SDHC:
        Serial.println("SDHC");
        break;
    default:
        Serial.println("UNKNOWN");
        break;
    }

    float cardSize = (float)SD.cardSize() / (1024 * 1024 * 1024);
    Serial.printf("SD Card Size: %.2fGB\n", cardSize);
    Serial.printf("Total space: %lluMB\n", SD.totalBytes() / (1024 * 1024));
    Serial.printf("Used space: %lluMB\n", SD.usedBytes() / (1024 * 1024));
}
//============================================================================================================//