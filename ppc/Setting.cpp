#include "Setting.h"
#include "CSVWalker.h"

/***********************************************************************************
*                              CLASS CSetting                                      *
***********************************************************************************/
//----------------------------------------------------------------------------------
void CSetting::Load() {
	CCSVWalker csv;

	csv.Load(this->GetSettingFile());
	this->lastDestNum = _strdup(csv.GetField("lastDestNum", 0, ""));
	this->srcNum = _strdup(csv.GetField("srcNum", 0, ""));
	this->pwd = _strdup(csv.GetField("pwd", 0, ""));
	this->msg = _strdup(csv.GetField("msg", 0, ""));
	this->useInternalPhonebook = csv.GetFieldInt("useInternalPhonebook", 0, false);
	this->nSentMessages = csv.GetFieldInt("nSentMessages", 0, 0);
	this->sign = _strdup(csv.GetField("sign", 0, ""));
	this->lastSendWay = csv.GetFieldInt("lastSendWay", 0, 1);
	this->confirmReceiver = csv.GetFieldInt("confirmReceiver", 0, true);
	this->email = _strdup(csv.GetField("email", 0, ""));
	this->lastDestEmail = _strdup(csv.GetField("lastDestEmail", 0, ""));
	this->allowSounds = csv.GetFieldInt("allowSounds", 0, true);
	this->quitOnSuccessfullSend = csv.GetFieldInt("quitOnSuccessfullSend", 0, false);
	this->inputMode = csv.GetFieldInt("inputMode", 0, EIM_TEXT);
	this->msgsReadPage = csv.GetFieldInt("msgsReadPage", 0, 3);
	this->msgsReadPassword = _strdup(csv.GetField("msgsReadPassword", 0, ""));
	this->msgsReadRememberPassword = csv.GetFieldInt("msgsReadRememberPassword", 0, false);
	this->expertMode = csv.GetFieldInt("expertMode", 0, false);
}
//----------------------------------------------------------------------------------
void CSetting::Write() {
	CCSVWalker csv;
	
	csv.AddColumn("lastDestNum");
	csv.AddColumn("srcNum");
	csv.AddColumn("pwd");
	csv.AddColumn("msg");
	csv.AddColumn("useInternalPhonebook");
	csv.AddColumn("nSentMessages");
	csv.AddColumn("sign");
	csv.AddColumn("lastSendWay");
	csv.AddColumn("confirmReceiver");
	csv.AddColumn("email");
	csv.AddColumn("lastDestEmail");
	csv.AddColumn("allowSounds");
	csv.AddColumn("quitOnSuccessfullSend");
	csv.AddColumn("inputMode");
	csv.AddColumn("msgsReadPage");
	csv.AddColumn("msgsReadPassword");
	csv.AddColumn("msgsReadRememberPassword");
	csv.AddColumn("expertMode");

	csv.AddRow();

	csv.SetField("lastDestNum", this->lastDestNum);
	csv.SetField("srcNum", this->srcNum);
	csv.SetField("pwd", this->pwd);
	csv.SetField("msg", this->msg);
	csv.SetFieldInt("useInternalPhonebook", this->useInternalPhonebook);
	csv.SetFieldInt("nSentMessages", this->nSentMessages);
	csv.SetField("sign", this->sign);
	csv.SetFieldInt("lastSendWay", this->lastSendWay);
	csv.SetFieldInt("confirmReceiver", this->confirmReceiver);
	csv.SetField("email", this->email);
	csv.SetField("lastDestEmail", this->lastDestEmail);
	csv.SetFieldInt("allowSounds", this->allowSounds);
	csv.SetFieldInt("quitOnSuccessfullSend", this->quitOnSuccessfullSend);
	csv.SetFieldInt("inputMode", this->inputMode);
	csv.SetFieldInt("msgsReadPage", this->msgsReadPage);
	csv.SetField("msgsReadPassword", this->msgsReadPassword);
	csv.SetFieldInt("msgsReadRememberPassword", this->msgsReadRememberPassword);
	csv.SetFieldInt("expertMode", this->expertMode);

	csv.Write(this->GetSettingFile());
}
/***********************************************************************************
*                            END CLASS CSetting                                    *
***********************************************************************************/
