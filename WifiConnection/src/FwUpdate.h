#ifndef FWUPDATE_H
#define FWUPDATE_H


bool performUpdate();
bool downloadAndPrepareUpdate(String url_update);
void checkUpdate(void);
void headerinit(void);

#endif //FWUPDATE_H