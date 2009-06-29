#ifndef __GIBOSMS_H__
#define __GIBOSMS_H__

#include "general.h"
#include "Setting.h"
#include "CPhonebook.h"
#include "Fce.h"
#include "CStack.h"
#include "XPhonebook.h"
#include "CSVWalker.h"


//WCHAR *version = L"1.37";

#define USER_REFRESHLIST          (WM_USER +  1)
#define USER_SELECTBYNAME         (WM_USER +  2)
#define USER_SELECTBYRECEIVER     (WM_USER +  3)
#define USER_SELECTBYNUMBER       (WM_USER +  3)
#define USER_SELECTBYEMAIL        (WM_USER +  4)
#define USER_FINISHDOWNLOADING    (WM_USER +  5)
#define USER_KEYDOWN              (WM_USER +  6)
#define USER_KEYUP                (WM_USER +  7)
#define USER_FOCUSSMILEY          (WM_USER +  8)
#define USER_GETMSG               (WM_USER +  9)
#define USER_GETSUBJECT           (WM_USER + 10)
#define USER_INSERTTEXT           (WM_USER + 11)
#define USER_INSERTSMILEY         (WM_USER + 12)



//--------------------------------------------------------------------
class CDlgAddressBook {
public:
	enum TYPE {SEND, SHOW};
	enum SENDTO {SMS = 1, EMAIL = 2, MAILBOX = 3, NORMALSMS = 4, WEBSMS = 5, MOBEMAIL = 6, PAIDSMS = 7};

public:
	TYPE type;
	SENDTO sendTo;
	WCHAR *emailSubject;

	CDlgAddressBook() {
		emailSubject = NULL;
	}
};


//--------------------------------------------------------------------
class CDlgStatus {
public:
	enum TYPE {SENDMSG, SHOWINFO, EXPORTPHONEBOOK, IMPORTPHONEBOOK_LIST, IMPORTPHONEBOOK_READ, MSGSREAD};

public:
	BOOL succeed;
	WCHAR *resultText;
	WCHAR *caption;
	TYPE type;
	BOOL showSucceedPic;
	BOOL playSound;
	UINT idMenu;
	BOOL quitCountdown;
	CArray<UINT> disabledMenuItems;
};


//--------------------------------------------------------------------
class CDlgPassword {
private:
	char *password;
	BOOL rememberPassword;

public:
	char *GetPassword() {return this->password;}
	BOOL GetRememberPassword() {return this->rememberPassword;}

	void SetPassword(char *password) {SetCharPtr(&this->password, password);}
	void SetRememberPassword(BOOL rememberPassword) {this->rememberPassword = rememberPassword;}
	

	CDlgPassword() {
		password = NULL;
		rememberPassword = false;
	}
	~CDlgPassword() {
		SetPassword(NULL);
	}
};


//--------------------------------------------------------------------
class CDlgJumpOver {
public:
	int step;
	int pageSize;

	CDlgJumpOver(int step, int pageSize) {
		this->step = step;
		this->pageSize = pageSize;
	}
	~CDlgJumpOver() {
	}
};


//--------------------------------------------------------------------
class CDlgSearch {
private:
	char *cont;
	char *search_text;
public:
	long startTime;
	long endTime;

public:
	char *GetCont() {return this->cont;}
	char *GetSearch_text() {return this->search_text;}

	void SetCont(char *cont) {SetCharPtr(&this->cont, cont);}
	void SetSearch_text(char *search_text) {SetCharPtr(&this->search_text, search_text);}
	

	CDlgSearch(char *cont, char *search_text, long startTime, long endTime) {
		cont = NULL;
		search_text = NULL;
		SetCont(cont);
		SetSearch_text(search_text);
		this->startTime = startTime;
		this->endTime = endTime;
	}
	~CDlgSearch() {
		SetCont(NULL);
		SetSearch_text(NULL);
	}
};


//--------------------------------------------------------------------
class COldProc {
public:
	CArray<HWND> hWnd;
	CArray<WNDPROC> proc;

public:
	void Add(HWND hWnd, WNDPROC proc) {
		this->hWnd.Add(hWnd);
		this->proc.Add(proc);
	}
	void Remove(HWND hWnd) {
		for (DWORD i = 0; i < this->hWnd.num; i++) {
			if (this->hWnd[i] == hWnd) {
				this->hWnd.Remove(i);
				this->proc.Remove(i);
				break;
			}
		}
	}
	void RemoveAll() {
		this->hWnd.RemoveAll();
		this->proc.RemoveAll();
	}
	WNDPROC GetProc(HWND hWnd) {
		for (DWORD i = 0; i < this->hWnd.num; i++) {
			if (this->hWnd[i] == hWnd) {
				return this->proc[i];
			}
		}
		return NULL;
	}

	COldProc() {
	}
	~COldProc() {
		RemoveAll();
	}
};


//--------------------------------------------------------------------
class CViewPage {
private:
	char *page;
	WCHAR *resultText;
	WCHAR *extras;
	BOOL succeed;
	HWND notifyHwnd;
	UINT notifyMessage;
	WPARAM notifyWParam;
	WCHAR *caption;
public:
	BOOL (* callback)(WPARAM wParam, LPARAM lParam);

public:
	char *GetPage() {return this->page;}
	WCHAR *GetResultText() {return this->resultText;}
	WCHAR *GetExtras() {return this->extras;}
	BOOL GetSucceed() {return this->succeed;}
	HWND GetNotifyHwnd() {return this->notifyHwnd;}
	UINT GetNotifyMessage() {return this->notifyMessage;}
	WPARAM GetNotifyWParam() {return this->notifyWParam;}
	WCHAR *GetCaption() {return this->caption;}

	void SetPage(char *page) {SetCharPtr(&this->page, page);}
	void SetResultText(WCHAR *resultText) {SetCharPtr(&this->resultText, resultText);}
	void SetExtras(WCHAR *extras) {SetCharPtr(&this->extras, extras);}
	void SetSucceed(BOOL succeed) {this->succeed = succeed;}
	void SetNotifyHwnd(HWND notifyHwnd) {this->notifyHwnd = notifyHwnd;}
	void SetNotifyMessage(UINT notifyMessage) {this->notifyMessage = notifyMessage;}
	void SetNotifyWParam(WPARAM notifyWParam) {this->notifyWParam = notifyWParam;}
	void SetCallback(BOOL (* callback)(WPARAM wParam, LPARAM lParam)) {this->callback = callback;}
	void SetCaption(WCHAR *caption) {SetCharPtr(&this->caption, caption);}
	
	CViewPage *Duplicate() {
		CViewPage *dupl = new CViewPage();
		dupl->SetPage(this->GetPage());
		dupl->SetResultText(this->GetResultText());
		dupl->SetExtras(this->GetExtras());
		dupl->SetSucceed(this->GetSucceed());
		dupl->SetNotifyHwnd(this->GetNotifyHwnd());
		dupl->SetNotifyMessage(this->GetNotifyMessage());
		dupl->SetNotifyWParam(this->GetNotifyWParam());
		dupl->SetCallback(this->callback);
		dupl->SetCaption(this->caption);
		return dupl;
	}
	

	CViewPage() {
		page = NULL;
		resultText = NULL;
		extras = NULL;
		succeed = false;
		notifyHwnd = NULL;
		notifyMessage = 0;
		notifyWParam = 0;
		callback = NULL;
		caption = NULL;
	}
	~CViewPage() {
		SetPage(NULL);
		SetResultText(NULL);
		SetExtras(NULL);
		SetCaption(NULL);
	}
};


//--------------------------------------------------------------------
enum DISPLAY {MAIN, SENDWAY, INSERTSMILEY, PHONEBOOK, WAIT, INFORMATION, IMPORTPHONEBOOK_LIST, LSTMSGSREAD, ENTERPICTOGRAM};

//--------------------------------------------------------------------
class CGlobalVariables {
public:
	HINSTANCE hInstance;
	WCHAR *szApp;
	WCHAR *szAppErr;
	WCHAR *szVersion;
	char *szVersionA;
	char *giboServer;
	char *client;
	int iVersion;
	HWND hMenuBarCurrent;
	HWND hMain;
	HWND hEditMsg;
	UINT idTimerCharCounter;
	UINT idTimerRefresh;
	UINT idTimerSaveUndo;
	UINT idTimerAnimStatus;
	UINT idTimerPlaysound;
	UINT idTimerCountdown;
	UINT idTimerSaveInputMode;
	int maxSMSLen;
	CSetting setting;
	CPhonebook phonebook;
	COldProc oldWndProcs;
	DWORD skipUntilTime;
	CStack<WCHAR *> textUndo;
	DISPLAY display;
	DISPLAY displayPrev;
	char *url;
	CViewPage lastRequestedPage;
	BOOL countdownTime;
	HBRUSH hbrBackground;

	char *sendMsgReceiver;
	CDlgAddressBook::SENDTO sendMsgSendTo;
	WCHAR *sendMsgEmailSubject;
	char *sendMsgMessageId;
	char *sendMsgPictogram;

	char *msgsReadType;
	char *msgsReadPassword;
	BOOL msgsReadCorrectPasswordSubmitted;
	char *msgsReadCont;
	char *msgsReadSearch_text;
	int msgsReadPos;
	int msgsReadPage;
	long msgsReadStartTime, msgsReadEndTime;
	int msgsReadMsg_0_start, msgsReadMsg_0_length;

	HDC screenHdc;

	void AllocateUrl(DWORD size) {
		if (url) delete[] url;
		url = new char[size];
	}


	CGlobalVariables() {
		hInstance = NULL;
		szApp = L"GiboSMS";
		szAppErr = L"GiboSMS - Error";
		szVersion = L"2.05";
		szVersionA = ConvertToMultiByte(szVersion, CP_ACP);
		giboServer = "http://www.gibosms.cz";
		//giboServer = "http://sms.dynawest.cz";
		//giboServer = "http://localhost/sms";
		client = "smartphone";
		iVersion = (szVersion[0] - L'0') * 100 + (szVersion[2] - L'0') * 10 + (szVersion[3] - L'0');
		hMenuBarCurrent = NULL;
		hMain = NULL;
		hEditMsg = NULL;
		idTimerCharCounter = 0x1;
		idTimerRefresh = 0x2;
		idTimerSaveUndo = 0x3;
		idTimerAnimStatus = 0x4;
		idTimerPlaysound = 0x5;
		idTimerCountdown = 0x6;
		idTimerSaveInputMode = 0x7;
		maxSMSLen = 760;
		skipUntilTime = 0;
		display = MAIN;
		displayPrev = display;
		url = NULL;
		countdownTime = 60;
		hbrBackground = NULL;

		sendMsgReceiver = NULL;
		sendMsgSendTo = CDlgAddressBook::SENDTO::SMS;
		sendMsgEmailSubject = NULL;
		sendMsgMessageId = NULL;
		sendMsgPictogram = NULL;

		msgsReadType = _strdup("");
		msgsReadPassword = _strdup("");
		msgsReadCorrectPasswordSubmitted = false;
		msgsReadCont = _strdup("");
		msgsReadSearch_text = _strdup("");
		msgsReadPos = 0;
		msgsReadPage = 3;
		msgsReadStartTime = 0; msgsReadEndTime = 0;
		msgsReadMsg_0_start = 0; msgsReadMsg_0_length = 0;

		setting.Load();
		phonebook.Load();
		phonebook.Sort();

		//phonebook.Add("_me", "777643925", "jkyri@volny.cz"); phonebook.Add("Nasrat", "123", "nevim");

		this->screenHdc = GetDC(0);
	}
	~CGlobalVariables() {
		setting.Write();
		phonebook.Write();
		ReleaseDC(0, this->screenHdc);
	}
};


//-------------------------------------------------------------------------------
HWND CreateMainWindow();
LRESULT WndMainProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
void DisplayDlg(DISPLAY form, LPARAM lParam, BOOL dontStoreAsPrevious);

LRESULT DlgMainProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL InitDlgMain(HWND hWnd);

LRESULT DlgWaitProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL InitDlgWait(HWND hWnd, BOOL allowCancel);

BOOL CALLBACK DlgSettingProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL InitDlgSetting(HWND hWnd);

BOOL CALLBACK DlgContactProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL InitDlgContact(HWND hWnd, WCHAR *title);

BOOL CALLBACK DlgStatusProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL InitDlgStatus(HWND hWnd, WCHAR *caption, UINT idMenu);

LRESULT CALLBACK DlgInsertSmileyProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL InitDlgInsertSmiley(HWND hWnd);

LRESULT DlgInformationProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL InitDlgInformation(HWND hWnd);

BOOL CALLBACK DlgEmailSubjectProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL InitDlgEmailSubject(HWND hWnd);

LRESULT DlgImportPhonebookProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL InitDlgImportPhonebook(HWND hWnd);

LRESULT DlgPhonebookProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL InitDlgPhonebook(HWND hWnd, CDlgAddressBook::TYPE type);

LRESULT DlgSendWayProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL InitDlgSendWay(HWND hWnd);

LRESULT DlgLstMsgsReadProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL InitDlgLstMsgsRead(HWND hWnd);

BOOL CALLBACK DlgPasswordProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL InitDlgPassword(HWND hWnd);

BOOL CALLBACK DlgJumpOverProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL InitDlgJumpOver(HWND hWnd);

BOOL CALLBACK DlgSearchProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL InitDlgSearch(HWND hWnd);

LRESULT DlgEnterPictogramProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL InitDlgEnterPictogram(HWND hWnd);


DWORD WINAPI ThreadOpenPage(LPVOID lParam);
DWORD OpenPageByThread(char *url, WCHAR *caption, HWND notifyHwnd, UINT notifyMessage, WPARAM wParam, BOOL (* callback)(WPARAM wParam, LPARAM lParam));
DWORD OpenLastPageByThread();
BOOL ContinueWithSending(WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK CtrlListViewProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
void DelayKeyboard();
void HandleWindowScroll(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
void SetScroller(HWND hWnd);
BOOL CreateItem(CHdc *hdc, HBITMAP bmpIcon, HBITMAP bmpTile);
void ShowMsgsRead();
void DrawBackgroundLogo(CHdc *hdcBackground);
void SendSMS(BOOL send_retry);
HBRUSH CreateBrushFromHDC(HDC hdc, int wid, int hei);

#endif //__GIBOSMS_H__