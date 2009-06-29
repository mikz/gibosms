#include "stdafx.h"
#include "CPhonebook.h"
#include "CSVWalker.h"

/***********************************************************************************
*                             CLASS CPhonebook                                     *
***********************************************************************************/
//----------------------------------------------------------------------------------
CPhonebookItem *CPhonebook::Get(DWORD n) {
	if (n < this->items.num) return this->items[n];
	return NULL;
}
//----------------------------------------------------------------------------------
BOOL CPhonebook::Add(char *name, char *number, char *email) {
	if (name) {
		CPhonebookItem *pi = new CPhonebookItem(name, number, email);
		return this->items.Add(pi);
	}
	return false;
}
//----------------------------------------------------------------------------------
BOOL CPhonebook::Remove(DWORD n) {
	if (n < this->items.num) {
		delete this->items[n];
		this->items.Remove(n);
		return true;
	}
	return false;
}
//----------------------------------------------------------------------------------
void CPhonebook::RemoveAll() {
	for (DWORD i = this->items.num - 1; i < this->items.num; i--)
		this->Remove(i);
}
//----------------------------------------------------------------------------------
DWORD CPhonebook::GetItemCount() {
	return this->items.num;
}
//----------------------------------------------------------------------------------
BOOL CPhonebook::Load() {
	CCSVWalker csv;
//return this->ImportFromPhone(true, false);
	if (csv.Load(this->GetPhonebookFile())) {
		if (csv.GetNumRows()) {
			do {
				this->Add(csv.GetField("name"), csv.GetField("number"), csv.GetField("email"));
			}while (csv.NextRow());
		}

		return true;
	}
	return false;
}
//----------------------------------------------------------------------------------
BOOL CPhonebook::Write() {
	CCSVWalker csv;
//return 0;
	csv.AddColumn("name");
	csv.AddColumn("number");
	csv.AddColumn("email");
	for (DWORD i = 0; i < this->items.num; i++) {
		csv.AddRow();
		csv.SetField("name", this->items[i]->GetName());
		csv.SetField("number", this->items[i]->GetNumber());
		csv.SetField("email", this->items[i]->GetEmail());
	}

	return csv.Write(this->GetPhonebookFile());
}
//----------------------------------------------------------------------------------
void CPhonebook::Sort() {
	for (DWORD i = 0; i < this->items.num; i++) {
		DWORD min = i;
		for (DWORD n = i; n < this->items.num; n++) {
			if (_stricmp(this->items[min]->GetName(), this->items[n]->GetName()) > 0) min = n;
		}
		if (min != i) {
			CPhonebookItem *tmp = this->items[i];
			this->items[i] = this->items[min];
			this->items[min] = tmp;
		}
	}
}
//----------------------------------------------------------------------------------
char *CPhonebook::GetNameByNumber(char *number) {
	for (DWORD i = 0; i < this->items.num; i++) {
		if (!strcmp(this->items[i]->GetNumber(), number))
			return this->items[i]->GetName();
	}
	return NULL;
}
//----------------------------------------------------------------------------------
char *CPhonebook::GetNameByEmail(char *email) {
	for (DWORD i = 0; i < this->items.num; i++) {
		if (!_stricmp(this->items[i]->GetEmail(), email))
			return this->items[i]->GetName();
	}
	return NULL;
}
//----------------------------------------------------------------------------------
BOOL CPhonebook::Exists(char *name) {
	for (DWORD i = 0; i < this->items.num; i++) {
		if (!_stricmp(this->items[i]->GetName(), name))
			return true;
	}
	return false;
}
//----------------------------------------------------------------------------------
DWORD CPhonebook::ImportFromNet(char *data) {
	data = _strdup(data);
	DWORD numLines;
	DWORD numImported = 0;
	char **lines = explode(data, "\r\n", &numLines, true);
	for (DWORD i = 0; i < numLines; i++) {
		char *p = strchr(lines[i], '=');
		if (p) {
			*p = '\0';
			char *number = p + 1;
			char *email = "";
			p = strchr(number, '|');
			if (p) {
				*p = '\0';
				email = p + 1;
			}

			if  (*number == '+') number += 4;
			if (!this->Exists(lines[i])) {
				numImported += this->Add(lines[i], number, email);
			}
		}
	}
	free(data);

	return numImported;
}
//----------------------------------------------------------------------------------
DWORD CPhonebook::ImportFromPhone(BOOL importNumbers, BOOL importEmails) {
	return 0;
}
/***********************************************************************************
*                           END CLASS CPhonebook                                   *
***********************************************************************************/
