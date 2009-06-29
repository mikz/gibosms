#include "Fce.h"
#include <windows.h>
#include <aygshell.h>
#include "Download.h"
#include <connmgr.h>


/***********************************************************************************
* Returns encoded URL string                                                       *
***********************************************************************************/
char *EncodeURL(char *url) {
	if (!url) return NULL;
	int nNonAlNum = 0;
	char *p = url;
	while (*p) {
		if (!isalnum((unsigned char)*p) && *p != ' ') nNonAlNum++;
		p++;
	}
	char *encodedUrl = new char[strlen(url) + 2*nNonAlNum + 1 + 10000];
	p = url;
	char *ep = encodedUrl;
	while (*p) {
		if (*p == ' ') {
			*ep = '+';
			ep++;
		}else if (isalnum((unsigned char)*p)) {
			*ep = *p;
			ep++;
		}else {
			sprintf(ep, "%%%02X", (unsigned char)*p);
			ep += 3;
		}
		p++;
	}
	*ep = '\0';
	return encodedUrl;
}


/***********************************************************************************
* Maps a character string to a wide-character (Unicode) string                     *
***********************************************************************************/
WCHAR *ConvertToWideChar(char *lpszA, UINT codePage) {
	if (lpszA) {
		int lenW = MultiByteToWideChar(codePage, 0, lpszA, -1, NULL, 0);
		if (!lenW) return NULL;
		WCHAR *lpszW = new WCHAR[lenW];
		if (!lpszW) return NULL;
		MultiByteToWideChar(codePage, 0, lpszA, -1, lpszW, lenW);
		return lpszW;
	}
	return NULL;
}


/***********************************************************************************
* Maps a wide-character string to a new character string                           *
***********************************************************************************/
char *ConvertToMultiByte(WCHAR *lpszW, UINT codePage) {
	if (lpszW) {
		int lenA = WideCharToMultiByte(codePage, 0, lpszW, -1, NULL, 0, NULL, NULL);
		if (!lenA) return NULL;
		char *lpszA = new char[lenA];
		if (!lpszA) return NULL;
		WideCharToMultiByte(codePage, 0, lpszW, -1, lpszA, lenA, NULL, NULL);
		return lpszA;
	}
	return NULL;
}


/***********************************************************************************
* Returns page data                                                                *
***********************************************************************************/
BOOL GetGiboURL(char *url, WCHAR **text, WCHAR **extras, BOOL (* callback)(WPARAM wParam, LPARAM lParam)) {
	char *data = NULL;
	BOOL retVal = false;
	if (extras) *extras = NULL;
#ifndef DEBUG
	if (ConnectToInternet()) {
#else
	if (true) {
#endif
		DWORD size = 0;
		int err = DownloadToMemory(url, NULL, &data, &size, true, NULL);
		if (err == DTME_NO) {
			if (text) *text = ConvertToWideChar(data, CP_ACP);
			if (extras) {
				DWORD dataLen = strlen(data);
				if (size > dataLen) {
					*extras = ConvertToWideChar(data + dataLen + 1, CP_ACP);
				}
			}
			retVal = true;
		}else {
			if (text) {
				*text = new WCHAR[100];
				if (err != DTME_ACTIONCANCELED) {
					WCHAR *errs[9] = {L"Žádná chyba", L"Byl zadán špatný parametr", L"Nepodaøilo se inicializovat winsock", L"Nepodaøilo se vytvoøit socket", L"Server nebyl nalezen", L"Nepodaøilo se pøipojit na server", L"Napodaøilo se poslat HTTP hlavièku", L"Napodaøilo se poslat HTTP data", L"Chyba pøi pøíjmu dat"};
					wsprintf(*text, L"Bìhem posílání nastala chyba #%i...(%s)", err, err < 9 ? errs[err] : L"Neznámá chyba");
				}else wcscpy(*text, L"Akce stornována");
			}
		}
		free(data);
	}else {
		if (text) *text = wcsdup(L"Nepodaøilo se pøipojit na internet");
	}
	return retVal;
}


/***********************************************************************************
* Adds an item to the listview control                                             *
***********************************************************************************/
int LVAddItem(HWND list, WCHAR *text, int i, int iImage) {
	LVITEM item;
	item.mask = LVIF_TEXT|LVIF_IMAGE;
	item.iSubItem = 0;
	item.iItem = i;
	item.cchTextMax = 1000;
	item.pszText = text;
	item.iImage = iImage;
	// pøidá položku
	return ListView_InsertItem(list,&item);
}


/***********************************************************************************
* Selects n-th item                                                                *
***********************************************************************************/
int LVSetSelectedItem(HWND hList, int n) {
	int num = ListView_GetItemCount(hList);
	for (int i = 0; i < num; i++) {
		if (i == n) {
			ListView_SetSelectionMark(hList, i);
			ListView_SetItemState(hList, i, LVIS_FOCUSED|LVIS_SELECTED, LVIS_FOCUSED|LVIS_SELECTED);
			ListView_EnsureVisible(hList, i, false);
		}else {
			ListView_SetItemState(hList, i, 0, LVIS_FOCUSED|LVIS_SELECTED);
		}
	}
	return -1;
}


/***********************************************************************************
* Return index of selected item (or -1)                                            *
***********************************************************************************/
int LVGetSelectedItem(HWND hList) {
	int num = ListView_GetItemCount(hList);
	for (int i = 0; i < num; i++) {
		// check whether item is selected
		if (ListView_GetItemState(hList, i, LVIS_SELECTED)) {
			return i;
		}
	}
	return -1;
}


/***********************************************************************************
* Returns title of the window                                                      *
***********************************************************************************/
WCHAR *GetWinText(HWND hWnd) {
	int size = GetWindowTextLength(hWnd) + 1;
	WCHAR *text = new WCHAR[size];
	GetWindowText(hWnd, text, size);
	return text;
}


/***********************************************************************************
* Connects mobile to the internet                                                  *
***********************************************************************************/
BOOL ConnectToInternet() {
	HANDLE evnt = ConnMgrApiReadyEvent();
	WaitForSingleObject(evnt, INFINITE);

	GUID networkGuid;
	ConnMgrMapURL(L"http://sms.dynawest.cz", &networkGuid, NULL);

	CONNMGR_CONNECTIONINFO ConnInfo;
	memset(&ConnInfo, 0, sizeof(ConnInfo));
	ConnInfo.cbSize = sizeof(ConnInfo);
	ConnInfo.dwParams = CONNMGR_PARAM_GUIDDESTNET;
	ConnInfo.dwFlags = 0;
	ConnInfo.dwPriority = CONNMGR_PRIORITY_USERINTERACTIVE;
	ConnInfo.guidDestNet = networkGuid;

	HANDLE m_hConnection;
	HRESULT hr = ConnMgrEstablishConnection(&ConnInfo, &m_hConnection);

	DWORD status;
	DWORD tm = GetTickCount();
	while (!ConnMgrConnectionStatus(m_hConnection, &status)) {
		if (status == CONNMGR_STATUS_CONNECTED ||
		    status == CONNMGR_STATUS_CONNECTIONFAILED ||
		    status == CONNMGR_STATUS_CONNECTIONCANCELED ||
		    status == CONNMGR_STATUS_CONNECTIONDISABLED ||
		    status == CONNMGR_STATUS_WAITINGCONNECTIONABORT)
			break;
		if (GetTickCount() > tm + 60000) break;
		Sleep(1000);
	}
	return status == CONNMGR_STATUS_CONNECTED;
}


/***********************************************************************************
* Returns optimized text without spaces                                            *
***********************************************************************************/
WCHAR *ClearSpaces(WCHAR *text) {
	BOOL spaceBefore = true;
	WCHAR *s = text;
	while (*text) {
		if (*text == ' ') {
			spaceBefore = true;
		}else {
			WCHAR b[2] = {*text, '\0'};
			*s = spaceBefore ? CharUpper(b)[0] : *text;
			s++;
			spaceBefore = false;
		}
		text++;
	}
	*s = '\0';
	return s;
}


/***********************************************************************************
* Returns pointers to each line of text which is fitted within specified space     *
***********************************************************************************/
CArray<WCHAR *> *WrapText(WCHAR *text, HDC hdc, int maxWidth, int *lineHeight) {
	CArray<WCHAR *> *lines = new CArray<WCHAR *>;
	if (text && hdc) {
		SIZE sz;
		int textLen = wcslen(text);
		int *WCHARsPos = new int[textLen];
		if (*text && GetTextExtentExPoint(hdc, text, textLen, 0, NULL, WCHARsPos, &sz)) {
			if (lineHeight) *lineHeight = sz.cy;
			if (sz.cx > maxWidth) {
				WCHAR *lastSpace = NULL;
				WCHAR *lastLine = text;
				int lineWidth = 0;
				for (int i = 0; i < textLen; i++) {
					lineWidth = WCHARsPos[i] - (lastLine == text ? 0 : WCHARsPos[lastLine - text - 1]);
					if (lineWidth > maxWidth) {
						lines->Add(lastLine);
						if (lastSpace) lastLine = lastSpace + 1;
						else lastLine = text + i;
						lastSpace = NULL;
					}
					if (text[i] == ' ') lastSpace = text + i;
				}
				lines->Add(lastLine);
			}else {
				lines->Add(text);
			}
		}else {
			lines->Add(text);
			if (lineHeight) {
				GetTextExtentPoint32(hdc, L"X", 1, &sz);
				*lineHeight = sz.cy;
			}
		}
		delete WCHARsPos;
	}
	return lines;
}


/***********************************************************************************
* Returns pointers to each line of text which is fitted within specified space     *
***********************************************************************************/
CArray<WCHAR *> *WrapText(WCHAR *text, LOGFONT *font, int maxWidth, int *lineHeight) {
	HDC hdcScreen = GetDC(0);
	HDC hdcTmp = CreateCompatibleDC(hdcScreen);
	HFONT hFnt = CreateFontIndirect(font);
	HFONT hFntOld = (HFONT)SelectObject(hdcTmp, hFnt);
	CArray<WCHAR *> *retVal = WrapText(text, hdcTmp, maxWidth, lineHeight);
	SelectObject(hdcTmp, hFntOld);
	DeleteObject(hFnt);
	DeleteDC(hdcTmp);
	ReleaseDC(0, hdcScreen);
	return retVal;
}


/***********************************************************************************
* Returns pointers to each line of text which is fitted within specified space     *
***********************************************************************************/
CArray<WCHAR *> *WrapTextMultiLine(WCHAR *text, HDC hdc, int maxWidth, int *lineHeight) {
	CArray<WCHAR *> *lines = new CArray<WCHAR *>;
	DWORD i;
	
	WCHAR *dupText = wcsdup(text);
	DWORD numLines;
	WCHAR **pLines = explode(dupText, L"\r\n", &numLines, true);
	for (i = 0; i < numLines; i++) {
		CArray<WCHAR *> *subLines = WrapText(pLines[i], hdc, maxWidth, lineHeight);
		for (DWORD n = 0; n < subLines->num; n++) {
			lines->Add((*subLines)[n]);
		}
		delete subLines;
	}
	
	for (i = 0; i < lines->num; i++) {
		(*lines)[i] = text + ((*lines)[i] - dupText);
	}
	
	free(pLines);
	free(dupText);

	return lines;
}


/***********************************************************************************
* Returns pointers to each line of text which is fitted within specified space     *
***********************************************************************************/
CArray<WCHAR *> *WrapTextMultiLine(WCHAR *text, LOGFONT *font, int maxWidth, int *lineHeight) {
	HDC hdcScreen = GetDC(0);
	HDC hdcTmp = CreateCompatibleDC(hdcScreen);
	HFONT hFnt = CreateFontIndirect(font);
	HFONT hFntOld = (HFONT)SelectObject(hdcTmp, hFnt);
	CArray<WCHAR *> *retVal = WrapTextMultiLine(text, hdcTmp, maxWidth, lineHeight);
	SelectObject(hdcTmp, hFntOld);
	DeleteObject(hFnt);
	DeleteDC(hdcTmp);
	ReleaseDC(0, hdcScreen);
	return retVal;
}


/***********************************************************************************
* Vrátí pole pointerù na stringy rozdìlených z jednoho stringu podle stringu       *
***********************************************************************************/
WCHAR **explode(WCHAR *str, WCHAR *d, unsigned long* count, BOOL split) {
#define POKOLIKA 10 // alokuje to pole po 10
	// alokuje pole pointeru
	unsigned long alokovano=0;
  WCHAR **a=(WCHAR **)malloc( (alokovano+=POKOLIKA) * sizeof(WCHAR *) );
	size_t offset=wcslen(d);
	*count=1;

	a[0]=str;
	while(str=wcsstr(str, d)){
		str+=offset;
		if (split) *(str-offset)='\0';
		a[(*count)++]=str;
		if(*count==alokovano) a=(WCHAR **)realloc(a, (alokovano+=POKOLIKA) * sizeof(WCHAR *) );
	}

	return a;	
}


/***********************************************************************************
* Vrátí pole pointerù na stringy rozdìlených z jednoho stringu podle stringu       *
***********************************************************************************/
char **explode(char *str, char *d, unsigned long* count, BOOL split) {
#define POKOLIKA 10 // alokuje to pole po 10
	// alokuje pole pointeru
	unsigned long alokovano=0;
  char **a=(char **)malloc( (alokovano+=POKOLIKA) * sizeof(char *) );
	size_t offset=strlen(d);
	*count=1;

	a[0]=str;
	while(str=strstr(str, d)){
		str+=offset;
		if (split) *(str-offset)='\0';
		a[(*count)++]=str;
		if(*count==alokovano) a=(char **)realloc(a, (alokovano+=POKOLIKA) * sizeof(char *) );
	}

	return a;	
}


/***********************************************************************************
* Converts linux ends of lines (\n) to windows ends of lines (\r\n)                *
***********************************************************************************/
WCHAR *CorrectEnters(WCHAR *text) {
	if (text) {
		WCHAR *p = text;
		int last = 0;
		WCHAR *retVal = NULL;
		
		DWORD allocated = 0;
		DWORD necessarySize = 1; // for '\0'
		int ALLOC_STEP = 100;
		
		while (*p) {
			if (*p == '\n' && (p == text || *(p - 1) != '\r')) {
				necessarySize++;
				if (necessarySize >= allocated) retVal = (WCHAR *)realloc(retVal, (allocated += ALLOC_STEP) * sizeof(WCHAR));
				retVal[last++] = '\r';
			}else if (*p == '\r' && *(p + 1) != '\n') {
				necessarySize += 2;
				if (necessarySize >= allocated) retVal = (WCHAR *)realloc(retVal, (allocated += ALLOC_STEP) * sizeof(WCHAR));
				retVal[last++] = *(p++);
				retVal[last++] = '\n';
				continue;
			}
			necessarySize++;
			if (necessarySize >= allocated) retVal = (WCHAR *)realloc(retVal, (allocated += ALLOC_STEP) * sizeof(WCHAR));
			retVal[last++] = *(p++);
		}
		if (!retVal) retVal = _wcsdup(text);
		else retVal[last] = '\0';
		return retVal;
	}
	return NULL;
}


/***********************************************************************************
* Returns whether the value is in array                                            *
***********************************************************************************/
BOOL IsInArray(int *array, int num, int val) {
	for (int i = 0; i < num; i++) {
		if (array[i] == val) return true;
	}
	return false;
}


/***********************************************************************************
* Returns string loaded from windows register                                      *
***********************************************************************************/
WCHAR *RegGetString(HKEY hKey, WCHAR *lpSubKey, WCHAR *key, WCHAR *szDefault) {
	WCHAR *retVal = NULL;
	HKEY hhKey;
	if (RegOpenKeyEx(hKey, lpSubKey, 0, KEY_ALL_ACCESS, &hhKey) == ERROR_SUCCESS) {
		DWORD size;
		RegQueryValueEx(hhKey, key, NULL, NULL, NULL, &size);
		if (size) {
			retVal = new WCHAR[size];
			RegQueryValueEx(hhKey, key, NULL, NULL, (BYTE *)retVal, &size);
		}
		RegCloseKey(hKey);
	}
	if (!retVal && szDefault) retVal = wcsdup(szDefault);
	return retVal;
}


/***********************************************************************************
* Returns whether the file exists                                                  *
***********************************************************************************/
BOOL FileExists(WCHAR *fileName) {
	return GetFileAttributes(fileName) != 0xFFFFFFFF;
}

/***********************************************************************************
* Return whole path to application file                                            *
***********************************************************************************/
WCHAR *GetApplicationFile(WCHAR *fileName) {
	WCHAR *path = new WCHAR[MAX_PATH];

	GetModuleFileName(NULL, path, MAX_PATH + 1);
	WCHAR *p = wcsrchr(path, '\\');
	if (p) {
		wcscpy(p + 1, fileName);
		return path;
	}
	delete[] path;
	return NULL;
}

/***********************************************************************************
* Converts system time to time measured in the number of seconds since the Unix    *
* Epoch (January 1 1970 00:00:00 GMT).                                             *
***********************************************************************************/
DWORD SystemTimeToUnixEpoch(SYSTEMTIME *st) {
	if (!st) return 0;
	DWORD days = 0;
	days += (st->wYear - 1970) * 365;
	days += (st->wYear - 1972)  / 4 + 1;
	int months[] = {31, (st->wYear - 1972) % 4 == 0 ? 29 : 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	for (int i = 1; i < st->wMonth; i++) {
		days += months[i];
	}
	days += st->wDay;

	return days * 3600 * 24 + st->wHour * 3600 + st->wMinute * 60 + st->wSecond;
}

/***********************************************************************************
* Returns integer number from hex number                                           *
***********************************************************************************/
int hex2int(char hex) {
	switch (hex) {
		case 'a': return 10;
		case 'b': return 11;
		case 'c': return 12;
		case 'd': return 13;
		case 'e': return 14;
		case 'f': return 15;
		case 'A': return 10;
		case 'B': return 11;
		case 'C': return 12;
		case 'D': return 13;
		case 'E': return 14;
		case 'F': return 15;
	}
	return hex - '0';
}

/***********************************************************************************
* Draws pictogram                                                                  *
***********************************************************************************/
void DrawPictogram(CHdc *hdcPictogram, char *pictogramData, int width, int height) {
	hdcPictogram->SetSize(width + 6, height + 6);

	BitBlt(*hdcPictogram, 0, 0, hdcPictogram->GetWidth(), hdcPictogram->GetHeight(), 0, 0, 0, WHITENESS);

	// ohranicujici obedelnik
	Rectangle(*hdcPictogram, 1, 1, width + 5, height + 5);

	int offsetX = 3, offsetY = 3;

	int actBit = 3;
	int actPos = 0;
	int len = strlen(pictogramData);
	int value = hex2int(pictogramData[actPos]);

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			if ((value & (1 << actBit)) != 0) SetPixel(*hdcPictogram, offsetX + x, offsetY + y, RGB(0, 0, 0));

			actBit--;
			if (actBit == -1 && actPos + 1 < len) {
				actBit = 3;
				actPos++;
				value = hex2int(pictogramData[actPos]);
			}
		}
	}
}

/***********************************************************************************
* Computes font size                                                               *
***********************************************************************************/
LONG ComputeFontSize(HWND hWnd, LONG pointSize)
{
	HDC hdc = GetDC(hWnd);
	LONG size = -(pointSize * GetDeviceCaps(hdc, LOGPIXELSY)) / 72;
	ReleaseDC(hWnd, hdc);
	return size;
}

/***********************************************************************************
* Computes font size                                                               *
***********************************************************************************/
LONG ComputeFontSize(HDC hdc, LONG pointSize)
{
	return -(pointSize * GetDeviceCaps(hdc, LOGPIXELSY)) / 72;
}