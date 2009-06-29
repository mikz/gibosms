#ifndef __SETTING_H__
#define __SETTING_H__

#include "SetCharPtr.h"
#include "Fce.h"


class CSetting {
private:
	WCHAR *settingFile;

public:
	char *lastDestNum;
	char *srcNum;
	char *pwd;
	char *msg;
	BOOL useInternalPhonebook;
	int nSentMessages;
	char *sign;
	BOOL confirmReceiver;
	char *email;
	int lastSendWay;
	char *lastDestEmail;
	BOOL allowSounds;
	BOOL quitOnSuccessfullSend;
	int inputMode;
	int msgsReadPage;
	char *msgsReadPassword;
	BOOL msgsReadRememberPassword;
	BOOL expertMode;
	int winX;
	int winY;
	BOOL runAfterWindowsStartup;
	DWORD hotKey;
	char *skinName;
	char *proxy;


public:
	char *EncodeField(char *str);
	char *DecodeField(char *str);

	void Load();
	void Write();

	void SetLastDestNum(char *val) {SetCharPtr(&this->lastDestNum, val);}
	void SetSrcNum(char *val) {SetCharPtr(&this->srcNum, val);}
	void SetPwd(char *val) {SetCharPtr(&this->pwd, val);}
	void SetMsg(char *val) {SetCharPtr(&this->msg, val);}
	void SetSign(char *val) {SetCharPtr(&this->sign, val);}
	void SetEmail(char *val) {SetCharPtr(&this->email, val);}
	void SetLastDestEmail(char *val) {SetCharPtr(&this->lastDestEmail, val);}
	void SetMsgsReadPassword(char *val) {SetCharPtr(&this->msgsReadPassword, val);}
	void SetSkinName(char *val) {SetCharPtr(&this->skinName, val);}
	void SetProxy(char *val) {SetCharPtr(&this->proxy, val);}

	char *GetLastDestNum() {return this->lastDestNum ? this->lastDestNum : "";}
	char *GetSrcNum() {return this->srcNum ? this->srcNum : "";}
	char *GetPwd() {return this->pwd ? this->pwd : "";}
	char *GetMsg() {return this->msg ? this->msg : "";}
	char *GetSign() {return this->sign ? this->sign : "";}
	char *GetEmail() {return this->email ? this->email : "";}
	char *GetLastDestEmail() {return this->lastDestEmail ? this->lastDestEmail : "";}
	char *GetMsgsReadPassword() {return this->msgsReadPassword ? this->msgsReadPassword : "";}
	char *GetSkinName() {return this->skinName ? this->skinName : "";}
	char *GetProxy() {return this->proxy? this->proxy: "";}
	WCHAR *GetSkinNameW() {
		return ConvertToWideChar(this->GetSkinName(), CP_ACP);
	}
	char *GetSettingFile() {
		static char *path = NULL;
		if (path) free(path);
		WCHAR pathW[MAX_PATH + 1] = L"";
		#ifndef DEBUG
		GetModuleFileName(NULL, pathW, MAX_PATH + 1);
		WCHAR *p;
		if (p = wcsrchr(pathW, '\\')) *(p + 1) = '\0';
		#endif
		wcscat(pathW, settingFile);
		path = ConvertToMultiByte(pathW, CP_ACP);
		return path;
	}

	CSetting() {
		settingFile = L"setting";

		lastDestNum = 0;
		srcNum = 0;
		pwd = NULL;
		msg = NULL;
		useInternalPhonebook = true;
		nSentMessages = 0;
		sign = NULL;
		lastSendWay = 1;
		confirmReceiver = false;
		email = NULL;
		lastDestEmail = NULL;
		allowSounds = true;
		quitOnSuccessfullSend = false;
		msgsReadPage = 3;
		msgsReadPassword = NULL;
		msgsReadRememberPassword = false;
		expertMode = false;
		winX = 100;
		winY = 100;
		runAfterWindowsStartup = true;
		hotKey = 0;
		skinName = NULL;
		proxy = NULL;
	}
	~CSetting() {
		SetPwd(NULL);
		SetSign(NULL);
		SetEmail(NULL);
		SetLastDestEmail(NULL);
		SetSkinName(NULL);
		SetProxy(NULL);
	}
};

#endif //__SETTING_H__