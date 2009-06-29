#include "stdafx.h"
#include "Registers.h"

/***********************************************************************************
* Vr�t� intovou hodnotu z registr�                                                 *
***********************************************************************************/
DWORD RegGetInt(HKEY hKey, LPTSTR lpSubKey, LPTSTR key, int nDefault) {
	DWORD retVal = nDefault;
	HKEY hhKey;
	if (RegOpenKeyEx(hKey, lpSubKey, 0, KEY_ALL_ACCESS, &hhKey) == ERROR_SUCCESS) {
		DWORD size = sizeof(DWORD);
		RegQueryValueEx(hhKey, key, NULL, NULL, (BYTE *)&retVal, &size);
		RegCloseKey(hKey);
	}
	return retVal;
}

/***********************************************************************************
* Ulo�� intovou hodnotu do registr�                                                *
***********************************************************************************/
BOOL RegWriteInt(HKEY hKey, LPTSTR lpSubKey, LPTSTR lpValueName, DWORD value) {
	if (RegCreateKey(hKey, lpSubKey, &hKey) == ERROR_SUCCESS) {
		RegSetValueEx(hKey, lpValueName, NULL, REG_DWORD, (BYTE *)&value, sizeof(DWORD));
		RegCloseKey(hKey);
		return true;
	}else return false;
}

/***********************************************************************************
* Returns string loaded from windows register                                      *
***********************************************************************************/
LPTSTR RegGetString(HKEY hKey, LPTSTR lpSubKey, LPTSTR key, LPTSTR szDefault) {
	LPTSTR retVal = NULL;
	HKEY hhKey;
	if (RegOpenKeyEx(hKey, lpSubKey, 0, KEY_ALL_ACCESS, &hhKey) == ERROR_SUCCESS) {
		DWORD size;
		RegQueryValueEx(hhKey, key, NULL, NULL, NULL, &size);
		if (size) {
			retVal = new TCHAR[size];
			RegQueryValueEx(hhKey, key, NULL, NULL, (BYTE *)retVal, &size);
		}
		RegCloseKey(hKey);
	}
	if (!retVal && szDefault) retVal = _tcsdup(szDefault);
	return retVal;
}

/***********************************************************************************
* Writes string into windows register                                              *
***********************************************************************************/
BOOL RegWriteString(HKEY hKey, LPTSTR lpSubKey, LPTSTR lpValueName, LPTSTR value) {
	if (RegCreateKey(hKey, lpSubKey, &hKey) == ERROR_SUCCESS) {
		RegSetValueEx(hKey, lpValueName, NULL, REG_SZ, (BYTE *)value, (_tcslen(value) * sizeof(TCHAR))+1);
		RegCloseKey(hKey);
		return true;
	}else return false;
}


/***********************************************************************************
* Vr�t� intovou hodnotu z registr�                                                 *
***********************************************************************************/
void RegDeleteValue(HKEY hKey, LPTSTR lpSubKey, LPTSTR key) {
	HKEY hhKey;
	if (RegOpenKeyEx(hKey, lpSubKey, 0, KEY_ALL_ACCESS, &hhKey) == ERROR_SUCCESS) {
		RegDeleteValue(hhKey, key);
		RegCloseKey(hKey);
	}
}
