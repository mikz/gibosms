#pragma once

#include "CArray.h"
#include "CHdc.h"

LPTSTR GetWinText(HWND hWnd);
BOOL AddTray(HWND hWnd, UINT ID, UINT callbackMessage, HICON icon, LPTSTR szTip);
BOOL RemoveTray(HWND hWnd, UINT ID);
BOOL ModifyTray(HWND hWnd, UINT ID, HICON icon);

char *EncodeURL(char *url);
WCHAR *ConvertToWideChar(char *lpszA, UINT codePage);
char *ConvertToMultiByte(WCHAR *lpszW, UINT codePage);
BOOL GetGiboURL(char *url, char *proxy, WCHAR **text, WCHAR **extras, BOOL (* callback)(WPARAM wParam, LPARAM lParam));
int LVAddItem(HWND list, WCHAR *text, int i, int iImage);
int LVSetSelectedItem(HWND hList, int n);
int LVGetSelectedItem(HWND hList);
WCHAR *GetWinText(HWND hWnd);
WCHAR *ClearSpaces(WCHAR *text);
CArray<WCHAR *> *WrapText(WCHAR *text, HDC hdc, int maxWidth, int *lineHeight);
CArray<WCHAR *> *WrapText(WCHAR *text, LOGFONT *font, int maxWidth, int *lineHeight);
CArray<WCHAR *> *WrapTextMultiLine(WCHAR *text, HDC hdc, int maxWidth, int *lineHeight);
CArray<WCHAR *> *WrapTextMultiLine(WCHAR *text, LOGFONT *font, int maxWidth, int *lineHeight);
WCHAR **explode(WCHAR *str, WCHAR *d, unsigned long* count, BOOL split);
char **explode(char *str, char *d, unsigned long* count, BOOL split);
WCHAR *CorrectEnters(WCHAR *text);
BOOL IsInArray(int *array, int num, int val);
BOOL FileExists(WCHAR *fileName);
WCHAR *GetApplicationFile(WCHAR *fileName);
DWORD SystemTimeToUnixEpoch(SYSTEMTIME *st);
int hex2int(char hex);
void DrawPictogram(CHdc *hdcPictogram, char *pictogramData, int width, int height);