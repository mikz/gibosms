#include "general.h"
#include "CPhonebook.h"
#include "CSVWalker.h"
#if _WIN32_WCE < WCE_VERSION_LT_WM05
	#include <addrmapi.h>
#else
	#define INITGUID
	#include <windows.h>
	#include <pimstore.h>
	#pragma comment(lib, "pimstore.lib")
#endif


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
#if _WIN32_WCE < WCE_VERSION_LT_WM05
//----------------------------------------------------------------------------------
DWORD CPhonebook::ImportFromPhone(BOOL importNumbers, BOOL importEmails) {
	DWORD numImported = 0;

	HANDLE hDataBase = NULL;
	CEOID CeOid = 0;

	DWORD dwIndex = 1;
	LPBYTE pData = NULL;
	WORD cPropID = 1;
	DWORD cbData = 0;
	CEGUID gDBGUID;
	CeOid = NULL;

	CREATE_SYSTEMGUID(&gDBGUID);
	//if (FileExists(L"\\Storage\\mxip_initdb.vol") || 1) CeMountDBVol(&gDBGUID, TEXT("\\Storage\\mxip_initdb.vol"), OPEN_EXISTING);
	//else if (FileExists(L"\\IPSM\\mxip_initdb.vol")) CeMountDBVol(&gDBGUID, TEXT("\\IPSM\\mxip_initdb.vol"), OPEN_EXISTING);
	CeMountDBVol(&gDBGUID, TEXT("\\mxip_initdb.vol"), OPEN_EXISTING);
	CeMountDBVol(&gDBGUID, TEXT("\\pim.vol"), OPEN_EXISTING);

	hDataBase = CeOpenDatabaseEx(&gDBGUID, &CeOid, TEXT("Contacts Database"), 0, CEDB_AUTOINCREMENT, NULL);
	if (hDataBase == INVALID_HANDLE_VALUE) return 0;

	CeSeekDatabase(hDataBase, CEDB_SEEK_BEGINNING, 0, &dwIndex);

	int propNums[] = {PR_MOBILE_TELEPHONE_NUMBER_W, PR_HOME_TELEPHONE_NUMBER_W, PR_BUSINESS_TELEPHONE_NUMBER_W, PR_HOME2_TELEPHONE_NUMBER_W, HHPR_OFFICE2_TELEPHONE_NUMBER};
	int propEmails[] = {HHPR_EMAIL1_EMAIL_ADDRESS , HHPR_EMAIL2_EMAIL_ADDRESS , HHPR_EMAIL3_EMAIL_ADDRESS };

	while (CeReadRecordPropsEx(hDataBase, CEDB_ALLOWREALLOC, &cPropID, NULL, &pData, &cbData, NULL)) {
		CEPROPVAL *fields = (CEPROPVAL*)pData;
		char *name = NULL;
		CArray<char *> values;
		DWORD i;
		for (i = 0; i < cPropID; i++) {
			if (LOWORD(fields[i].propid) == CEVT_LPWSTR) {
				if (!name && fields[i].propid == HHPR_FILEAS) {
					name = ConvertToMultiByte(fields[i].val.lpwstr, CP_ACP);
				}

				if (importNumbers && IsInArray(propNums, sizeof(propNums) / sizeof(propNums[0]), fields[i].propid)) {
					char *val = ConvertToMultiByte(fields[i].val.lpwstr, CP_ACP);
					values.Add(val);
				}else if (importEmails && IsInArray(propEmails, sizeof(propEmails) / sizeof(propEmails[0]), fields[i].propid)) {
					char *val = ConvertToMultiByte(fields[i].val.lpwstr, CP_ACP);
					values.Add(val);
				}
			}
		}
		if (name) {
			for (i = 0; i < values.num; i++) {
				char *val = values[i];
				if (importNumbers && *val == '+') val += 4;
				char *contact = new char[strlen(name) + strlen(val) + 50];
				//sprintf(contact, "%s (%s)", name, val);
				sprintf(contact, "%s", name, val);
				numImported += this->Add(contact, importNumbers ? val : (char *)NULL, importEmails ? val : (char *)NULL);
				//MessageBox(0, ConvertToWideChar(contact, CP_ACP), L"x", 0);
				delete[] contact;
			}
			delete[] name;
			name = NULL;
		}
		for (i = 0; i < values.num; i++) {
			free(values[i]);
		}
	}
	CloseHandle(hDataBase); 
//MessageBox(0, L"konec", L"nacteno", 0);
	return numImported;
}
#else
//----------------------------------------------------------------------------------
DWORD CPhonebook::ImportFromPhone(BOOL importNumbers, BOOL importEmails)
{
	DWORD numImported = 0;

	HRESULT hr;
	IPOutlookApp * polApp;
	hr = CoInitializeEx(NULL, 0);
	if (FAILED(hr)) {
		return 0;
	}
	hr = CoCreateInstance(CLSID_Application, NULL, CLSCTX_INPROC_SERVER, IID_IPOutlookApp, (LPVOID*)&polApp);
	if (FAILED(hr)) {
		return 0;
	}
	hr = polApp->Logon(NULL);
	if (FAILED(hr)) {
		return 0;
	}
	IPOutlookItemCollection *pItems;
	IFolder *pFolder;
	hr = polApp->GetDefaultFolder(olFolderContacts, &pFolder);
	if (hr != S_OK) {
		return 0;
	}
	hr = pFolder->get_Items(&pItems);
	if (hr != S_OK) {
		return 0;
	}
	int iNum;
	hr = pItems->get_Count(&iNum);
	for (int iItem = 0; iItem < iNum; iItem++) {
		IContact *pcontact;
		if (FAILED(hr = pItems->Item(iItem + 1,(IDispatch**)&pcontact))) {
			continue;
		}
		BSTR sFileAsW, sMobileTelephoneNumberW, sHomeTelephoneNumberW, sHome2TelephoneNumberW, sBusinessTelephoneNumberW, sBusiness2TelephoneNumberW, sEmail1AddressW, sEmail2AddressW, sEmail3AddressW;
		char *sFileAsA = NULL, *sMobileTelephoneNumberA = NULL, *sHomeTelephoneNumberA = NULL, *sHome2TelephoneNumberA = NULL, *sBusinessTelephoneNumberA = NULL, *sBusiness2TelephoneNumberA = NULL, *sEmail1AddressA = NULL, *sEmail2AddressA = NULL, *sEmail3AddressA = NULL;

		// read title
		if (FAILED(hr = pcontact->get_FileAs(&sFileAsW))) {
			continue;
		}
		sFileAsA = ConvertToMultiByte(sFileAsW, CP_ACP);

		// read MobileTelephoneNumber
		if (!FAILED(hr = pcontact->get_MobileTelephoneNumber(&sMobileTelephoneNumberW)) && *sMobileTelephoneNumberW) {
			sMobileTelephoneNumberA = ConvertToMultiByte(sMobileTelephoneNumberW, CP_ACP);
			this->Add(sFileAsA, *sMobileTelephoneNumberA == '+' ? sMobileTelephoneNumberA + 4 : sMobileTelephoneNumberA, NULL);
			free(sMobileTelephoneNumberA);
		}

		// read HomeTelephoneNumber
		if (!FAILED(hr = pcontact->get_HomeTelephoneNumber(&sHomeTelephoneNumberW)) && *sHomeTelephoneNumberW) {
			sHomeTelephoneNumberA = ConvertToMultiByte(sHomeTelephoneNumberW, CP_ACP);
			this->Add(sFileAsA, *sHomeTelephoneNumberA == '+' ? sHomeTelephoneNumberA + 4 : sHomeTelephoneNumberA, NULL);
			free(sHomeTelephoneNumberA);
		}

		// read Home2TelephoneNumber
		if (!FAILED(hr = pcontact->get_Home2TelephoneNumber(&sHome2TelephoneNumberW)) && *sHome2TelephoneNumberW) {
			sHome2TelephoneNumberA = ConvertToMultiByte(sHome2TelephoneNumberW, CP_ACP);
			this->Add(sFileAsA, *sHome2TelephoneNumberA == '+' ? sHome2TelephoneNumberA + 4 : sHome2TelephoneNumberA, NULL);
			free(sHome2TelephoneNumberA);
		}

		// read BusinessTelephoneNumber
		if (!FAILED(hr = pcontact->get_BusinessTelephoneNumber(&sBusinessTelephoneNumberW)) && *sBusinessTelephoneNumberW) {
			sBusinessTelephoneNumberA = ConvertToMultiByte(sBusinessTelephoneNumberW, CP_ACP);
			this->Add(sFileAsA, *sBusinessTelephoneNumberA == '+' ? sBusinessTelephoneNumberA + 4 : sBusinessTelephoneNumberA, NULL);
			free(sBusinessTelephoneNumberA);
		}

		// read Business2TelephoneNumber
		if (!FAILED(hr = pcontact->get_Business2TelephoneNumber(&sBusiness2TelephoneNumberW)) && *sBusiness2TelephoneNumberW) {
			sBusiness2TelephoneNumberA = ConvertToMultiByte(sBusiness2TelephoneNumberW, CP_ACP);
			this->Add(sFileAsA, *sBusiness2TelephoneNumberA == '+' ? sBusiness2TelephoneNumberA + 4 : sBusiness2TelephoneNumberA, NULL);
			free(sBusiness2TelephoneNumberA);
		}

		// read Email1Address
		if (!FAILED(hr = pcontact->get_Email1Address(&sEmail1AddressW)) && *sEmail1AddressW) {
			sEmail1AddressA = ConvertToMultiByte(sEmail1AddressW, CP_ACP);
			this->Add(sFileAsA, sEmail1AddressA, NULL);
			free(sEmail1AddressA);
		}

		// read Email2Address
		if (!FAILED(hr = pcontact->get_Email2Address(&sEmail2AddressW)) && *sEmail2AddressW) {
			sEmail2AddressA = ConvertToMultiByte(sEmail2AddressW, CP_ACP);
			this->Add(sFileAsA, sEmail2AddressA, NULL);
			free(sEmail2AddressA);
		}

		// read Email3Address
		if (!FAILED(hr = pcontact->get_Email3Address(&sEmail3AddressW)) && *sEmail3AddressW) {
			sEmail3AddressA = ConvertToMultiByte(sEmail3AddressW, CP_ACP);
			this->Add(sFileAsA, sEmail3AddressA, NULL);
			free(sEmail3AddressA);
		}

		free(sFileAsA);
	}
	return numImported;
}
#endif
/***********************************************************************************
*                           END CLASS CPhonebook                                   *
***********************************************************************************/
