#ifndef __DOWNLOAD_H__
#define __DOWNLOAD_H__

// possible errors of DownloadToMemory function
#define DTME_NO                    0
#define DTME_INVALIDPARAMETER      1
#define DTME_CANTINITIALIZEWSA     2
#define DTME_CANTCREATESOCKET      3
#define DTME_SERVERNOTFOUND        4
#define DTME_CANTCONNECT           5
#define DTME_CANTSENDHEADERS       6
#define DTME_CANTSENDPARAMETERS    7
#define DTME_RECEIVEDATA           8
#define DTME_ACTIONCANCELED        9


// status events of DownloadToMemory function
#define DTMS_BEGINNING             0
#define DTMS_WSAINITIALIZED        1
#define DTMS_SOCKETCREATED         2
#define DTMS_SERVERFOUND           3
#define DTMS_CONNECTED             4
#define DTMS_HEADERSSENT           5
#define DTMS_PARTOFPARAMETERSSENT  6
#define DTMS_PARAMETERSSENT        7
#define DTMS_PARTOFDATARECEIVED    8
#define DTMS_DATARECEIVED          9
#define DTMS_DOWNLOADINGCOMPLETE   10

int DownloadToMemory(char *lpFrom, char *proxyAddress, char **lplpData, DWORD *lpSize, BOOL postMethod, BOOL (* callback)(WPARAM wParam, LPARAM lParam));

#endif //__DOWNLOAD_H__