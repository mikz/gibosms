#include "stdafx.h"
#include "Setting.h"
#include "CSVWalker.h"
#include "Registers.h"

/***********************************************************************************
*                              CLASS CSetting                                      *
***********************************************************************************/
//----------------------------------------------------------------------------------
char *CSetting::EncodeField(char *str) {
	size_t len = strlen(str);
	char *es = new char[len * 2 + 1];
	
	for (DWORD i = 0; i < len; i++) {
		char b[10];
		_ltoa(str[i], b, 16);
		es[i * 2 + 0] = b[1] ? b[0] : '0';
		es[i * 2 + 1] = b[1] ? b[1] : b[0];
	}
	es[len * 2] = '\0';
	
	return es;
}
//----------------------------------------------------------------------------------
char *CSetting::DecodeField(char *str) {
	size_t len = strlen(str);
	char *ds = new char[len / 2 + 1];
	
	for (DWORD i = 0; i < len / 2; i++) {
		char n1a = str[i * 2 + 0];
		char n2a = str[i * 2 + 1];
		int n1 = (n1a >= '0' && n1a <= '9') ? n1a - '0' : ((n1a >= 'a' && n1a <= 'f') ? 10 + n1a - 'a' : 0);
		int n2 = (n2a >= '0' && n2a <= '9') ? n2a - '0' : ((n2a >= 'a' && n2a <= 'f') ? 10 + n2a - 'a' : 0);
		ds[i] = n1 * 16 + n2;
	}
	ds[len / 2] = '\0';
	
	return ds;
}
//----------------------------------------------------------------------------------
void CSetting::Load() {
	CCSVWalker csv;

	csv.Load(this->GetSettingFile());
	this->lastDestNum = _strdup(csv.GetField("lastDestNum", 0, ""));
	this->srcNum = _strdup(csv.GetField("srcNum", 0, ""));
	this->pwd = _strdup(csv.GetField("pwd", 0, ""));
	this->pwd = this->DecodeField(this->pwd);
	this->msg = _strdup(csv.GetField("msg", 0, ""));
	//this->useInternalPhonebook = csv.GetFieldInt("useInternalPhonebook", 0, false);
	this->nSentMessages = csv.GetFieldInt("nSentMessages", 0, 0);
	this->sign = _strdup(csv.GetField("sign", 0, ""));
	this->lastSendWay = csv.GetFieldInt("lastSendWay", 0, 1);
	this->confirmReceiver = csv.GetFieldInt("confirmReceiver", 0, true);
	this->email = _strdup(csv.GetField("email", 0, ""));
	this->lastDestEmail = _strdup(csv.GetField("lastDestEmail", 0, ""));
	this->allowSounds = csv.GetFieldInt("allowSounds", 0, true);
	this->quitOnSuccessfullSend = csv.GetFieldInt("quitOnSuccessfullSend", 0, false);
	this->msgsReadPage = csv.GetFieldInt("msgsReadPage", 0, 3);
	this->msgsReadPassword = this->DecodeField(_strdup(csv.GetField("msgsReadPassword", 0, "")));
	this->msgsReadRememberPassword = csv.GetFieldInt("msgsReadRememberPassword", 0, false);
	this->expertMode = csv.GetFieldInt("expertMode", 0, false);
	this->winX = csv.GetFieldInt("winX", 0, 100);
	this->winY = csv.GetFieldInt("winY", 0, 100);
	this->runAfterWindowsStartup = RegGetString(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"), _T("Gibo SMS"), NULL) != NULL;
	this->hotKey = csv.GetFieldInt("hotKey", 0, 0);
	this->skinName = _strdup(csv.GetField("skinName", 0, "default"));
	this->proxy = _strdup(csv.GetField("proxy", 0, ""));
}
//----------------------------------------------------------------------------------
void CSetting::Write() {
	CCSVWalker csv;
	
	csv.AddColumn("lastDestNum");
	csv.AddColumn("srcNum");
	csv.AddColumn("pwd");
	csv.AddColumn("msg");
	//csv.AddColumn("useInternalPhonebook");
	csv.AddColumn("nSentMessages");
	csv.AddColumn("sign");
	csv.AddColumn("lastSendWay");
	csv.AddColumn("confirmReceiver");
	csv.AddColumn("email");
	csv.AddColumn("lastDestEmail");
	csv.AddColumn("allowSounds");
	csv.AddColumn("quitOnSuccessfullSend");
	csv.AddColumn("msgsReadPage");
	csv.AddColumn("msgsReadPassword");
	csv.AddColumn("msgsReadRememberPassword");
	csv.AddColumn("expertMode");
	csv.AddColumn("winX");
	csv.AddColumn("winY");
	csv.AddColumn("hotKey");
	csv.AddColumn("skinName");
	csv.AddColumn("proxy");

	csv.AddRow();

	csv.SetField("lastDestNum", this->lastDestNum);
	csv.SetField("srcNum", this->srcNum);
	csv.SetField("pwd", this->EncodeField(this->pwd));
	csv.SetField("msg", this->msg);
	//csv.SetFieldInt("useInternalPhonebook", this->useInternalPhonebook);
	csv.SetFieldInt("nSentMessages", this->nSentMessages);
	csv.SetField("sign", this->sign);
	csv.SetFieldInt("lastSendWay", this->lastSendWay);
	csv.SetFieldInt("confirmReceiver", this->confirmReceiver);
	csv.SetField("email", this->email);
	csv.SetField("lastDestEmail", this->lastDestEmail);
	csv.SetFieldInt("allowSounds", this->allowSounds);
	csv.SetFieldInt("quitOnSuccessfullSend", this->quitOnSuccessfullSend);
	csv.SetFieldInt("msgsReadPage", this->msgsReadPage);
	csv.SetField("msgsReadPassword", this->EncodeField(this->msgsReadPassword));
	csv.SetFieldInt("msgsReadRememberPassword", this->msgsReadRememberPassword);
	csv.SetFieldInt("expertMode", this->expertMode);
	csv.SetFieldInt("winX", this->winX);
	csv.SetFieldInt("winY", this->winY);
	csv.SetFieldInt("hotKey", this->hotKey);
	csv.SetField("skinName", this->skinName);
	csv.SetField("proxy", this->proxy);

	if (this->runAfterWindowsStartup) {
		TCHAR exePath[MAX_PATH + 1];
		GetModuleFileName(NULL, exePath, MAX_PATH + 1);
		
		_tcscat(exePath, _T(" -hide"));
		
		RegWriteString(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"), _T("Gibo SMS"), exePath);
	}else {
		RegDeleteValue(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"), _T("Gibo SMS"));
	}

	csv.Write(this->GetSettingFile());
}
/***********************************************************************************
*                            END CLASS CSetting                                    *
***********************************************************************************/
