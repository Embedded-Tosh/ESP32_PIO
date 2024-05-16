#ifndef SDCARD_H
#define SDCARD_H

typedef enum
{
    SUCCESS = 0,
    DIR_CREATION_FAILED,
    FILE_CREATION_FAILED,
    SD_INITIALIZATION_FAILED
} StatusCode;

typedef struct
{
    int deletedFileCount;
    int deletedFolderCount;
    int failCount;
    int response;
} OperationResult;

extern OperationResult result;

StatusCode createPathAndFile(const char *fullPath);
void listDir(const char *dirname, uint8_t levels);
void createDir(const char *path);
void removeDir(const char *path);
void readFile(const char *path);
void writeFile(const char *path, const char *message);
void appendFile(const char *path, const char *message);
void renameFile(const char *path1, const char *path2);
void deleteFile(const char *path);
void printDirectory(File dir, int numTabs);
void SendFileOverHTTPandDeleteFileFolders(File dir, int numTabs);
void sdCard_Init(void);
bool SdIsAvailable(void);
void printDirList(void);

#endif // SDCARD_H