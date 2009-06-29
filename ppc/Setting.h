#ifndef __SETTING_H__
#define __SETTING_H__

#include <windows.h>
#include "general.h"
#if _WIN32_WCE < WCE_VERSION_LT_WM05
	#include <tpcuser.h>
#else
	#include <winuserm.h>
#endif
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


public:
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

	char *GetLastDestNum() {return this->lastDestNum ? this->lastDestNum : "";}
	char *GetSrcNum() {return this->srcNum ? this->srcNum : "";}
	char *GetPwd() {return this->pwd ? this->pwd : "";}
	char *GetMsg() {return this->msg ? this->msg : "";}
	char *GetSign() {return this->sign ? this->sign : "";}
	char *GetEmail() {return this->email ? this->email : "";}
	char *GetLastDestEmail() {return this->lastDestEmail ? this->lastDestEmail : "";}
	char *GetMsgsReadPassword() {return this->msgsReadPassword ? this->msgsReadPassword : "";}
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
		useInternalPhonebook = false;
		nSentMessages = 0;
		sign = NULL;
		lastSendWay = 1;
		confirmReceiver = false;
		email = NULL;
		lastDestEmail = NULL;
		allowSounds = true;
		quitOnSuccessfullSend = false;
		inputMode = EIM_TEXT;
		msgsReadPage = 3;
		msgsReadPassword = NULL;
		msgsReadRememberPassword = false;
		expertMode = false;
	}
	~CSetting() {
		SetPwd(NULL);
		SetSign(NULL);
		SetEmail(NULL);
		SetLastDestEmail(NULL);
	}
};

#endif //__SETTING_H__