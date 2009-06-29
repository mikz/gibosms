#ifndef __REGISTERS_H__
#define __REGISTERS_H__

DWORD RegGetInt(HKEY hKey, LPTSTR lpSubKey, LPTSTR key, int nDefault);
BOOL RegWriteInt(HKEY hKey, LPTSTR lpSubKey, LPTSTR lpValueName, DWORD value);
LPTSTR RegGetString(HKEY hKey, LPTSTR lpSubKey, LPTSTR key, LPTSTR szDefault);
BOOL RegWriteString(HKEY hKey, LPTSTR lpSubKey, LPTSTR lpValueName, LPTSTR value);
void RegDeleteValue(HKEY hKey, LPTSTR lpSubKey, LPTSTR key);

#endif //__REGISTERS_H__