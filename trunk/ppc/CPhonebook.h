#ifndef __CPHONEBOOK_H__
#define __CPHONEBOOK_H__

#include "CArray.h"
#include "SetCharPtr.h"
#include "Fce.h"


class CPhonebookItem {
private:
	char *number;
	char *name;
	char *email;

public:
	char *GetName() {return this->name ? this->name : "";}
	char *GetNumber() {return this->number ? this->number : "";}
	char *GetEmail() {return this->email ? this->email : "";}
	void SetName(char *val) {SetCharPtr(&this->name, val);};
	void SetNumber(char *val) {SetCharPtr(&this->number, val);};
	void SetEmail(char *val) {SetCharPtr(&this->email, val);};

	CPhonebookItem() {
		name = NULL;
		number = NULL;
		email = NULL;
	}
	CPhonebookItem(char *name, char *number, char *email) {
		this->name = _strdup(name);
		this->number = number ? _strdup(number) : NULL;;
		this->email = email ? _strdup(email) : NULL;
	}
	~CPhonebookItem() {
		SetName(NULL);
		SetNumber(NULL);
		SetEmail(NULL);
	}
};


class CPhonebook {
private:
	WCHAR *phonebookFile;

private:
	CArray<CPhonebookItem *> items;

public:
	CPhonebookItem *Get(DWORD n);
	BOOL Add(char *name, char *number, char *email);
	BOOL Remove(DWORD n);
	void RemoveAll();
	DWORD GetItemCount();
	BOOL Load();
	BOOL Write();
	void Sort();
	char *GetNameByNumber(char *number);
	char *GetNameByEmail(char *email);
	BOOL Exists(char *name);
	DWORD ImportFromNet(char *data);
	DWORD ImportFromPhone(BOOL importNumbers, BOOL importEmails);
	char *GetPhonebookFile() {
		static char *path = NULL;
		if (path) free(path);
		WCHAR pathW[MAX_PATH + 1] = L"";
		#ifndef DEBUG
		GetModuleFileName(NULL, pathW, MAX_PATH + 1);
		WCHAR *p;
		if (p = wcsrchr(pathW, '\\')) *(p + 1) = '\0';
		#endif
		wcscat(pathW, phonebookFile);
		path = ConvertToMultiByte(pathW, CP_ACP);
		return path;
	}

	CPhonebook() {
		phonebookFile = L"phonebook.csv";
	}
	~CPhonebook() {
	}
};
#endif //__CPHONEBOOK_H__