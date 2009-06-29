#pragma once
#include "resource.h"
#include "CHdc.h"
#include "Fce.h"
#include "CStack.h"
#include "CArray.h"
#include "Setting.h"
#include "CPhonebook.h"
#include "XPhonebook.h"
#include "CSVWalker.h"
#include "Mmsystem.h"
#include "commctrl.h"
#include <stdio.h>


#define TRAY_CALLBACK_ID                             1
#define TRAY_CALLBACK_MSG                 (WM_USER + 1)
#define HOTKEY_ID                                    1

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
	class TYPE {
	public:
		enum _ENUM {SEND, SHOW};
	};
	class SENDTO {
	public:
		enum _ENUM {SMS = 1, EMAIL = 2, MAILBOX = 3, NORMALSMS = 4, WEBSMS = 5, MOBEMAIL = 6, PAIDSMS = 7};
	};
	//enum TYPE {SEND, SHOW};
	//enum SENDTO {SMS = 1, EMAIL = 2, MAILBOX = 3, NORMALSMS = 4, WEBSMS = 5, MOBEMAIL = 6, PAIDSMS = 7};

public:
	TYPE::_ENUM type;
	SENDTO::_ENUM sendTo;
	WCHAR *emailSubject;

	CDlgAddressBook() {
		emailSubject = NULL;
	}
};


//--------------------------------------------------------------------
class CDlgStatus {
public:
	class TYPE {
	public:
		enum _ENUM {SENDMSG, SHOWINFO, EXPORTPHONEBOOK, IMPORTPHONEBOOK_LIST, IMPORTPHONEBOOK_READ, MSGSREAD};
	};
	//enum TYPE {SENDMSG, SHOWINFO, EXPORTPHONEBOOK, IMPORTPHONEBOOK_LIST, IMPORTPHONEBOOK_READ, MSGSREAD};

public:
	BOOL succeed;
	WCHAR *resultText;
	WCHAR *caption;
	TYPE::_ENUM type;
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
class DISPLAY {
public:
	enum _ENUM {MAIN, SENDWAY, INSERTSMILEY, PHONEBOOK, WAIT, INFORMATION, IMPORTPHONEBOOK_LIST, LSTMSGSREAD, ENTERPICTOGRAM};
};
//enum DISPLAY {MAIN, SENDWAY, INSERTSMILEY, PHONEBOOK, WAIT, INFORMATION, IMPORTPHONEBOOK_LIST, LSTMSGSREAD, ENTERPICTOGRAM};


// global variables (stored in class)
char *p;                                                     // auxiliary pointer variable
class GlobalVariables {
public:
	LPTSTR szAppName;                                          // name of application
	HINSTANCE hInst;                                           // instance of application
	HWND hWndMain;                                             // handle to main window
	HWND hWndMainInside;                                       // handle to main inside window
	
	CSetting setting;

	HCURSOR hArrow;                                            // kurzor šipkya
	HCURSOR hHand;                                             // kurzor ruka
	HCURSOR hMove;                                             // kurzor pohyb
	HCURSOR hText;                                             // kurzor text
	
	LPTSTR szTitle;                                            // titulek
	LPTSTR szMenuLeft;                                         // menu levé tlaèítko
	LPTSTR szMenuRight;                                        // menu pravé tlaèítko
	
	BOOL buttonMenuLeftPushed;                                 // zda-li je levé tlaèítko menu pushed
	BOOL buttonMenuRightPushed;                                // zda-li je pravé tlaèítko menu pushed
	
	HHOOK hKeyboardHook;
	
	BOOL popupShown;
	UINT idTimerUninitMenu;

	
	WCHAR *szAppErr;
	WCHAR *szVersion;
	char *szVersionA;
	TCHAR szAppDir[MAX_PATH + 1];
	char *giboServer;
	char *client;
	int iVersion;
	HWND hMenuBarCurrent;
	HWND hMain;
	HWND hEditMsg;
	HWND hMenuHandler;
	HWND hDisplayedDialog;
	LPTSTR lpMenuName;
	UINT idTimerCharCounter;
	UINT idTimerRefresh;
	UINT idTimerSaveUndo;
	UINT idTimerAnimStatus;
	UINT idTimerPlaysound;
	UINT idTimerCountdown;
	UINT idTimerCheckSetting;
	int maxSMSLen;
	CPhonebook phonebook;
	COldProc oldWndProcs;
	DWORD skipUntilTime;
	CStack<WCHAR *> textUndo;
	DISPLAY::_ENUM display;
	DISPLAY::_ENUM displayPrev;
	char *url;
	CViewPage lastRequestedPage;
	BOOL countdownTime;
	HBRUSH hbrBackground;

	char *sendMsgReceiver;
	CDlgAddressBook::SENDTO::_ENUM sendMsgSendTo;
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

	void AllocateUrl(DWORD size) {
		if (url) delete[] url;
		url = new char[size];
	}
	
	void SetTitle(TCHAR *val) {SetCharPtr(&this->szTitle, val);}
	void SetMenuLeft(TCHAR *val) {SetCharPtr(&this->szMenuLeft, val);}
	void SetMenuRight(TCHAR *val) {SetCharPtr(&this->szMenuRight, val);}


	GlobalVariables() {
		this->szAppName = _T("Gibo SMS");
		GetModuleFileName(NULL, this->szAppDir, MAX_PATH + 1);
		TCHAR *p = _tcsrchr(this->szAppDir, _T('\\'));
		if (*p) *p = _T('\0');
		_tcscat(this->szAppDir, _T("\\"));
		this->hInst = NULL;
		this->hWndMain = NULL;
		this->hWndMainInside = NULL;

		this->hArrow = LoadCursor(NULL, IDC_ARROW);
		this->hHand = LoadCursor(NULL, IDC_HAND);
		this->hMove = LoadCursor(NULL, IDC_SIZEALL);
		this->hText = LoadCursor(NULL, IDC_IBEAM);
		this->szTitle = NULL;
		this->szMenuLeft = NULL;
		this->szMenuRight = NULL;

		this->buttonMenuLeftPushed = false;
		this->buttonMenuRightPushed = false;
		
		this->hKeyboardHook = NULL;
		
		this->popupShown = false;
		this->idTimerUninitMenu = 0x7;

		this->szVersion = L"1.03";
		this->szVersionA = ConvertToMultiByte(this->szVersion, CP_ACP);
		this->giboServer = "http://www.gibosms.cz";
		//this->giboServer = "http://localhost/sms";
		this->client = "pc";
		this->iVersion = (this->szVersion[0] - L'0') * 100 + (this->szVersion[2] - L'0') * 10 + (this->szVersion[3] - L'0');
		this->hMenuBarCurrent = NULL;
		this->hMain = NULL;
		this->hEditMsg = NULL;
		this->hMenuHandler = NULL;
		this->hDisplayedDialog = NULL;
		this->lpMenuName = NULL;
		this->idTimerCharCounter = 0x1;
		this->idTimerRefresh = 0x2;
		this->idTimerSaveUndo = 0x3;
		this->idTimerAnimStatus = 0x4;
		this->idTimerPlaysound = 0x5;
		this->idTimerCountdown = 0x6;
		this->idTimerCheckSetting = 0x8;
		this->maxSMSLen = 760;
		this->skipUntilTime = 0;
		this->display = DISPLAY::MAIN;
		this->displayPrev = display;
		this->url = NULL;
		this->countdownTime = 60;
		this->hbrBackground = NULL;

		this->sendMsgReceiver = NULL;
		this->sendMsgSendTo = CDlgAddressBook::SENDTO::SMS;
		this->sendMsgEmailSubject = NULL;
		this->sendMsgMessageId = NULL;
		this->sendMsgPictogram = NULL;

		this->msgsReadType = _strdup("");
		this->msgsReadPassword = _strdup("");
		this->msgsReadCorrectPasswordSubmitted = false;
		this->msgsReadCont = _strdup("");
		this->msgsReadSearch_text = _strdup("");
		this->msgsReadPos = 0;
		this->msgsReadPage = 3;
		this->msgsReadStartTime = 0; this->msgsReadEndTime = 0;
		this->msgsReadMsg_0_start = 0; this->msgsReadMsg_0_length = 0;

		this->setting.Load();

		this->phonebook.Load();
		this->phonebook.Sort();
	}
	~GlobalVariables() {
		this->setting.Write();
		this->phonebook.Write(); 
	}
};

class Skin{
public:
	RECT buttonClose;
	RECT display;
	RECT displayInside;
	RECT buttonMenuLeft;
	RECT buttonMenuRight;
	RECT title;

	COLORREF clrTransparent;	
	COLORREF clrTitle;
	HFONT fntTitle;
	COLORREF clrMenu;
	COLORREF clrMenuPushed;
	HFONT fntMenu;

	CHdc background;                                           // bitmap with background
	CHdc stage;                                                // output stage
	
	int transparency;
	
	HICON hAppIcon;                                            // ikona programu

public:
	Skin() : background(false), stage(false) {
		this->hAppIcon = NULL;
	}
	~Skin() {
	}
};


// functions
int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow);
void CreateApplicationMainWindow();
LRESULT WndApplicationMainProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
void CreateMainInsideWindow();
LRESULT WndMainInsideProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
void RepaintWindow(BOOL repaintOnlyTitle = false);
void DrawTitle(LPTSTR text);
void DrawMenuBar(LPTSTR buttonLeft, BOOL buttonLeftPushed, LPTSTR buttonRight, BOOL buttonRightPushed);

HWND CreateMainWindow();
LRESULT WndMainProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
void DisplayDlg(DISPLAY::_ENUM form, LPARAM lParam, BOOL dontStoreAsPrevious);

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
BOOL InitDlgPhonebook(HWND hWnd, CDlgAddressBook::TYPE::_ENUM type);

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
LRESULT CALLBACK CtrlEditProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
void DelayKeyboard();
void HandleWindowScroll(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
void SetScroller(HWND hWnd);
BOOL CreateItem(CHdc *hdc, HBITMAP bmpIcon, HBITMAP bmpTile, int width);
void ShowMsgsRead();
void DrawBackgroundLogo(CHdc *hdcBackground);
void SendSMS(BOOL send_retry);
HBRUSH CreateBrushFromHDC(HDC hdc, int wid, int hei);
void CreateMenu(HWND hWndHandler, LPTSTR lpMenuName);
void DisplayDialog(HWND hWnd);
BOOL RegisterApplicationHotKey(UINT modifiers, BYTE vk);
LRESULT CALLBACK KeyboardProc(int code, WPARAM wParam, LPARAM lParam);
void DlgMenuHandler(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL LoadSkin(LPTSTR szSkinName);