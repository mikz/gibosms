#ifndef __FCE_H__
#define __FCE_H__

#include <windows.h>
#include "CArray.h"
#include "CHdc.h"

char *EncodeURL(char *url);
WCHAR *ConvertToWideChar(char *lpszA, UINT codePage);
char *ConvertToMultiByte(WCHAR *lpszW, UINT codePage);
BOOL GetGiboURL(char *url, WCHAR **text, WCHAR **extras, BOOL (* callback)(WPARAM wParam, LPARAM lParam));
int LVAddItem(HWND list, WCHAR *text, int i, int iImage);
int LVSetSelectedItem(HWND hList, int n);
int LVGetSelectedItem(HWND hList);
WCHAR *GetWinText(HWND hWnd);
BOOL ConnectToInternet();
WCHAR *ClearSpaces(WCHAR *text);
CArray<WCHAR *> *WrapText(WCHAR *text, HDC hdc, int maxWidth, int *lineHeight);
CArray<WCHAR *> *WrapText(WCHAR *text, LOGFONT *font, int maxWidth, int *lineHeight);
CArray<WCHAR *> *WrapTextMultiLine(WCHAR *text, HDC hdc, int maxWidth, int *lineHeight);
CArray<WCHAR *> *WrapTextMultiLine(WCHAR *text, LOGFONT *font, int maxWidth, int *lineHeight);
WCHAR **explode(WCHAR *str, WCHAR *d, unsigned long* count, BOOL split);
char **explode(char *str, char *d, unsigned long* count, BOOL split);
WCHAR *CorrectEnters(WCHAR *text);
BOOL IsInArray(int *array, int num, int val);
WCHAR *RegGetString(HKEY hKey, WCHAR *lpSubKey, WCHAR *key, WCHAR *szDefault);
BOOL FileExists(WCHAR *fileName);
WCHAR *GetApplicationFile(WCHAR *fileName);
DWORD SystemTimeToUnixEpoch(SYSTEMTIME *st);
int hex2int(char hex);
void DrawPictogram(CHdc *hdcPictogram, char *pictogramData, int width, int height);
LONG ComputeFontSize(HWND hWnd, LONG pointSize);
LONG ComputeFontSize(HDC hdc, LONG pointSize);


#endif //__FCE_H__