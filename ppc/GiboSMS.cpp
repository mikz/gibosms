//#define PPC


#include <windows.h>
#include <aygshell.h>
#include "general.h"
#if _WIN32_WCE < WCE_VERSION_LT_WM05
	#include <tpcuser.h>
#else
	#include <winuserm.h>
#endif
#ifndef PPC
#include <tpcshell.h>
#endif
#include "resource.h"
#include "GiboSMS.h"
#include "CHdc.h"
#include "CFont.h"

// global variables
CGlobalVariables g;


/***********************************************************************************
* Program beginning                                                                *
***********************************************************************************/
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int CmdShow) {
	HWND hPreviousGiboSMS;
	if (hPreviousGiboSMS = FindWindow(L"clsGiboSMS", NULL)) {
		SetForegroundWindow(hPreviousGiboSMS);
		SetActiveWindow(hPreviousGiboSMS);
		return 0;
	}
	
	g.hInstance = hInstance;

	g.hMain = CreateMainWindow();
	ShowWindow(g.hMain, SW_SHOWNORMAL);
	
	DisplayDlg(MAIN, 1, false);
	//DisplayDlg(WAIT, true, false);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;

	CDlgStatus ds;
	ds.caption = L"Status";
	ds.resultText = L"Nasrat a rozmazat";
	ds.resultText = L"Gibo SMS je aplikace naprogramovaná v Javì pro mobilní telefony. Využívá technologie GPRS a bezplatných internetových SMS bran. Z toho vyplývájí její možnosti. Aplikace spuštìná na mobilním telefonu se pøipojí pøes GPRS na server, který pøes patøiènou SMS bránu odešle SMS.";
	ds.succeed = true;
	ds.type = CDlgStatus::TYPE::SHOWINFO;
	ds.showSucceedPic = true;
	ds.idMenu = IDR_MENU_STATUS_BACKEND;
	ds.quitCountdown = false;
	// show status dialog
	//DialogBoxParam(g.hInstance, MAKEINTRESOURCE(IDD_STATUS), 0, (DLGPROC)DlgStatusProc, (LPARAM)&ds);
	ds.resultText = L"Nasrat a rozmazat";
	//return DialogBoxParam(g.hInstance, MAKEINTRESOURCE(IDD_STATUS), 0, (DLGPROC)DlgStatusProc, (LPARAM)&ds);

	/*HWND h = CreateDlgWait(true);
	ShowWindow(h, SW_SHOW);
	UpdateWindow(h);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;*/

	//CDlgAddressBook ab;
	//ab.type = CDlgAddressBook::TYPE::SHOW;
	//return DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_ADDRESSBOOK), 0, (DLGPROC)DlgAddressBookProc, (LPARAM)&ab);
	//return DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_WAIT), 0, (DLGPROC)DlgWaitProc, 1);
	return DialogBox(hInstance, MAKEINTRESOURCE(IDD_MAIN), 0, (DLGPROC)DlgMainProc);
	//return DialogBox(hInstance, MAKEINTRESOURCE(IDD_SETTING), 0, (DLGPROC)DlgSettingProc);
	//return DialogBox(hInstance, MAKEINTRESOURCE(IDD_CONTACT), 0, (DLGPROC)DlgContactProc);
}


/***********************************************************************************
* Creates wait dialog                                                              *
***********************************************************************************/
HWND CreateMainWindow() {
	WNDCLASS wc;

	ZeroMemory(&wc, sizeof(wc));
	wc.style = CS_DBLCLKS;
	wc.lpfnWndProc = (WNDPROC)WndMainProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hIcon = LoadIcon(g.hInstance, (LPTSTR)IDI_ICON);
	wc.hInstance = g.hInstance;
	wc.hCursor = NULL;
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = L"clsGiboSMS";
	RegisterClass(&wc);

	// Create the main window.
	HWND h = CreateWindow(wc.lpszClassName, g.szApp,
					WS_CLIPCHILDREN | CS_DBLCLKS, // Setting this to 0 gives a default style we don't want.  Use a benign style bit instead.
					CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
					NULL, NULL, g.hInstance, NULL);

	return h;
}


/***********************************************************************************
* Dlg message handler                                                              *
***********************************************************************************/
LRESULT WndMainProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam) {
	if (msg == WM_DESTROY) PostQuitMessage(0);

	switch (g.display) {
		case MAIN: return DlgMainProc(hDlg, msg, wParam, lParam);
		case SENDWAY: return DlgSendWayProc(hDlg, msg, wParam, lParam);
		case INSERTSMILEY: return DlgInsertSmileyProc(hDlg, msg, wParam, lParam);
		case PHONEBOOK: return DlgPhonebookProc(hDlg, msg, wParam, lParam);
		case WAIT: return DlgWaitProc(hDlg, msg, wParam, lParam);
		case LSTMSGSREAD: return DlgLstMsgsReadProc(hDlg, msg, wParam, lParam);
		case INFORMATION: return DlgInformationProc(hDlg, msg, wParam, lParam);
		case IMPORTPHONEBOOK_LIST: return DlgImportPhonebookProc(hDlg, msg, wParam, lParam);
		case ENTERPICTOGRAM: return DlgEnterPictogramProc(hDlg, msg, wParam, lParam);
	}
	return DefWindowProc(hDlg, msg, wParam, lParam);
}


/***********************************************************************************
* Displays the form                                                                *
***********************************************************************************/
void DisplayDlg(DISPLAY form, LPARAM lParam, BOOL dontStoreAsPrevious) {
	static BOOL lastDontStoreAsPrevious = false;
	if (!lastDontStoreAsPrevious) g.displayPrev = g.display;
	lastDontStoreAsPrevious = dontStoreAsPrevious;
	g.display = form;

	switch (form) {
		case MAIN: {
			if (!g.hEditMsg) {
				/*
				g.hEditMsg = CreateWindow(L"EDIT", L"",
					1350631492, // Setting this to 0 gives a default style we don't want.  Use a benign style bit instead.
					CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
					g.hMain, NULL, g.hInstance, NULL );
				*/
				g.hEditMsg = CreateWindow(L"EDIT", L"",
					1350631492 | WS_VSCROLL, // Setting this to 0 gives a default style we don't want.  Use a benign style bit instead.
					CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
					g.hMain, NULL, g.hInstance, NULL );
			}
			ShowWindow(g.hEditMsg, SW_SHOW);
			SetFocus(g.hEditMsg);

			SendMessage(g.hMain, WM_INITDIALOG, 0, lParam);
		}break; // end case MAIN

		case SENDWAY: {
			ShowWindow(g.hEditMsg, SW_HIDE);

			SendMessage(g.hMain, WM_INITDIALOG, 0, lParam);
		}break; // end case SENDWAY

		case INSERTSMILEY: {
			ShowWindow(g.hEditMsg, SW_HIDE);

			SendMessage(g.hMain, WM_INITDIALOG, 0, lParam);
		}break; // end case INSERTSMILEY

		case PHONEBOOK: {
			ShowWindow(g.hEditMsg, SW_HIDE);

			SendMessage(g.hMain, WM_INITDIALOG, 0, lParam);
		}break; // end case PHONEBOOK
		
		case WAIT: {
			ShowWindow(g.hEditMsg, SW_HIDE);

			SendMessage(g.hMain, WM_INITDIALOG, 0, lParam);
		}break; // end case WAIT

		case INFORMATION: {
			ShowWindow(g.hEditMsg, SW_HIDE);

			SendMessage(g.hMain, WM_INITDIALOG, 0, lParam);
		}break; // end case WAIT

		case IMPORTPHONEBOOK_LIST: {
			ShowWindow(g.hEditMsg, SW_HIDE);

			SendMessage(g.hMain, WM_INITDIALOG, 0, lParam);
		}break; // end case WAIT

		case LSTMSGSREAD: {
			ShowWindow(g.hEditMsg, SW_HIDE);

			SendMessage(g.hMain, WM_INITDIALOG, 0, lParam);
		}break; // end case INSERTSMILEY

		case ENTERPICTOGRAM: {
			ShowWindow(g.hEditMsg, SW_HIDE);

			SendMessage(g.hMain, WM_INITDIALOG, 0, lParam);
		}break; // end case ENTERPICTOGRAM
	}
}


/***********************************************************************************
* Dlg message handler                                                              *
***********************************************************************************/
LRESULT DlgMainProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
		case WM_INITDIALOG: {
			if (!InitDlgMain(hDlg)) {
				DestroyWindow(hDlg);
			}else {
				if (lParam) {
					LOGFONT fn;
					memset(&fn, 0, sizeof(fn));
					fn.lfHeight = ComputeFontSize(g.hEditMsg, 12);
					fn.lfWidth = 0;
					fn.lfEscapement = 0;
					fn.lfOrientation = 0;
					fn.lfWeight = FW_BOLD;
					fn.lfItalic = false;
					fn.lfUnderline = false;
					fn.lfStrikeOut = false;
					fn.lfCharSet = EASTEUROPE_CHARSET;
					fn.lfOutPrecision = OUT_DEFAULT_PRECIS;
					fn.lfClipPrecision = CLIP_DEFAULT_PRECIS;
					fn.lfQuality = DEFAULT_QUALITY;
					fn.lfPitchAndFamily = DEFAULT_PITCH;
					wcscpy(fn.lfFaceName, L"Arial");
					HFONT hFnt = CreateFontIndirect(&fn);
					SendMessage(g.hEditMsg, WM_SETFONT, (WPARAM)hFnt, true);

					// vytvoøit pozadí pro editbox
					/*CHdc hdcBackground(false);
					RECT rc;
					GetClientRect(hDlg, &rc);
					hdcBackground.SetSize(rc.right - rc.left, rc.bottom - rc.top);
					DrawBackgroundLogo(&hdcBackground);
					g.hbrBackground = CreateBrushFromHDC(hdcBackground, hdcBackground.GetWidth(), hdcBackground.GetHeight());
					*/
					g.hbrBackground = CreateSolidBrush(RGB(239, 239, 239));


					SetFocus(g.hEditMsg);

					SendMessage(hDlg, WM_SIZE, 0, 0);

					if (*g.setting.GetMsg()) {
						WCHAR *textW = ConvertToWideChar(g.setting.GetMsg(), CP_ACP);
						SetWindowText(g.hEditMsg, textW);
						SendMessage(g.hEditMsg, EM_SETSEL, GetWindowTextLength(g.hEditMsg), GetWindowTextLength(g.hEditMsg));
						SendMessage(g.hEditMsg, EM_SCROLLCARET, 0, 0);
						g.textUndo.Push(textW);
					}

					SendMessage(g.hEditMsg, EM_SETLIMITTEXT, g.maxSMSLen, 0);
					SendMessage(g.hEditMsg, EM_SETINPUTMODE, 0, g.setting.inputMode);

					WNDPROC wndProc = (WNDPROC)SetWindowLong(g.hEditMsg, GWL_WNDPROC, (LONG)CtrlListViewProc);
					g.oldWndProcs.Add(g.hEditMsg, wndProc);

					// set timer
					SetTimer(hDlg, g.idTimerCharCounter, 100, NULL);
					SetTimer(hDlg, g.idTimerSaveUndo, 10000, NULL);
					SetTimer(hDlg, g.idTimerSaveInputMode, 500, NULL);
				}
			}
		}break; // end case WM_INITDIALOG

		case WM_ACTIVATE: {
			if (LOWORD(wParam) !=	WA_INACTIVE) {
				if (g.hEditMsg) {
					SendMessage(g.hEditMsg, EM_SETINPUTMODE, 0, g.setting.inputMode);
					SendMessage(hDlg, USER_INSERTTEXT, -1, (LPARAM)L"");
				}
			}
		}break; // end case WM_ACTIVATE

		case WM_HOTKEY: { // Override the back key to delete
			if (VK_TBACK == HIWORD(lParam) && !(lParam & MOD_KEYUP)) {
				#ifndef PPC
					SHSendBackToFocusWindow(msg, wParam, lParam);
				#endif
			}
		}break; // end case WM_HOTKEY

		case WM_CHAR: {
			if (g.skipUntilTime < GetTickCount()) {
				PostMessage(g.hEditMsg, msg, wParam, lParam);
			}
		}break; // end case WM_CHAR

		case USER_KEYUP: {
			if (g.skipUntilTime < GetTickCount()) {
				if (wParam == 115) {
					//SendMessage(hDlg, WM_COMMAND, IDM_MOZNOSTI_SMAJLIK, 0);
					PostMessage(hDlg, WM_COMMAND, IDM_MOZNOSTI_KONEC, 0);
				}
			}
		}break; // end case USER_USERKEYDOWN

		case USER_GETMSG: {
			int len;
			WCHAR *text = new WCHAR[len = (GetWindowTextLength(g.hEditMsg) + strlen(g.setting.GetSign()) + 10)];
			GetWindowText(g.hEditMsg, text, len);
			if (*g.setting.GetSign()) {
				wsprintf(text, L"%s %S", text, g.setting.GetSign());
			}
			if (lParam) *((WCHAR **)lParam) = text;
		}break; // end case USER_GETMSG

		case USER_INSERTTEXT: {
			// wParam = position (-1 represents caret position)
			// lParam = what
			int pos = wParam;
			WCHAR *text = (WCHAR *)lParam;

			int len, tlen = wcslen(text);
			WCHAR *b = new WCHAR[len = (GetWindowTextLength(g.hEditMsg) + wcslen(text) + 10)];
			GetWindowText(g.hEditMsg, b, len);
			DWORD caretPos = 0;
			if (pos == -1) SendMessage(g.hEditMsg, EM_GETSEL, (WPARAM)&caretPos, NULL);
			else caretPos = pos;
			memmove(b + caretPos + tlen, b + caretPos, (wcslen(b + caretPos) + 1) * sizeof(WCHAR));
			wcsncpy(b + caretPos, text, tlen);
			SetWindowText(g.hEditMsg, b);
			SendMessage(g.hEditMsg, EM_SETSEL, caretPos + tlen, caretPos + tlen);

			SendMessage(g.hEditMsg, EM_SCROLLCARET, 0, 0);
		}break; // end case USER_INSERTTEXT

		case WM_INITMENUPOPUP: {
			EnableMenuItem((HMENU)wParam, IDM_MOZNOSTI_VRATITTEXT, MF_BYCOMMAND | (!g.textUndo.IsEmpty() ? MF_ENABLED : MF_GRAYED));
		}break; // end case WM_INITMENUPOPUP

		case WM_TIMER: {
			if (wParam == g.idTimerCharCounter) {
				int writtenChars = GetWindowTextLength(g.hEditMsg);
				if (g.setting.sign && *g.setting.sign) writtenChars += 1 + strlen(g.setting.sign);
				int partsO = (int)((writtenChars - 1) / (160 - 8)) + 1;
				int partsE = (int)((writtenChars - 1) / (56)) + 1;
				WCHAR title[100];
				wsprintf(title, L"%i/%i:%i %i  %s", writtenChars, partsO, partsE, g.maxSMSLen - writtenChars, g.szApp);
				SetWindowText(hDlg, title);

				if (GetForegroundWindow() == hDlg && GetFocus() != g.hEditMsg) SetFocus(g.hEditMsg);
			}else if (wParam == g.idTimerSaveUndo) {
				WCHAR *text = GetWinText(g.hEditMsg);
				WCHAR *lastUndo = !g.textUndo.IsEmpty() ? g.textUndo.Cur() : L"";
				if (wcscmp(text, lastUndo)) {
					g.textUndo.Push(text);
				}else free(text);
			}else if (wParam == g.idTimerSaveInputMode) {
				if (GetActiveWindow() == hDlg) g.setting.inputMode = SendMessage(g.hEditMsg, EM_GETINPUTMODE, 0, true);
			}
		}break; // end case WM_TIMER

		case WM_SIZE: {
			RECT rc;
			GetClientRect(hDlg, &rc);
			if (g.hEditMsg) SetWindowPos(g.hEditMsg, 0, 0, 0, rc.right, rc.bottom, 0);
		}break; // end case WM_SIZE

		case WM_COMMAND: {
			switch (LOWORD(wParam)) {
				case IDM_MOZNOSTI_KONEC: {
					WCHAR *text = GetWinText(g.hEditMsg);
					char *textA = ConvertToMultiByte(text, CP_ACP);
					g.setting.SetMsg(textA);
					free(textA);
					free(text);
										
					DestroyWindow(g.hMain);
				}break; // end case IDM_MOZNOSTI_KONEC

				case IDM_POSLAT: {
					DisplayDlg(SENDWAY, true, false);
				}break; // end case IDM_POSLAT

				case IDM_MOZNOSTI_SMAJLIK: {
					DisplayDlg(INSERTSMILEY, 0, false);
				}break;

				case IDM_MOZNOSTI_VRATITTEXT: {
					if (!g.textUndo.IsEmpty()) {
						free(g.textUndo.Cur());
						g.textUndo.Pop();
						SetWindowText(g.hEditMsg, !g.textUndo.IsEmpty() ? g.textUndo.Cur() : L"");
						SendMessage(g.hEditMsg, EM_SETSEL, GetWindowTextLength(g.hEditMsg), GetWindowTextLength(g.hEditMsg));
						SendMessage(g.hEditMsg, EM_SCROLLCARET, 0, 0);
					}
				}break; // end case IDM_MOZNOSTI_VRATITTEXT

				case IDM_MOZNOSTI_VYMAZATMEZERY: {
					WCHAR *text = GetWinText(g.hEditMsg);
					g.textUndo.Push(wcsdup(text));
					ClearSpaces(text);
					SetWindowText(g.hEditMsg, text);
					SendMessage(g.hEditMsg, EM_SETSEL, GetWindowTextLength(g.hEditMsg), GetWindowTextLength(g.hEditMsg));
					SendMessage(g.hEditMsg, EM_SCROLLCARET, 0, 0);
					free(text);
				}break; // end case IDM_MOZNOSTI_VYMAZATMEZERY

				case IDM_MOZNOSTI_VYMAZATTEXT: {
					WCHAR *text = GetWinText(g.hEditMsg);
					g.textUndo.Push(wcsdup(text));
					SetWindowText(g.hEditMsg, L"");
				}break; // end case IDM_MOZNOSTI_VYMAZATTEXT

				case IDM_MOZNOSTI_ADRESAR: {
					static CDlgAddressBook ab;
					ab.type = CDlgAddressBook::TYPE::SHOW;
					// show phonebook
					DisplayDlg(PHONEBOOK, (LPARAM)&ab, false);
				}break; // end case IDM_MOZNOSTI_ADRESAR

				case IDM_MOZNOSTI_INFORMACE: {
					DisplayDlg(INFORMATION, true, false);
				}break; // end case IDM_MOZNOSTI_NASTAVENI

				case IDM_MOZNOSTI_NASTAVENI: {
					DialogBoxParam(g.hInstance, MAKEINTRESOURCE(IDD_SETTING), 0, (DLGPROC)DlgSettingProc, 0);
				}break; // end case IDM_MOZNOSTI_NASTAVENI

				case IDM_MOZNOSTI_CTENIZPRAV: {
					DisplayDlg(LSTMSGSREAD, true, false);
				}break; // end case IDM_MOZNOSTI_CTENIZPRAV

				case 0: {
					//if (HIWORD(wParam) == 512) PostQuitMessage(0);
				}break;

				case IDOK: {
					PostQuitMessage(0);
				}break; // end case IDOK
			}
		}break; // end case WM_COMMAND

		case WM_CTLCOLOREDIT: {
			//SetBkMode((HDC)wParam, TRANSPARENT);
			SetBkColor((HDC)wParam, RGB(239, 239, 239));
			return (LRESULT)g.hbrBackground;
		}break; // end case WM_CTLCOLOREDIT

		case WM_DESTROY: {
			PostQuitMessage(0);
		}break; // end case WM_DESTROY
	}

	// !!! HandleWindowScroll(hDlg, msg, wParam, lParam);

	return DefWindowProc(hDlg, msg, wParam, lParam);
}


/***********************************************************************************
* Initialize the dialog                                                            *
***********************************************************************************/
BOOL InitDlgMain(HWND hWnd) {
	// initialize dialog box, specifying that it should stretch full screen
	SHINITDLGINFO sid;
	ZeroMemory(&sid, sizeof(sid));
	sid.dwMask  = SHIDIM_FLAGS;
	sid.dwFlags = SHIDIF_SIZEDLGFULLSCREEN/*|SHIDIF_DONEBUTTON*/;
	sid.hDlg    = hWnd;

	// initialize the dialog
	if (!SHInitDialog(&sid)) return false;

	// set the title bar
	SetWindowText(hWnd, g.szApp);

	// initialize menubar
	SHMENUBARINFO mbi;
	ZeroMemory(&mbi, sizeof(SHMENUBARINFO));
	mbi.cbSize      = sizeof(SHMENUBARINFO);
	mbi.hwndParent  = hWnd;
	mbi.nToolBarId  = IDR_MENU_MAIN;
	mbi.hInstRes    = g.hInstance;

	if (!SHCreateMenuBar(&mbi)) return false;

	// Overriding Back button functionality
	SendMessage(mbi.hwndMB, SHCMBM_OVERRIDEKEY, VK_TBACK, 
				  MAKELPARAM(SHMBOF_NODEFAULT | SHMBOF_NOTIFY, 
							 SHMBOF_NODEFAULT | SHMBOF_NOTIFY));

	ShowWindow(g.hMenuBarCurrent, SW_HIDE);
	g.hMenuBarCurrent = mbi.hwndMB;
	ShowWindow(g.hMenuBarCurrent, SW_SHOW);


	return true;
}


/***********************************************************************************
* Dlg message handler                                                              *
***********************************************************************************/
LRESULT DlgWaitProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam) {
	static CHdc hStage(false), hWait(false);
	static BOOL waitLoaded = false;
	static int animPos = 0;

	switch (msg) {
		case WM_INITDIALOG: {
			if (!InitDlgWait(hDlg, lParam)) {
				DisplayDlg(MAIN, 0, false);
			}else {
				// load wait image
				if (!waitLoaded) {
					hWait.LoadBitmap(LoadBitmap(g.hInstance, (LPCTSTR)IDB_WAIT));
					waitLoaded = true;
				}
				
				// load stage
				RECT rc;
				GetWindowRect(hDlg, &rc);
				hStage.SetSize(rc.right - rc.left, rc.bottom - rc.top);
				SetRect(&rc, 0, 0, hStage.GetWidth(), hStage.GetHeight());
				HBRUSH hbr = CreateSolidBrush(RGB(255, 255, 255));
				FillRect(hStage, &rc, hbr);

				// load background image
				CHdc bg(false);
				bg.LoadBitmap(LoadBitmap(g.hInstance, (LPCTSTR)IDB_BG));
				int screenWidth = GetSystemMetrics(SM_CXSCREEN);
				for (DWORD y = 0; y < hStage.GetHeight(); y += bg.GetHeight()) {
					StretchBlt(hStage, 0, y, screenWidth, bg.GetHeight(), bg, 0, 0, bg.GetWidth(), bg.GetHeight(), SRCCOPY);
				}

				// draw background logo
				DrawBackgroundLogo(&hStage);

				BitBltTransparent(hStage, hStage.GetWidth() / 2 - hWait.GetWidth() / 2, hStage.GetHeight() / 2 - hWait.GetHeight(), hWait.GetWidth(), hWait.GetHeight(), hWait, 0, 0, RGB(255, 255, 255));

				// set animation properties
				animPos = 0;

				// set timer
				SetTimer(hDlg, g.idTimerRefresh, 60, NULL);
			}
		}break; // end case WM_INITDIALOG

		case USER_FINISHDOWNLOADING: {
			CViewPage *page = (CViewPage *)lParam;
			if (page) {
				static CDlgStatus ds;
				ds.caption = page->GetCaption();
				ds.type = (CDlgStatus::TYPE)wParam;
				ds.showSucceedPic = ds.type == CDlgStatus::TYPE::SENDMSG || ds.type == CDlgStatus::TYPE::EXPORTPHONEBOOK;
				ds.succeed = (page->GetSucceed() && (ds.type == CDlgStatus::TYPE::SENDMSG || ds.type == CDlgStatus::TYPE::EXPORTPHONEBOOK || ds.type == CDlgStatus::TYPE::IMPORTPHONEBOOK_LIST || ds.type == CDlgStatus::TYPE::IMPORTPHONEBOOK_READ || ds.type == CDlgStatus::TYPE::MSGSREAD)) ? page->GetResultText()[0] == L'1' : page->GetSucceed();
				ds.resultText = (page->GetSucceed() && (ds.type == CDlgStatus::TYPE::SENDMSG || ds.type == CDlgStatus::TYPE::EXPORTPHONEBOOK || ds.type == CDlgStatus::TYPE::IMPORTPHONEBOOK_LIST || ds.type == CDlgStatus::TYPE::IMPORTPHONEBOOK_READ || ds.type == CDlgStatus::TYPE::MSGSREAD)) ? page->GetResultText() + 1 : page->GetResultText();
				ds.playSound = g.setting.allowSounds && ds.showSucceedPic;
				ds.quitCountdown = g.setting.quitOnSuccessfullSend && ds.succeed && ds.type == CDlgStatus::TYPE::SENDMSG;
				ds.disabledMenuItems.RemoveAll();

				if ((ds.type == CDlgStatus::TYPE::SENDMSG || ds.type == CDlgStatus::TYPE::SHOWINFO || ds.type == CDlgStatus::TYPE::EXPORTPHONEBOOK || ds.type == CDlgStatus::TYPE::MSGSREAD) || !ds.succeed) {
					if (ds.type == CDlgStatus::TYPE::SENDMSG && ds.succeed) ds.idMenu = IDR_MENU_STATUS_NEWMSGEND;
					else if (ds.type == CDlgStatus::TYPE::MSGSREAD && ds.succeed) ds.idMenu = IDR_MENU_STATUS_MSGSREAD;
					else if (/*ds.type == CDlgStatus::TYPE::SENDMSG &&*/ !ds.succeed) ds.idMenu = IDR_MENU_STATUS_REPEATBACKEND;
					else ds.idMenu = IDR_MENU_STATUS_BACKEND;

					BOOL showStatus = true;
					
					// pokud je 
					if (ds.type == CDlgStatus::TYPE::MSGSREAD && ds.succeed) {
						WCHAR *extras = page->GetExtras();
						char *extrasA = ConvertToMultiByte(extras, CP_ACP);

						CCSVWalker csv;

						csv.ParseText(extrasA);
						
						g.msgsReadCorrectPasswordSubmitted = csv.GetFieldInt("correct_password", 0, false);
						g.msgsReadMsg_0_start = csv.GetFieldInt("msg_0_start", 0, 0);
						g.msgsReadMsg_0_length = csv.GetFieldInt("msg_0_length", 0, 0);

						if (csv.GetFieldInt("end_eof", 0, 1)) ds.disabledMenuItems.Add(IDM_MOZNOSTI_DALSIZPRAVY);
						if (csv.GetFieldInt("start_eof", 0, 1)) ds.disabledMenuItems.Add(IDM_MOZNOSTI_PREDCHOZIZPRAVY);
						if (!csv.GetFieldInt("allow_jump", 0, 1)) ds.disabledMenuItems.Add(IDM_MOZNOSTI_PRESUNOUTSEO);
						if (!csv.GetFieldInt("allow_search", 0, 1)) ds.disabledMenuItems.Add(IDM_MOZNOSTI_VYHLEDAT);
						if (!csv.GetFieldInt("allow_edit_first", 0, 1)) ds.disabledMenuItems.Add(IDM_MOZNOSTI_ZKOPIROVATPRVNIZPRAVU);
						if (!csv.GetFieldInt("allow_edit_all", 0, 1)) ds.disabledMenuItems.Add(IDM_MOZNOSTI_ZKOPIROVATVSE);
						if (!csv.GetFieldInt("allow_refresh", 0, 1)) ds.disabledMenuItems.Add(IDM_MOZNOSTI_OBNOVIT);

						delete[] extrasA;
					}else if (ds.type == CDlgStatus::TYPE::SENDMSG) {
						WCHAR *extras = page->GetExtras();
						char *extrasA = ConvertToMultiByte(extras, CP_ACP);

						CCSVWalker csv;

						csv.ParseText(extrasA);
						
						char *message_id = csv.GetField("message_id", 0, NULL);
						if (g.sendMsgMessageId) free(g.sendMsgMessageId);
						if (message_id) g.sendMsgMessageId = _strdup(message_id);
						else g.sendMsgMessageId = _strdup("");

						char *action = csv.GetField("action", 0, NULL);
						if (action && !_stricmp(action, "enter_pictogram")) {
							showStatus = false;

							static CHdc hdcPictogram(false);
							DrawPictogram(&hdcPictogram, csv.GetField("pictogram_data", 0, NULL), csv.GetFieldInt("pictogram_width", 0, 0), csv.GetFieldInt("pictogram_height", 0, 0));
							DisplayDlg(ENTERPICTOGRAM, (LPARAM)&hdcPictogram, false);
						}
					}

					// show status dialog
					if (showStatus) DialogBoxParam(g.hInstance, MAKEINTRESOURCE(IDD_STATUS), 0, (DLGPROC)DlgStatusProc, (LPARAM)&ds);
				}else if (ds.type == CDlgStatus::TYPE::IMPORTPHONEBOOK_LIST) {
					DisplayDlg(IMPORTPHONEBOOK_LIST, (LPARAM)ds.resultText, false);
				}else if (ds.type == CDlgStatus::TYPE::IMPORTPHONEBOOK_READ) {
					char *data = ConvertToMultiByte(ds.resultText, CP_ACP);
					DWORD numImported = g.phonebook.ImportFromNet(data);
					g.phonebook.Sort();
					free(data);

					WCHAR b[20];
					wsprintf(b, L"Bylo importováno %i nových kontaktù", numImported);
					MessageBox(hDlg, b, g.szApp, MB_OK|MB_ICONINFORMATION);

					DisplayDlg(PHONEBOOK, 0, false);

					SendMessage(g.hMain, USER_REFRESHLIST, 0, 0);
				}
				
				// delete object create in thread
				delete page;
			}
		}break; // end case USER_FINISHDOWNLOADING

		case WM_TIMER: {
			if (wParam == g.idTimerRefresh) {
				animPos += 2;
				InvalidateRect(hDlg, NULL, false);
				UpdateWindow(hDlg);
			}
		}break; // end case WM_TIMER

		case WM_PAINT: {
			PAINTSTRUCT ps;
			BeginPaint(hDlg, &ps);

			// draw the animation
			int n = 63;
			int wid = 126, baseX = hStage.GetWidth() / 2 - wid / 2, baseY = hStage.GetHeight() / 2 + 15;
			int w = wid / n;
			for (int i = 0; i < n; i++) {
				HBRUSH hbr = CreateSolidBrush(RGB(0, 0, abs(255 - 2 * i * (255 / n))));
				RECT rc = {baseX + ((animPos + i) * w) % wid, baseY, 0, baseY + 5};
				rc.right = rc.left + w;
				FillRect(hStage, &rc, hbr);
				DeleteObject(hbr);
			}

			BitBlt(ps.hdc, 0, 0, hStage.GetWidth(), hStage.GetHeight(), hStage, 0, 0, SRCCOPY);
			EndPaint(hDlg, &ps);
		}break; // end case WM_PAINT

		case WM_COMMAND: {
			switch (LOWORD(wParam)) {
				case IDM_PRERUSIT: {
					DisplayDlg(MAIN, 0, false);
				}break; // end case IDM_MOZNOSTI_KONEC

				case 0: {
					if (HIWORD(wParam) == 512) PostQuitMessage(0);
				}break;
			}
		}break; // end case WM_COMMAND

		case WM_DESTROY: {
			PostQuitMessage(0);
		}break; // end case WM_DESTROY
	}

	HandleWindowScroll(hDlg, msg, wParam, lParam);

	return DefWindowProc(hDlg, msg, wParam, lParam);
}


/***********************************************************************************
* Shows wait dialog                                                                *
***********************************************************************************/
BOOL InitDlgWait(HWND hWnd, BOOL allowCancel) {
	// set the title bar
	WCHAR b[100];
	wsprintf(b, L"%s %s", g.szApp, g.szVersion);
	SetWindowText(hWnd, b);

	// initialize menubar
	SHMENUBARINFO mbi;
	ZeroMemory(&mbi, sizeof(SHMENUBARINFO));
	mbi.cbSize      = sizeof(SHMENUBARINFO);
	mbi.hwndParent  = hWnd;
	mbi.nToolBarId  = allowCancel ? IDR_MENU_WAIT : IDR_MENU_NONE;
	mbi.hInstRes    = g.hInstance;

	if (!SHCreateMenuBar(&mbi)) return false;

	// Overriding Back button functionality
	SendMessage(mbi.hwndMB, SHCMBM_OVERRIDEKEY, VK_TBACK, 
				  MAKELPARAM(SHMBOF_NODEFAULT | SHMBOF_NOTIFY, 
							 SHMBOF_NODEFAULT | SHMBOF_NOTIFY));

	ShowWindow(g.hMenuBarCurrent, SW_HIDE);
	g.hMenuBarCurrent = mbi.hwndMB;
	ShowWindow(g.hMenuBarCurrent, SW_SHOW);

	return true;
}


/***********************************************************************************
* Dlg message handler                                                              *
***********************************************************************************/
BOOL CALLBACK DlgSettingProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
		case WM_INITDIALOG: {
			if (!InitDlgSetting(hDlg)) {
				EndDialog(hDlg, 0);
				DisplayDlg(MAIN, 0, false);
			}else {
				SetFocus(GetDlgItem(hDlg, IDC_EDIT_SRCNUM));

				WCHAR *b = ConvertToWideChar(g.setting.GetSrcNum(), CP_ACP);
				SetDlgItemText(hDlg, IDC_EDIT_SRCNUM, b);
				free(b);

				b = ConvertToWideChar(g.setting.GetPwd(), CP_ACP);
				SetDlgItemText(hDlg, IDC_EDIT_PASSWORD, b);
				free(b);

				b = ConvertToWideChar(g.setting.GetSign(), CP_ACP);
				SetDlgItemText(hDlg, IDC_EDIT_SIGN, b);
				free(b);

				if (g.setting.confirmReceiver) SendDlgItemMessage(hDlg, IDC_CHECK_CONFIRMRECEIVER, BM_SETCHECK, BST_CHECKED, 0);

				if (g.setting.allowSounds) SendDlgItemMessage(hDlg, IDC_CHECK_ALLOWSOUNDS, BM_SETCHECK, BST_CHECKED, 0);

				if (g.setting.quitOnSuccessfullSend) SendDlgItemMessage(hDlg, IDC_CHECK_QUITONSUCCESSFULLSEND, BM_SETCHECK, BST_CHECKED, 0);

				if (g.setting.expertMode) SendDlgItemMessage(hDlg, IDC_CHECK_EXPERTMODE, BM_SETCHECK, BST_CHECKED, 0);

				SetDlgItemInt(hDlg, IDC_EDIT_MSGSREADPAGE, g.setting.msgsReadPage, false);

				b = ConvertToWideChar(g.setting.GetEmail(), CP_ACP);
				SetDlgItemText(hDlg, IDC_EDIT_EMAIL, b);
				free(b);

				SetScroller(hDlg);
			}
		}break; // end case WM_INITDIALOG

		case WM_HOTKEY: { // Override the back key to delete
			if (VK_TBACK == HIWORD(lParam)) {
				#ifndef PPC
					SHSendBackToFocusWindow(msg, wParam, lParam);
				#endif
			}
		}break;

		case WM_CLOSE: {
			EndDialog(hDlg, 0);
			DisplayDlg(MAIN, 0, false);
		}break; // case WM_CLOSE

		case WM_COMMAND: {
			switch (LOWORD(wParam)) {
				case IDOK: {
					WCHAR b[100]; char *bb;

					GetDlgItemText(hDlg, IDC_EDIT_SRCNUM, b, 100);
					g.setting.SetSrcNum(bb = ConvertToMultiByte(b, CP_ACP));
					free(bb);

					GetDlgItemText(hDlg, IDC_EDIT_PASSWORD, b, 100);
					g.setting.SetPwd(bb = ConvertToMultiByte(b, CP_ACP));
					free(bb);

					GetDlgItemText(hDlg, IDC_EDIT_SIGN, b, 100);
					g.setting.SetSign(bb = ConvertToMultiByte(b, CP_ACP));
					free(bb);

					g.setting.confirmReceiver = SendDlgItemMessage(hDlg, IDC_CHECK_CONFIRMRECEIVER, BM_GETCHECK, 0, 0);

					g.setting.allowSounds = SendDlgItemMessage(hDlg, IDC_CHECK_ALLOWSOUNDS, BM_GETCHECK, 0, 0);

					g.setting.quitOnSuccessfullSend = SendDlgItemMessage(hDlg, IDC_CHECK_QUITONSUCCESSFULLSEND, BM_GETCHECK, 0, 0);

					g.setting.expertMode = SendDlgItemMessage(hDlg, IDC_CHECK_EXPERTMODE, BM_GETCHECK, 0, 0);

					g.setting.msgsReadPage = max(GetDlgItemInt(hDlg, IDC_EDIT_MSGSREADPAGE, NULL, false), 1);

					GetDlgItemText(hDlg, IDC_EDIT_EMAIL, b, 100);
					g.setting.SetEmail(bb = ConvertToMultiByte(b, CP_ACP));
					free(bb);


					g.setting.Write();
					EndDialog(hDlg, 0);
					DisplayDlg(MAIN, 0, false);
				}break; // end case IDOK

				case IDCANCEL: {
					EndDialog(hDlg, 0);
					DisplayDlg(MAIN, 0, false);
				}break; // end case IDCANCEL:
			}
		}break; // end case WM_COMMAND
	}

	HandleWindowScroll(hDlg, msg, wParam, lParam);

	return false;
}


/***********************************************************************************
* Initialize the dialog                                                            *
***********************************************************************************/
BOOL InitDlgSetting(HWND hWnd) {
	// initialize dialog box, specifying that it should stretch full screen
	SHINITDLGINFO sid;
	ZeroMemory(&sid, sizeof(sid));
	sid.dwMask  = SHIDIM_FLAGS;
	sid.dwFlags = SHIDIF_SIZEDLGFULLSCREEN;
	sid.hDlg    = hWnd;

	// initialize the dialog
	if (!SHInitDialog(&sid)) return false;

	// set the title bar
	WCHAR b[100];
	wsprintf(b, L"%s - %s", g.szApp, L"Nastavení");
	SetWindowText(hWnd, b);


	// initialize menubar
	SHMENUBARINFO mbi;
	ZeroMemory(&mbi, sizeof(SHMENUBARINFO));
	mbi.cbSize      = sizeof(SHMENUBARINFO);
	mbi.hwndParent  = hWnd;
	mbi.nToolBarId  = IDR_MENU_SETTING;
	mbi.hInstRes    = g.hInstance;

	if (!SHCreateMenuBar(&mbi)) return false;

	// Overriding Back button functionality
	SendMessage(mbi.hwndMB, SHCMBM_OVERRIDEKEY, VK_TBACK, 
				  MAKELPARAM(SHMBOF_NODEFAULT | SHMBOF_NOTIFY, 
							 SHMBOF_NODEFAULT | SHMBOF_NOTIFY));

	ShowWindow(g.hMenuBarCurrent, SW_HIDE);
	g.hMenuBarCurrent = mbi.hwndMB;
	ShowWindow(g.hMenuBarCurrent, SW_SHOW);


	return true;
}


/***********************************************************************************
* Dlg message handler                                                              *
***********************************************************************************/
BOOL CALLBACK DlgContactProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam) {
	static CPhonebookItem *pi = NULL;

	switch (msg) {
		case WM_INITDIALOG: {
			pi = (CPhonebookItem *)lParam;
			WCHAR title[100];
			if (pi && pi->GetName() && *pi->GetName()) wsprintf(title, L"Adresáø::%S", pi->GetName());
			else wcscpy(title, L"Adresáø::Nový záznam");

			if (!InitDlgContact(hDlg, title)) {
				EndDialog (hDlg, -1);
			}else {
				if (pi) {
					WCHAR *b = ConvertToWideChar(pi->GetName(), CP_ACP);
					SetDlgItemText(hDlg, IDC_EDIT_NAME, b);
					free(b);

					b = ConvertToWideChar(pi->GetNumber(), CP_ACP);
					SetDlgItemText(hDlg, IDC_EDIT_NUMBER, b);
					free(b);

					b = ConvertToWideChar(pi->GetEmail(), CP_ACP);
					SetDlgItemText(hDlg, IDC_EDIT_EMAIL, b);
					free(b);
				}

				SetFocus(GetDlgItem(hDlg, IDC_EDIT_NAME));

				SetScroller(hDlg);
			}
		}break; // end case WM_INITDIALOG

		case WM_HOTKEY: { // Override the back key to delete
			if (VK_TBACK == HIWORD(lParam)) {
				#ifndef PPC
					SHSendBackToFocusWindow(msg, wParam, lParam);
				#endif
			}
		}break;

		case WM_CLOSE: {
			EndDialog(hDlg, 0);
		}break; // case WM_CLOSE

		case WM_COMMAND: {
			switch (LOWORD(wParam)) {
				case IDOK: {
					if (pi) {
						if (GetWindowTextLength(GetDlgItem(hDlg, IDC_EDIT_NAME)) > 0) {
							WCHAR b[100]; char *bb;

							GetDlgItemText(hDlg, IDC_EDIT_NAME, b, 100);
							pi->SetName(bb = ConvertToMultiByte(b, CP_ACP));
							free(bb);

							GetDlgItemText(hDlg, IDC_EDIT_NUMBER, b, 100);
							pi->SetNumber(bb = ConvertToMultiByte(b, CP_ACP));
							free(bb);

							GetDlgItemText(hDlg, IDC_EDIT_EMAIL, b, 100);
							pi->SetEmail(bb = ConvertToMultiByte(b, CP_ACP));
							free(bb);

							EndDialog(hDlg, 1);
						}else MessageBox(hDlg, L"Musíte zadat jméno kontaktu", g.szAppErr, MB_OK|MB_ICONSTOP);
					}else EndDialog(hDlg, 0);
				}break; // end case IDOK

				case IDCANCEL: {
					EndDialog(hDlg, 0);
				}break; // end case IDCANCEL:
			}
		}break; // end case WM_COMMAND
	}

	HandleWindowScroll(hDlg, msg, wParam, lParam);

	return false;
}


/***********************************************************************************
* Initialize the dialog                                                            *
***********************************************************************************/
BOOL InitDlgContact(HWND hWnd, WCHAR *title) {
	// initialize dialog box, specifying that it should stretch full screen
	SHINITDLGINFO sid;
	ZeroMemory(&sid, sizeof(sid));
	sid.dwMask  = SHIDIM_FLAGS;
	sid.dwFlags = SHIDIF_SIZEDLGFULLSCREEN;
	sid.hDlg    = hWnd;

	// initialize the dialog
	if (!SHInitDialog(&sid)) return false;

	// set the title bar
	SetWindowText(hWnd, title);


	// initialize menubar
	SHMENUBARINFO mbi;
	ZeroMemory(&mbi, sizeof(SHMENUBARINFO));
	mbi.cbSize      = sizeof(SHMENUBARINFO);
	mbi.hwndParent  = hWnd;
	mbi.nToolBarId  = IDR_MENU_CONTACT;
	mbi.hInstRes    = g.hInstance;

	if (!SHCreateMenuBar(&mbi)) return false;

	// Overriding Back button functionality
	SendMessage(mbi.hwndMB, SHCMBM_OVERRIDEKEY, VK_TBACK, 
				  MAKELPARAM(SHMBOF_NODEFAULT | SHMBOF_NOTIFY, 
							 SHMBOF_NODEFAULT | SHMBOF_NOTIFY));

	ShowWindow(g.hMenuBarCurrent, SW_HIDE);
	g.hMenuBarCurrent = mbi.hwndMB;
	ShowWindow(g.hMenuBarCurrent, SW_SHOW);


	return true;
}


/***********************************************************************************
* Dlg message handler                                                              *
***********************************************************************************/
BOOL CALLBACK DlgStatusProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam) {
	static CDlgStatus *ds = NULL;
	static HFONT hFnt = NULL;
	static LOGFONT fn;
	static UINT actFrame = 0;
	static UINT frameTimingSuccess[] = {500, 100, 1500, 100, 3000};
	static UINT frameTimingError[] = {0};
	static HBITMAP framesSuccess[] = {LoadBitmap(g.hInstance, (LPCWSTR)IDB_SMILE0), LoadBitmap(g.hInstance, (LPCWSTR)IDB_SMILE1), LoadBitmap(g.hInstance, (LPCWSTR)IDB_SMILE2), LoadBitmap(g.hInstance, (LPCWSTR)IDB_SMILE3), LoadBitmap(g.hInstance, (LPCWSTR)IDB_SMILE0)};
	static HBITMAP framesError[] = {LoadBitmap(g.hInstance, (LPCWSTR)IDB_ERROR)};
	static BOOL succeed;
	static CHdc hStage(false);
	static CHdc hdcBackground(false);
	int picTopPadding = 15, textSidesPadding = 10;
	static DWORD countdownStart;

	switch (msg) {
		case WM_INITDIALOG: {
			if (!hFnt) {
				memset(&fn, 0, sizeof(fn));
				fn.lfHeight = ComputeFontSize(g.hEditMsg, 12);
				fn.lfWidth = 0;
				fn.lfEscapement = 0;
				fn.lfOrientation = 0;
				fn.lfWeight = FW_NORMAL;
				fn.lfItalic = false;
				fn.lfUnderline = false;
				fn.lfStrikeOut = false;
				fn.lfCharSet = EASTEUROPE_CHARSET;
				fn.lfOutPrecision = OUT_DEFAULT_PRECIS;
				fn.lfClipPrecision = CLIP_DEFAULT_PRECIS;
				fn.lfQuality = DEFAULT_QUALITY;
				fn.lfPitchAndFamily = DEFAULT_PITCH;
				wcscpy(fn.lfFaceName, L"Tahoma");
				hFnt = CreateFontIndirect(&fn);

				//SendDlgItemMessage(hDlg, IDC_STATIC_STATUS, WM_SETFONT, (WPARAM)hFnt, MAKELPARAM(true, 0));
			}


			ds = (CDlgStatus *)lParam;
			if (!ds || !InitDlgStatus(hDlg, ds->caption, ds->idMenu)) {
				EndDialog(hDlg, 0);
			}else {
				WCHAR *res = ds->resultText;
				res = CorrectEnters(res);
				SetDlgItemText(hDlg, IDC_STATIC_STATUS, /*res*/ L"");
				succeed = ds->succeed;
				// set size of the static control
				RECT rc;
				GetWindowRect(hDlg, &rc);

				int dialogHeight = 0;

				int textWidth = rc.right - rc.left - 2 * textSidesPadding;

				int lineHeight, textHeight;
				CArray<WCHAR *> *lines = WrapTextMultiLine(res, &fn, textWidth, &lineHeight);
				textHeight = (lineHeight * lines->num);
				if (textHeight > (rc.bottom - rc.top)) {
					textWidth -= GetSystemMetrics(SM_CXVSCROLL);
					if (lines) delete lines;
					lines = WrapTextMultiLine(res, &fn, textWidth, &lineHeight);
					textHeight = (lineHeight * lines->num);
				}
				dialogHeight += textHeight;
				//SetWindowPos(GetDlgItem(hDlg, IDC_STATIC_STATUS), 0, textSidesPadding, 0, textWidth, textHeight, 0);
				SetWindowPos(GetDlgItem(hDlg, IDC_STATIC_STATUS), 0, 0, 0, 1, textHeight, 0);
				// prepare the stage a draw text on it
				hdcBackground.SetSize(rc.right - rc.left, max(textHeight, rc.bottom - rc.top));
				DrawBackgroundLogo(&hdcBackground);

				HBRUSH hBr = CreateSolidBrush(RGB(255, 255, 255));
				RECT rcClear;
				hStage.SetSize(hdcBackground.GetWidth(), hdcBackground.GetHeight());
				SetRect(&rcClear, 0, 0, hStage.GetWidth(), hStage.GetHeight());
				FillRect(hStage, &rcClear, hBr);
				DeleteObject(hBr);

				// draw background logo
				if (!(ds->showSucceedPic || !succeed)) BitBlt(hStage, 0, 0, hdcBackground.GetWidth(), hdcBackground.GetHeight(), hdcBackground, 0, 0, SRCCOPY);

				HFONT fntOld = (HFONT)SelectObject(hStage, hFnt);
				RECT rcLine = {textSidesPadding, 0, textSidesPadding + textWidth, lineHeight};
				SetBkMode(hStage, TRANSPARENT);
				for (DWORD i = 0; i < lines->num; i++) {
					DrawText(hStage, (*lines)[i], (i + 1 < lines->num) ? (*lines)[i + 1] - (*lines)[i] : -1, &rcLine, DT_NOPREFIX);
					OffsetRect(&rcLine, 0, lineHeight);
				}
				SelectObject(hStage, fntOld);

				delete lines;

				if (ds->showSucceedPic || !succeed) {
					int bmpWid = succeed ? 66 : 51, bmpHei = succeed ? 60 : 46;

					actFrame = 0;
					
					SendDlgItemMessage(hDlg, IDC_STATIC_STATUSPIC, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)(succeed ? framesSuccess[actFrame] : framesError[actFrame]));
					SetWindowPos(GetDlgItem(hDlg, IDC_STATIC_STATUSPIC), 0, (rc.right - rc.left) / 2 - bmpWid / 2, textHeight + picTopPadding, bmpWid, bmpHei, 0);

					dialogHeight += picTopPadding + bmpHei;

					if (succeed ? frameTimingSuccess[actFrame] : frameTimingError[actFrame]) SetTimer(hDlg, g.idTimerAnimStatus, frameTimingSuccess[0], NULL);
				}else ShowWindow(GetDlgItem(hDlg, IDC_STATIC_STATUSPIC), SW_HIDE);

				if (ds->playSound) {
					SetTimer(hDlg, g.idTimerPlaysound, 100, NULL);
				}
				if (ds->quitCountdown) {
					countdownStart = GetTickCount();
					SetTimer(hDlg, g.idTimerCountdown, 1000, NULL);
				}

				free(res);

				// timer for refreshing
				SetTimer(hDlg, g.idTimerRefresh, 50, NULL);

				// set the scrollbar
				SCROLLINFO si;
				si.cbSize = sizeof(si);
				si.fMask = SIF_RANGE|SIF_PAGE;
				GetScrollInfo(hDlg, SB_VERT, &si);
				si.nMax = dialogHeight;
				si.nPage = rc.bottom - rc.top;
				SetScrollInfo(hDlg, SB_VERT, &si, true);
			}
		}break; // end case WM_INITDIALOG

		case WM_INITMENUPOPUP: {
			if (ds) {
				for (DWORD i = 0; i < ds->disabledMenuItems.num; i++) {
					EnableMenuItem((HMENU)wParam, ds->disabledMenuItems[i], MF_BYCOMMAND | MF_GRAYED);
				}
			}
		}break; // end case WM_INITMENUPOPUP

		case WM_TIMER: {
			if (wParam == g.idTimerAnimStatus) {
				SendDlgItemMessage(hDlg, IDC_STATIC_STATUSPIC, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)(succeed ? framesSuccess[actFrame] : framesError[actFrame]));
				if (succeed ? frameTimingSuccess[actFrame] : frameTimingError[actFrame]) SetTimer(hDlg, g.idTimerAnimStatus, succeed ? frameTimingSuccess[actFrame] : frameTimingError[actFrame], NULL);

				actFrame++;
				if (actFrame >= (succeed ? sizeof(framesSuccess) : sizeof(framesError)) / sizeof(HBITMAP)) actFrame = 0;
			}else if (wParam == g.idTimerRefresh) {
				static int oldScrollPos = 0;
				SCROLLINFO si;
				
				si.cbSize = sizeof(si);
				si.fMask = SIF_POS;
				GetScrollInfo(hDlg, SB_VERT, &si);

				if (si.nPos != oldScrollPos) {
					InvalidateRect(hDlg, NULL, false);
					UpdateWindow(hDlg);
					oldScrollPos = si.nPos;
				}
			}else if (wParam == g.idTimerPlaysound) {
				WCHAR *soundFile = GetApplicationFile(succeed ? L"success.wav" : L"error.wav");
				BOOL ok = false;
				if (FileExists(soundFile)) {
					PlaySound(soundFile, NULL, SND_FILENAME);
					ok = true;
				}
				delete[] soundFile;

				if (!ok) PlaySound((LPCWSTR)(succeed ? IDR_WAVE_SUCCESS : IDR_WAVE_ERROR), g.hInstance, SND_RESOURCE);
				
				KillTimer(hDlg, g.idTimerPlaysound);
			}else if (wParam == g.idTimerCountdown) {
				WCHAR b[100];
				int t = g.countdownTime - (GetTickCount() - countdownStart) / 1000;
				if (t < 0) {
					SendMessage(hDlg, WM_COMMAND, IDM_KONEC, 0);
				}else {
					wsprintf(b, L"Zavøít za %i s", t);
					SetWindowText(hDlg, b);
				}
			}
		}break; // end case WM_TIMER

		case WM_PAINT: {
			PAINTSTRUCT ps;
			SCROLLINFO si;
			RECT rc;
			
			BeginPaint(hDlg, &ps);
			
			ps.hdc = GetDC(hDlg);

			GetWindowRect(hDlg, &rc);
			BitBlt(ps.hdc, 0, 0, rc.right, rc.bottom, NULL, 0, 0, WHITENESS);

			si.cbSize = sizeof(si);
			si.fMask = SIF_POS;
			GetScrollInfo(hDlg, SB_VERT, &si);

			int hei = min((int)hStage.GetHeight() - si.nPos, rc.bottom - rc.top);
			BitBlt(ps.hdc, 0, 0, hStage.GetWidth(), hei, hStage, 0, si.nPos, SRCCOPY);

			ReleaseDC(hDlg, ps.hdc);

			EndPaint(hDlg, &ps);
		}break; // end case WM_PAINT

		case WM_CLOSE: {
			EndDialog(hDlg, 0);
		}break; // case WM_CLOSE

		case WM_COMMAND: {
			switch (LOWORD(wParam)) {
				case IDM_ZPET: {
					EndDialog(hDlg, 0);
					DisplayDlg(g.displayPrev, 0, false);
				}break; // end case IDOK

				case IDM_KONEC: {
					if (succeed) g.setting.SetMsg("");
					EndDialog(hDlg, 0);
					DestroyWindow(g.hMain);
				}break; // end case IDM_KONEC

				case IDM_NOVAZPRAVA_PRAZDNA: {
					SetWindowText(g.hEditMsg, L"");
					EndDialog(hDlg, 0);
					DisplayDlg(MAIN, 0, false);
				}break; // end case IDM_NOVAZPRAVA_PRAZDNA

				case IDM_NOVAZPRAVA_PREDCHOZI: {
					EndDialog(hDlg, 0);
					DisplayDlg(MAIN, 0, false);
				}break; // end case IDM_NOVAZPRAVA_PREDCHOZI

				case IDM_OPAKOVAT: {
					DisplayDlg(WAIT, true, false);
					EndDialog(hDlg, 0);

					if (g.sendMsgPictogram) free(g.sendMsgPictogram);
					g.sendMsgPictogram = NULL;

					SendSMS(true);
					//OpenLastPageByThread();
				}break; // end case IDM_OPAKOVAT

				case IDM_MOZNOSTI_DALSIZPRAVY: {
					EndDialog(hDlg, 0);
					g.msgsReadPos += g.msgsReadPage;
					ShowMsgsRead();
				}break; // end case IDM_MOZNOSTI_DALSIZPRAVY

				case IDM_MOZNOSTI_PREDCHOZIZPRAVY: {
					EndDialog(hDlg, 0);
					g.msgsReadPos = max(g.msgsReadPos - g.msgsReadPage, 0);
					ShowMsgsRead();
				}break; // end case IDM_MOZNOSTI_PREDCHOZIZPRAVY

				case IDM_MOZNOSTI_PRESUNOUTSEO: {
					static CDlgJumpOver jo(1, g.msgsReadPage);
					if (DialogBoxParam(g.hInstance, (LPCWSTR)IDD_JUMPOVER, hDlg, (DLGPROC)DlgJumpOverProc, (LPARAM)&jo)) {
						g.msgsReadPos = max(g.msgsReadPos + jo.step, 0);
						g.msgsReadPage = max(jo.pageSize, 1);

						EndDialog(hDlg, 0);
						ShowMsgsRead();
					}else {
						InitDlgStatus(hDlg, ds->caption, ds->idMenu);
					}
				}break; // end case IDM_MOZNOSTI_PRESUNOUTSEO

				case IDM_MOZNOSTI_VYHLEDAT: {
					INITCOMMONCONTROLSEX ic;
					ic.dwSize = sizeof(ic);
					ic.dwICC = ICC_DATE_CLASSES;
					InitCommonControlsEx(&ic);

					static CDlgSearch srch(g.msgsReadCont, g.msgsReadSearch_text, g.msgsReadStartTime, g.msgsReadEndTime);
					if (DialogBoxParam(g.hInstance, (LPCWSTR)IDD_SEARCH, hDlg, (DLGPROC)DlgSearchProc, (LPARAM)&srch)) {
						SetCharPtr(&g.msgsReadCont, srch.GetCont());
						SetCharPtr(&g.msgsReadSearch_text, srch.GetSearch_text());
						g.msgsReadStartTime = srch.startTime;
						g.msgsReadEndTime = srch.endTime;
						g.msgsReadPos = 0;

						EndDialog(hDlg, 0);
						ShowMsgsRead();
					}else {
						InitDlgStatus(hDlg, ds->caption, ds->idMenu);
					}
				}break; // end case IDM_MOZNOSTI_VYHLEDAT

				case IDM_MOZNOSTI_ZKOPIROVATPRVNIZPRAVU:
				case IDM_MOZNOSTI_ZKOPIROVATVSE: {
					EndDialog(hDlg, 0);
					WCHAR *text = _wcsdup(ds->resultText);
					if (LOWORD(wParam) == IDM_MOZNOSTI_ZKOPIROVATPRVNIZPRAVU) {
						memcpy(text, text + g.msgsReadMsg_0_start, sizeof(WCHAR) * g.msgsReadMsg_0_length);
						text[g.msgsReadMsg_0_length] = '\0';
					}
					DisplayDlg(MAIN, 0, false);
					
					WCHAR *p = CorrectEnters(text);
					SendMessage(g.hMain, USER_INSERTTEXT, -1, (LPARAM)p);
					free(p);

					free(text);
				}break; // end case IDM_MOZNOSTI_ZKOPIROVATPRVNIZPRAVU || IDM_MOZNOSTI_ZKOPIROVATVSE

				case IDM_MOZNOSTI_OBNOVIT: {
					EndDialog(hDlg, 0);
					ShowMsgsRead();
				}break; // end case IDM_MOZNOSTI_OBNOVIT
			}
		}break; // end case WM_COMMAND
	}

	HandleWindowScroll(hDlg, msg, wParam, lParam);

	return false;
}


/***********************************************************************************
* Initialize the dialog                                                            *
***********************************************************************************/
BOOL InitDlgStatus(HWND hWnd, WCHAR *caption, UINT idMenu) {
	// initialize dialog box, specifying that it should stretch full screen
	SHINITDLGINFO sid;
	ZeroMemory(&sid, sizeof(sid));
	sid.dwMask  = SHIDIM_FLAGS;
	sid.dwFlags = SHIDIF_SIZEDLGFULLSCREEN;
	sid.hDlg    = hWnd;

	// initialize the dialog
	if (!SHInitDialog(&sid)) return false;

	// set the title bar
	WCHAR b[100];
	wsprintf(b, L"%s::%s", g.szApp, caption);
	SetWindowText(hWnd, b);


	// initialize menubar
	SHMENUBARINFO mbi;
	ZeroMemory(&mbi, sizeof(SHMENUBARINFO));
	mbi.cbSize      = sizeof(SHMENUBARINFO);
	mbi.hwndParent  = hWnd;
	mbi.nToolBarId  = idMenu;
	mbi.hInstRes    = g.hInstance;

	if (!SHCreateMenuBar(&mbi)) return false;

	//SendMessage( g_hWndMenuBarCurrent, TB_SETBUTTONINFO, uiCurSkId, (LPARAM)(LPTBBUTTONINFO)&buttonInfo );

	// Overriding Back button functionality
	SendMessage(mbi.hwndMB, SHCMBM_OVERRIDEKEY, VK_TBACK, 
				  MAKELPARAM(SHMBOF_NODEFAULT | SHMBOF_NOTIFY, 
							 SHMBOF_NODEFAULT | SHMBOF_NOTIFY));

	ShowWindow(g.hMenuBarCurrent, SW_HIDE);
	g.hMenuBarCurrent = mbi.hwndMB;
	ShowWindow(g.hMenuBarCurrent, SW_SHOW);


	return true;
}


/***********************************************************************************
* Thread                                                                           *
***********************************************************************************/
DWORD WINAPI ThreadOpenPage(LPVOID lParam) {
	if (!lParam) return 0;

	CViewPage *page = ((CViewPage *)lParam)->Duplicate();
	WCHAR *result = NULL, *extras = NULL;
	BOOL ok = GetGiboURL(page->GetPage(), &result, &extras, page->callback);
	page->SetResultText(result);
	page->SetExtras(extras);
	page->SetSucceed(ok);
	
	SendMessage(page->GetNotifyHwnd(), page->GetNotifyMessage(), page->GetNotifyWParam(), (LPARAM)page);

	return 0;
}


/***********************************************************************************
* Opens and url by thread                                                          *
***********************************************************************************/
DWORD OpenPageByThread(char *url, WCHAR *caption, HWND notifyHwnd, UINT notifyMessage, WPARAM wParam, BOOL (* callback)(WPARAM wParam, LPARAM lParam)) {
	DWORD id;
	g.lastRequestedPage.SetPage(url);
	g.lastRequestedPage.SetNotifyHwnd(notifyHwnd);
	g.lastRequestedPage.SetNotifyMessage(notifyMessage);
	g.lastRequestedPage.SetCallback(callback);
	g.lastRequestedPage.SetNotifyWParam(wParam);
	g.lastRequestedPage.SetCaption(caption);

	CreateThread(NULL, 0, ThreadOpenPage, (void *)&g.lastRequestedPage, 0, &id);
	return id;
}


/***********************************************************************************
* Opens and url by thread                                                          *
***********************************************************************************/
DWORD OpenLastPageByThread() {
	DWORD id;

	CreateThread(NULL, 0, ThreadOpenPage, (void *)&g.lastRequestedPage, 0, &id);
	return id;
}


/***********************************************************************************
* Return whether current thread should continue with sending                       *
***********************************************************************************/
BOOL ContinueWithSending(WPARAM wParam, LPARAM lParam) {
	/*DWORD currThrd = GetCurrentThreadId();
	// return true when the current thread isn't discarded
	for (DWORD dlgID = 0; dlgID < g.hDlgSend.num; dlgID++) {
		if (g.hDlgSend[dlgID].threadID == currThrd) {
			return !g.hDlgSend[dlgID].discardSending;
		}
	}*/

	return true;
}


/***********************************************************************************
* Edit box message handler                                                         *
***********************************************************************************/
LRESULT CALLBACK CtrlListViewProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
		case WM_KEYUP: {
			PostMessage(GetParent(hWnd), USER_KEYUP, wParam, lParam);
		}break; // end case WM_KEYUP

		case WM_KEYDOWN: {
			PostMessage(GetParent(hWnd), USER_KEYDOWN, wParam, lParam);
		}break; // end case WM_KEYDOWN
	}
	WNDPROC wndProc = g.oldWndProcs.GetProc(hWnd);
	return wndProc ? CallWindowProc(wndProc, hWnd, msg, wParam, lParam) : 0;
}


/***********************************************************************************
* Dlg message handler                                                              *
***********************************************************************************/
LRESULT DlgInsertSmileyProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam) {
	static CHdc hStage(false), hSmileys(false);
	static BOOL smileysLoaded = false;
	static int lastSmileyIndex = -1;
	static int offsetX = 0, offsetY = 0;

	switch (msg) {
		case WM_INITDIALOG: {
			if (!InitDlgInsertSmiley(hDlg)) {
				DisplayDlg(MAIN, 0, false);
			}else {
				if (!smileysLoaded) {
					HBITMAP bmp = NULL;
					WCHAR *smileysFile = GetApplicationFile(L"smileys.bmp");
					if (FileExists(smileysFile)) {
						bmp = (HBITMAP)LoadImage(NULL, smileysFile, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
						/*int e = GetLastError();
						WCHAR a[20];
						if (!bmp) MessageBox(0, smileysFile, _ltow(e, a, 10), 0);*/
					}
					if (!bmp) {
						bmp = LoadBitmap(g.hInstance, (LPCWSTR)IDB_SMILEYS);
					}
					delete[] smileysFile;
					hSmileys.LoadBitmap(bmp);
					smileysLoaded = true;
				}
				hStage.SetSize(hSmileys.GetWidth(), hSmileys.GetHeight());
				BitBlt(hStage, 0, 0, hSmileys.GetWidth(), hSmileys.GetHeight(), hSmileys, 0, 0, SRCCOPY);
				
				RECT rc;
				GetClientRect(hDlg, &rc);
				offsetX = rc.right / 2 - hSmileys.GetWidth() / 2;
				offsetY = rc.bottom / 2 - hSmileys.GetHeight() / 2;

				SendMessage(hDlg, USER_FOCUSSMILEY, -1, lastSmileyIndex = 4);

				DelayKeyboard();
			}
		}break; // end case WM_INITDIALOG

		case WM_CLOSE: {
			hStage.LoadBitmap((HBITMAP)NULL);
		}break; // end case WM_DESTROY

		case USER_FOCUSSMILEY: {
			RECT rc;
			int cx = hStage.GetWidth() / 3;
			int cy = hStage.GetHeight() / 4;
			if (wParam != -1) {
				SetRect(&rc, wParam % 3 * cx, wParam / 3 * cy, wParam % 3 * cx + cx, wParam / 3 * cy + cy);
				DrawFocusRect(hStage, &rc);
			}
			SetRect(&rc, lParam % 3 * cx, lParam / 3 * cy, lParam % 3 * cx + cx, lParam / 3 * cy + cy);
			DrawFocusRect(hStage, &rc);

			InvalidateRect(hDlg, NULL, false);
			UpdateWindow(hDlg);
		}break; // end case USER_FOCUSSMILEY

		case USER_INSERTSMILEY: {
			DisplayDlg(MAIN, 0, false);

			
			BOOL ok = false;

			WCHAR *smileysFile = GetApplicationFile(L"smileys.txt");
			if (FileExists(smileysFile)) {
				WCHAR **smileys = NULL;

				FILE *fp = _wfopen(smileysFile, L"rb");
				if (fp) {
					char dataA[1024];
					int len = fread(dataA, 1, 1024, fp);
					dataA[len] = '\0';
					WCHAR *data = ConvertToWideChar(dataA, CP_ACP);
					DWORD num = 0;
					smileys = explode(data, L" ", &num, true);
					
					if (lastSmileyIndex != -1 && lastSmileyIndex < (int)num)
						SendMessage(g.hMain, USER_INSERTTEXT, -1, (LPARAM)smileys[lastSmileyIndex]);
					
					delete[] data;
					fclose(fp);

					ok = true;
				}
			}
			delete[] smileysFile;

			WCHAR *smileys[] = {L":-D", L":/", L":P",
												L";-)", L":-)", L":-(",
												L"8-)", L"%-)", L":,-(",
												L":-*", L"hm?", L"c(_)"};
			if (!ok && lastSmileyIndex != -1) SendMessage(g.hMain, USER_INSERTTEXT, -1, (LPARAM)smileys[lastSmileyIndex]);
		}break; // end case USER_INSERTSMILEY

		case WM_LBUTTONDOWN: {
			POINT pt;
			pt.x = LOWORD(lParam) - offsetX;
			pt.y = HIWORD(lParam) - offsetY;

			int nx = pt.x / (hStage.GetWidth() / 3);
			int ny = pt.y / (hStage.GetHeight() / 4);

			lastSmileyIndex = min(ny * 3 + nx, 11);

			SendMessage(hDlg, USER_INSERTSMILEY, 0, 0);
		}break; // end case WM_LBUTTONDOWN

		case WM_HOTKEY: {
			if (VK_TBACK == HIWORD(lParam) && !(lParam & MOD_KEYUP)) {
				DisplayDlg(MAIN, 0, false);
			}
		}break;

		case WM_CHAR: {
			SendMessage(hDlg, WM_COMMAND, IDOK, 0);
		}break; // end case WM_CHAR

		case WM_KEYDOWN: {
			if (g.skipUntilTime < GetTickCount()) {
				if (wParam == VK_LEFT) {
					if (lastSmileyIndex % 3 > 0) {
						SendMessage(hDlg, USER_FOCUSSMILEY, lastSmileyIndex, lastSmileyIndex - 1);
						lastSmileyIndex -= 1;
					}
				}else if (wParam == VK_RIGHT) {
					if (lastSmileyIndex % 3 < 2) {
						SendMessage(hDlg, USER_FOCUSSMILEY, lastSmileyIndex, lastSmileyIndex + 1);
						lastSmileyIndex += 1;
					}
				}else if (wParam >= '0' && wParam <= '9') {
					int newSmileyIndex = wParam == '0' ? 10 : wParam - '0' - 1;
					if (newSmileyIndex != lastSmileyIndex) {
						SendMessage(hDlg, USER_FOCUSSMILEY, lastSmileyIndex, newSmileyIndex);
						lastSmileyIndex = newSmileyIndex;
					}
				}else if (wParam == VK_UP) {
					if (lastSmileyIndex / 3 > 0) {
						SendMessage(hDlg, USER_FOCUSSMILEY, lastSmileyIndex, lastSmileyIndex - 3);
						lastSmileyIndex -= 3;
					}
				}else if (wParam == VK_DOWN) {
					if (lastSmileyIndex / 3 < 3) {
						SendMessage(hDlg, USER_FOCUSSMILEY, lastSmileyIndex, lastSmileyIndex + 3);
						lastSmileyIndex += 3;
					}
				}else if (wParam == VK_RETURN) {
					SendMessage(hDlg, WM_COMMAND, IDOK, 0);
				}
			}
		}break; // end case WM_KEYDOWN

		case WM_PAINT: {
			PAINTSTRUCT ps;
			BeginPaint(hDlg, &ps);
			RECT rc;
			GetClientRect(hDlg, &rc);
			BitBlt(ps.hdc, 0, 0, rc.right, rc.bottom, NULL, 0, 0, WHITENESS);
			BitBlt(ps.hdc, offsetX, offsetY, hStage.GetWidth(), hStage.GetHeight(), hStage, 0, 0, SRCCOPY);
			EndPaint(hDlg, &ps);
		}break; // end case WM_PAINT

		case WM_COMMAND: {
			switch (LOWORD(wParam)) {
				case IDOK: {
					if (g.skipUntilTime < GetTickCount()) {
						DelayKeyboard();

						SendMessage(hDlg, USER_INSERTSMILEY, 0, 0);
					}
				}break; // end case IDOK

				case IDCANCEL: {
					DisplayDlg(MAIN, 0, false);
				}break; // end case IDM_MOZNOSTI_KONEC
			}
		}break; // end case WM_COMMAND
	}

	HandleWindowScroll(hDlg, msg, wParam, lParam);

	return DefWindowProc(hDlg, msg, wParam, lParam);
}


/***********************************************************************************
* Shows wait dialog                                                                *
***********************************************************************************/
BOOL InitDlgInsertSmiley(HWND hWnd) {
	// set the title bar
	SetWindowText(hWnd, L"Vložit smajlíka...");

	// initialize menubar
	SHMENUBARINFO mbi;
	ZeroMemory(&mbi, sizeof(SHMENUBARINFO));
	mbi.cbSize      = sizeof(SHMENUBARINFO);
	//mbi.dwFlags = SHCMBF_EMPTYBAR;
	mbi.hwndParent  = hWnd;
	mbi.nToolBarId  = IDR_MENU_INSERTSMILEY;
	mbi.hInstRes    = g.hInstance;

	if (!SHCreateMenuBar(&mbi)) return false;

	// Overriding Back button functionality
	SendMessage(mbi.hwndMB, SHCMBM_OVERRIDEKEY, VK_TBACK, 
				  MAKELPARAM(SHMBOF_NODEFAULT | SHMBOF_NOTIFY, 
							 SHMBOF_NODEFAULT | SHMBOF_NOTIFY));


	ShowWindow(g.hMenuBarCurrent, SW_HIDE);
	g.hMenuBarCurrent = mbi.hwndMB;
	ShowWindow(g.hMenuBarCurrent, SW_SHOW);

	return true;
}


/***********************************************************************************
* Dlg message handler                                                              *
***********************************************************************************/
LRESULT DlgInformationProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam) {
	static XPhonebook *pb = new XPhonebook();
	static BOOL initialized = false;
	static CHdc hdcItemStatistika(false);
	static CHdc hdcSelectedItemStatistika(false);
	static CHdc hdcItemAktuality(false);
	static CHdc hdcSelectedItemAktuality(false);
	static CHdc hdcItemAktivace(false);
	static CHdc hdcSelectedItemAktivace(false);
	static CHdc hdcItemTM(false);
	static CHdc hdcSelectedItemTM(false);
	static CHdc hdcItemOProgramu(false);
	static CHdc hdcSelectedItemOProgramu(false);
	static CHdc hdcItemNovinky(false);
	static CHdc hdcSelectedItemNovinky(false);
	static CHdc hdcItemVseobecne(false);
	static CHdc hdcSelectedItemVseobecne(false);
	static CHdc hdcBackground(false);
	static CFont fntItem;
	static CFont fntSelectedItem;

	switch (msg) {
		case WM_INITDIALOG: {
			if (!InitDlgInformation(hDlg)) {
				DisplayDlg(MAIN, 0, false);
			}else {
				if (lParam) {
					if (!initialized) {
						CreateItem(&hdcItemStatistika, LoadBitmap(g.hInstance, (LPCWSTR)IDB_ITEMSTATISTIKA), LoadBitmap(g.hInstance, (LPCWSTR)IDB_ITEMTILE));
						CreateItem(&hdcSelectedItemStatistika, LoadBitmap(g.hInstance, (LPCWSTR)IDB_SELECTEDITEMSTATISTIKA), LoadBitmap(g.hInstance, (LPCWSTR)IDB_SELECTEDITEMTILE));
						CreateItem(&hdcItemAktuality, LoadBitmap(g.hInstance, (LPCWSTR)IDB_ITEMAKTUALITY), LoadBitmap(g.hInstance, (LPCWSTR)IDB_ITEMTILE));
						CreateItem(&hdcSelectedItemAktuality, LoadBitmap(g.hInstance, (LPCWSTR)IDB_SELECTEDITEMAKTUALITY), LoadBitmap(g.hInstance, (LPCWSTR)IDB_SELECTEDITEMTILE));
						CreateItem(&hdcItemAktivace, LoadBitmap(g.hInstance, (LPCWSTR)IDB_ITEMAKTIVACE), LoadBitmap(g.hInstance, (LPCWSTR)IDB_ITEMTILE));
						CreateItem(&hdcSelectedItemAktivace, LoadBitmap(g.hInstance, (LPCWSTR)IDB_SELECTEDITEMAKTIVACE), LoadBitmap(g.hInstance, (LPCWSTR)IDB_SELECTEDITEMTILE));
						CreateItem(&hdcItemTM, LoadBitmap(g.hInstance, (LPCWSTR)IDB_ITEMTM), LoadBitmap(g.hInstance, (LPCWSTR)IDB_ITEMTILE));
						CreateItem(&hdcSelectedItemTM, LoadBitmap(g.hInstance, (LPCWSTR)IDB_SELECTEDITEMTM), LoadBitmap(g.hInstance, (LPCWSTR)IDB_SELECTEDITEMTILE));
						CreateItem(&hdcItemOProgramu, LoadBitmap(g.hInstance, (LPCWSTR)IDB_ITEMOPROGRAMU), LoadBitmap(g.hInstance, (LPCWSTR)IDB_ITEMTILE));
						CreateItem(&hdcSelectedItemOProgramu, LoadBitmap(g.hInstance, (LPCWSTR)IDB_SELECTEDITEMOPROGRAMU), LoadBitmap(g.hInstance, (LPCWSTR)IDB_SELECTEDITEMTILE));
						CreateItem(&hdcItemNovinky, LoadBitmap(g.hInstance, (LPCWSTR)IDB_ITEMNOVINKY), LoadBitmap(g.hInstance, (LPCWSTR)IDB_ITEMTILE));
						CreateItem(&hdcSelectedItemNovinky, LoadBitmap(g.hInstance, (LPCWSTR)IDB_SELECTEDITEMNOVINKY), LoadBitmap(g.hInstance, (LPCWSTR)IDB_SELECTEDITEMTILE));
						CreateItem(&hdcItemVseobecne, LoadBitmap(g.hInstance, (LPCWSTR)IDB_ITEMVSEOBECNE), LoadBitmap(g.hInstance, (LPCWSTR)IDB_ITEMTILE));
						CreateItem(&hdcSelectedItemVseobecne, LoadBitmap(g.hInstance, (LPCWSTR)IDB_SELECTEDITEMVSEOBECNE), LoadBitmap(g.hInstance, (LPCWSTR)IDB_SELECTEDITEMTILE));

						fntItem.CreateFont(hDlg, 10, L"MS Sans Serif", false, false, false);
						fntSelectedItem.CreateFont(hDlg, 10, L"MS Sans Serif", false, false, false);

						pb->SetGraphics(NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, &fntItem, &fntSelectedItem, NULL);

						RECT rc;
						GetClientRect(hDlg, &rc);

						pb->SetDialogSize(rc.right - rc.left, rc.bottom - rc.top);

						// nastavit pozadí s logem
						hdcBackground.SetSize(rc.right - rc.left, rc.bottom - rc.top);
						pb->SetBackground(&hdcBackground, 0);
						DrawBackgroundLogo(&hdcBackground);

						pb->SetAllowInputItem(false);

						XPhonebookItem *item = new XPhonebookItem;
						
						item->SetText(L"Statistika");
						item->SetGraphics(&hdcItemStatistika, &hdcSelectedItemStatistika);
						pb->Add(item);

						item = new XPhonebookItem;
						item->SetText(L"Vaše aktuality");
						item->SetGraphics(&hdcItemAktuality, &hdcSelectedItemAktuality);
						pb->Add(item);

						item = new XPhonebookItem;
						item->SetText(L"Aktivace");
						item->SetGraphics(&hdcItemAktivace, &hdcSelectedItemAktivace);
						pb->Add(item);

						item = new XPhonebookItem;
						item->SetText(L"Volné TM SMS");
						item->SetGraphics(&hdcItemTM, &hdcSelectedItemTM);
						pb->Add(item);

						item = new XPhonebookItem;
						item->SetText(L"O programu...");
						item->SetGraphics(&hdcItemOProgramu, &hdcSelectedItemOProgramu);
						pb->Add(item);

						item = new XPhonebookItem;
						item->SetText(L"Co je nového?");
						item->SetGraphics(&hdcItemNovinky, &hdcSelectedItemNovinky);
						pb->Add(item);

						item = new XPhonebookItem;
						item->SetText(L"Všeobecné");
						item->SetGraphics(&hdcItemVseobecne, &hdcSelectedItemVseobecne);
						pb->Add(item);

						initialized = true;
					}

					pb->SetSelectedItem(0, NOT_SELECTED);

					pb->Redraw();
					InvalidateRect(hDlg, NULL, false);
					UpdateWindow(hDlg);

					DelayKeyboard();
				}
			}
		}break; // end case WM_INITDIALOG

		case WM_HOTKEY: { // Override the back key to delete
			if (HIWORD(lParam) == VK_TBACK && !(lParam & MOD_KEYUP)) {
				SendMessage(hDlg, WM_COMMAND, IDM_ZPET, 0);
			}
		}break;

		case WM_KEYDOWN: {
			if (g.skipUntilTime < GetTickCount()) {
				if (wParam == VK_RETURN) {
					SendMessage(hDlg, WM_COMMAND, IDM_VYBRAT, 0);
				}
			}
		}break; // end case USER_USERKEYDOWN

		case WM_COMMAND: {
			switch (LOWORD(wParam)) {
				case IDM_ZPET: {
					DisplayDlg(MAIN, 0, false);
				}break; // end case IDM_MOZNOSTI_KONEC

				case IDM_VYBRAT: {
					int selItem = pb->GetSelectedItemIndex();;
					if (selItem != NOT_SELECTED) {
						if (selItem != 4) {
							g.AllocateUrl(1024*10);
							WCHAR *caption = NULL;
							
							char *encPwd = EncodeURL(g.setting.GetPwd());
							
							if (selItem == 0) {caption = L"Statistika"; sprintf(g.url, "%s/statistika.php?num=%s&pwd=%s&client=%s&ver=%s", g.giboServer, g.setting.GetSrcNum(), encPwd, g.client, g.szVersionA);}
							else if (selItem == 1) {caption = L"Vaše aktuality"; sprintf(g.url, "%s/aktuality.php?num=%s&pwd=%s&client=%s&ver=%s", g.giboServer, g.setting.GetSrcNum(), encPwd, g.client, g.szVersionA);}
							else if (selItem == 2) {caption = L"Aktivace"; sprintf(g.url, "%s/info_aktivace.php?client=%s&ver=%s", g.giboServer, g.client, g.szVersionA);}
							else if (selItem == 3) {caption = L"Volné T-Mobile SMS"; sprintf(g.url, "%s/t-mobile.php?num=%s&pwd=%s&action=getfreesms&client=%s&ver=%s", g.giboServer, g.setting.GetSrcNum(), encPwd, g.client, g.szVersionA);}
							else if (selItem == 5) {caption = L"Co je nového?"; sprintf(g.url, "%s/whatsnew.php?client=%s&ver=%s", g.giboServer, g.client, g.szVersionA);}
							else if (selItem == 6) {caption = L"Všeobecné informace"; sprintf(g.url, "%s/info_general.php?client=%s&ver=%s", g.giboServer, g.client, g.szVersionA);}
							
							free(encPwd);

							DisplayDlg(WAIT, true, true);
							OpenPageByThread(g.url, caption, g.hMain, USER_FINISHDOWNLOADING, CDlgStatus::TYPE::SHOWINFO, ContinueWithSending);
						}else if (selItem == 4) {
							CDlgStatus ds;
							ds.caption = L"Informace o programu";
							ds.type = CDlgStatus::TYPE::SHOWINFO;
							ds.showSucceedPic = false;
							ds.succeed = true;
							ds.playSound = false;
							ds.resultText = new WCHAR[1024*3];
							ds.idMenu = IDR_MENU_STATUS_BACKEND;
							ds.quitCountdown = false;
							wsprintf(ds.resultText, L"%s\r\n\r\nVerze: %s\r\n\r\nWeb stranka: http://www.gibosms.cz\r\n\r\nDiskuzni forum: http://www.gibosms.cz/forum\r\n\r\ne-mail: gibo@gibosms.cz\r\n\r\nICQ: 239553554", g.szApp, g.szVersion);

							g.displayPrev = g.display;
							// show status dialog
							DialogBoxParam(g.hInstance, MAKEINTRESOURCE(IDD_STATUS), 0, (DLGPROC)DlgStatusProc, (LPARAM)&ds);

							delete[] ds.resultText;
						}
					}else MessageBox(hDlg, L"Nejprve musíte vybrat nìjakou položku", g.szAppErr, MB_OK|MB_ICONSTOP);

					DelayKeyboard();
				}break; // end case IDM_VYBRAT
			}
		}break; // end case WM_COMMAND

		case WM_LBUTTONUP:
		case WM_LBUTTONDBLCLK: {
			SendMessage(hDlg, WM_COMMAND, IDM_VYBRAT, 0);
		}break; // end case WM_LBUTTONDBLCLK
	}

	pb->OnMsg(hDlg, msg, wParam, lParam);

	HandleWindowScroll(hDlg, msg, wParam, lParam);

	return DefWindowProc(hDlg, msg, wParam, lParam);
}


/***********************************************************************************
* Shows wait dialog                                                                *
***********************************************************************************/
BOOL InitDlgInformation(HWND hWnd) {
	// initialize dialog box, specifying that it should stretch full screen
	SHINITDLGINFO sid;
	ZeroMemory(&sid, sizeof(sid));
	sid.dwMask  = SHIDIM_FLAGS;
	sid.dwFlags = SHIDIF_SIZEDLGFULLSCREEN;
	sid.hDlg    = hWnd;

	// initialize the dialog
	if (!SHInitDialog(&sid)) return false;

	// set the title bar
	SetWindowText(hWnd, L"Informace");

	// initialize menubar
	SHMENUBARINFO mbi;
	ZeroMemory(&mbi, sizeof(SHMENUBARINFO));
	mbi.cbSize      = sizeof(SHMENUBARINFO);
	mbi.hwndParent  = hWnd;
	mbi.nToolBarId  = IDR_MENU_INFORMATION;
	mbi.hInstRes    = g.hInstance;

	if (!SHCreateMenuBar(&mbi)) return false;

	// Overriding Back button functionality
	SendMessage(mbi.hwndMB, SHCMBM_OVERRIDEKEY, VK_TBACK, 
				  MAKELPARAM(SHMBOF_NODEFAULT | SHMBOF_NOTIFY, 
							 SHMBOF_NODEFAULT | SHMBOF_NOTIFY));


	ShowWindow(g.hMenuBarCurrent, SW_HIDE);
	g.hMenuBarCurrent = mbi.hwndMB;
	ShowWindow(g.hMenuBarCurrent, SW_SHOW);

	return true;
}


/***********************************************************************************
* Dlg message handler                                                              *
***********************************************************************************/
BOOL CALLBACK DlgEmailSubjectProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
		case WM_INITDIALOG: {
			if (!InitDlgEmailSubject(hDlg)) {
				EndDialog (hDlg, -1);
			}else {
				SetFocus(GetDlgItem(hDlg, IDC_EDIT_SUBJECT));
			}
		}break; // end case WM_INITDIALOG

		case WM_HOTKEY: { // Override the back key to delete
			if (VK_TBACK == HIWORD(lParam)) {
				#ifndef PPC
					SHSendBackToFocusWindow(msg, wParam, lParam);
				#endif
			}
		}break;

		case WM_CLOSE: {
			EndDialog(hDlg, 0);
		}break; // case WM_CLOSE

		case WM_COMMAND: {
			switch (LOWORD(wParam)) {
				case IDOK: {
					WCHAR *subject = GetWinText(GetDlgItem(hDlg, IDC_EDIT_SUBJECT));
					EndDialog(hDlg, (int)subject);
				}break; // end case IDOK

				case IDCANCEL: {
					EndDialog(hDlg, NULL);
				}break; // end case IDCANCEL:
			}
		}break; // end case WM_COMMAND
	}

	HandleWindowScroll(hDlg, msg, wParam, lParam);

	return false;
}


/***********************************************************************************
* Initialize the dialog                                                            *
***********************************************************************************/
BOOL InitDlgEmailSubject(HWND hWnd) {
	// initialize dialog box, specifying that it should stretch full screen
	SHINITDLGINFO sid;
	ZeroMemory(&sid, sizeof(sid));
	sid.dwMask  = SHIDIM_FLAGS;
	sid.dwFlags = SHIDIF_SIZEDLGFULLSCREEN;
	sid.hDlg    = hWnd;

	// initialize the dialog
	if (!SHInitDialog(&sid)) return false;

	// set the title bar
	SetWindowText(hWnd, L"Pøedmìt E-mailu");


	// initialize menubar
	SHMENUBARINFO mbi;
	ZeroMemory(&mbi, sizeof(SHMENUBARINFO));
	mbi.cbSize      = sizeof(SHMENUBARINFO);
	mbi.hwndParent  = hWnd;
	mbi.nToolBarId  = IDR_MENU_EMAILSUBJECT;
	mbi.hInstRes    = g.hInstance;

	if (!SHCreateMenuBar(&mbi)) return false;

	// Overriding Back button functionality
	SendMessage(mbi.hwndMB, SHCMBM_OVERRIDEKEY, VK_TBACK, 
				  MAKELPARAM(SHMBOF_NODEFAULT | SHMBOF_NOTIFY, 
							 SHMBOF_NODEFAULT | SHMBOF_NOTIFY));

	ShowWindow(g.hMenuBarCurrent, SW_HIDE);
	g.hMenuBarCurrent = mbi.hwndMB;
	ShowWindow(g.hMenuBarCurrent, SW_SHOW);


	return true;
}


/***********************************************************************************
* Makes delay when all user keyboard clicks are omitted                            *
***********************************************************************************/
void DelayKeyboard() {
#ifndef DEBUG
	g.skipUntilTime = GetTickCount() + 100; // it must be delayed becaused it thinks that user pressed the key althought it's the old click
#else
	g.skipUntilTime = GetTickCount() + 500; // it must be delayed becaused it thinks that user pressed the key althought it's the old click
#endif
}


/***********************************************************************************
* Dlg message handler                                                              *
***********************************************************************************/
LRESULT DlgImportPhonebookProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam) {
	static XPhonebook *pb = new XPhonebook();
	static BOOL graphics_initialized = false;
	static CHdc hdcItem(false);
	static CHdc hdcSelectedItem(false);
	static CHdc hdcBackground(false);
	static CFont fntItem;
	static CFont fntSelectedItem;

	switch (msg) {
		case WM_INITDIALOG: {
			if (!InitDlgImportPhonebook(hDlg)) {
				DisplayDlg(MAIN, 0, false);
			}else {
				if (!graphics_initialized) {
					CreateItem(&hdcItem, LoadBitmap(g.hInstance, (LPCWSTR)IDB_ITEMDATA), LoadBitmap(g.hInstance, (LPCWSTR)IDB_ITEMTILE));
					CreateItem(&hdcSelectedItem, LoadBitmap(g.hInstance, (LPCWSTR)IDB_SELECTEDITEMDATA), LoadBitmap(g.hInstance, (LPCWSTR)IDB_SELECTEDITEMTILE));

					fntItem.CreateFont(hDlg, 10, L"MS Sans Serif", false, false, false);
					fntSelectedItem.CreateFont(hDlg, 10, L"MS Sans Serif", false, false, false);

					pb->SetGraphics(NULL, NULL, &hdcItem, &hdcSelectedItem, NULL, NULL, NULL, NULL, &fntItem, &fntSelectedItem, NULL);

					graphics_initialized = true;
				}

				RECT rc;
				GetClientRect(hDlg, &rc);

				pb->SetDialogSize(rc.right, rc.bottom);

				// nastavit pozadí s logem
				hdcBackground.SetSize(rc.right - rc.left, rc.bottom - rc.top);
				pb->SetBackground(&hdcBackground, 0);
				DrawBackgroundLogo(&hdcBackground);

				pb->SetAllowInputItem(false);

				pb->RemoveAllItemsData();

				// fill list with backups
				DWORD numLines;
				WCHAR *data = wcsdup((WCHAR *)lParam);
				WCHAR **lines = explode(data, L"\r\n", &numLines, true);
				for (DWORD i = 0; i < numLines; i++) {
					pb->Add(lines[i], 0);
				}
				free(data);

				pb->SetSelectedItem(0, NOT_SELECTED);
				pb->CenterSelectedItem();
				pb->Redraw();
				InvalidateRect(hDlg, NULL, false);
				UpdateWindow(hDlg);

				DelayKeyboard();
			}
		}break; // end case WM_INITDIALOG

		case WM_HOTKEY: { // Override the back key to delete
			if (HIWORD(lParam) == VK_TBACK && !(lParam & MOD_KEYUP)) {
				SendMessage(hDlg, WM_COMMAND, IDM_ZPET, 0);
			}
		}break;

		case WM_KEYDOWN: {
			if (g.skipUntilTime < GetTickCount()) {
				if (wParam == VK_RETURN) {
					SendMessage(hDlg, WM_COMMAND, IDM_VYBRAT, 0);
				}
			}
		}break; // end case USER_USERKEYDOWN

		case WM_COMMAND: {
			switch (LOWORD(wParam)) {
				case IDM_ZPET: {
					DisplayDlg(PHONEBOOK, 0, false);
				}break; // end case IDM_MOZNOSTI_KONEC

				case IDM_VYBRAT: {
					DWORD selItem = pb->GetSelectedItemDataIndex();
					if (selItem != NOT_SELECTED) {
						char *encPwd = EncodeURL(g.setting.GetPwd());
						g.AllocateUrl(1024*1);
						
						sprintf(g.url, "%s/pb_import.php?num=%s&pwd=%s&action=read&id=%i&client=%s&ver=%s", g.giboServer, g.setting.GetSrcNum(), encPwd, selItem, g.client, g.szVersionA);
						
						free(encPwd);

						DisplayDlg(WAIT, true, true);
						OpenPageByThread(g.url, L"Import adresáøe...", g.hMain, USER_FINISHDOWNLOADING, CDlgStatus::TYPE::IMPORTPHONEBOOK_READ, ContinueWithSending);
					}else MessageBox(hDlg, L"Nejprve musíte vybrat nìjakou položku", g.szAppErr, MB_OK|MB_ICONSTOP);

					DelayKeyboard();
				}break; // end case IDM_VYBRAT
			}
		}break; // end case WM_COMMAND

		case WM_LBUTTONUP:
		case WM_LBUTTONDBLCLK: {
			SendMessage(hDlg, WM_COMMAND, IDM_VYBRAT, 0);
		}break; // end case WM_LBUTTONDBLCLK
	}

	pb->OnMsg(hDlg, msg, wParam, lParam);

	HandleWindowScroll(hDlg, msg, wParam, lParam);

	return DefWindowProc(hDlg, msg, wParam, lParam);
}


/***********************************************************************************
* Shows wait dialog                                                                *
***********************************************************************************/
BOOL InitDlgImportPhonebook(HWND hWnd) {
	// set the title bar
	SetWindowText(hWnd, L"Vyberte zálohu adresáøe...");

	// initialize menubar
	SHMENUBARINFO mbi;
	ZeroMemory(&mbi, sizeof(SHMENUBARINFO));
	mbi.cbSize      = sizeof(SHMENUBARINFO);
	mbi.hwndParent  = hWnd;
	mbi.nToolBarId  = IDR_MENU_IMPORTPHONEBOOK;
	mbi.hInstRes    = g.hInstance;

	if (!SHCreateMenuBar(&mbi)) return false;

	// Overriding Back button functionality
	SendMessage(mbi.hwndMB, SHCMBM_OVERRIDEKEY, VK_TBACK, 
				  MAKELPARAM(SHMBOF_NODEFAULT | SHMBOF_NOTIFY, 
							 SHMBOF_NODEFAULT | SHMBOF_NOTIFY));


	ShowWindow(g.hMenuBarCurrent, SW_HIDE);
	g.hMenuBarCurrent = mbi.hwndMB;
	ShowWindow(g.hMenuBarCurrent, SW_SHOW);

	return true;
}


/***********************************************************************************
* Dlg message handler                                                              *
***********************************************************************************/
LRESULT DlgPhonebookProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam) {
	static CDlgAddressBook *ab = NULL;
	static CDlgAddressBook *lastAb = NULL;
	static XPhonebook *pb = new XPhonebook();
	static BOOL graphics_initialized = false;
	static CHdc hdcInputItem(false);
	static CHdc hdcSelectedInputItem(false);
	static CHdc hdcItem(false);
	static CHdc hdcSelectedItem(false);
	static CHdc hdcExpandableItem(false);
	static CHdc hdcSelectedExpandableItem(false);
	static CHdc hdcBackground(false);
	static CFont fntInputItem;
	static CFont fntSelectedInputItem;
	static CFont fntItem;
	static CFont fntSelectedItem;
	static CFont fntInputChar;
	static BOOL usePhonebook = false; // phonebook in phone (not application phonebook)

	switch (msg) {
		case WM_INITDIALOG: {
			ab = (CDlgAddressBook *)lParam;
			if (!ab) ab = lastAb;
			if (!ab) return false;
			lastAb = ab;

			if (!InitDlgPhonebook(hDlg, ab->type)) {
				DisplayDlg(MAIN, 0, false);
			}else {
				if (!graphics_initialized) {
					CreateItem(&hdcInputItem, LoadBitmap(g.hInstance, (LPCWSTR)IDB_INPUTITEM), LoadBitmap(g.hInstance, (LPCWSTR)IDB_INPUTITEMTILE));
					CreateItem(&hdcSelectedInputItem, LoadBitmap(g.hInstance, (LPCWSTR)IDB_SELECTEDINPUTITEM), LoadBitmap(g.hInstance, (LPCWSTR)IDB_SELECTEDINPUTITEMTILE));
					CreateItem(&hdcItem, LoadBitmap(g.hInstance, (LPCWSTR)IDB_ITEM), LoadBitmap(g.hInstance, (LPCWSTR)IDB_ITEMTILE));
					CreateItem(&hdcSelectedItem, LoadBitmap(g.hInstance, (LPCWSTR)IDB_SELECTEDITEM), LoadBitmap(g.hInstance, (LPCWSTR)IDB_SELECTEDITEMTILE));
					CreateItem(&hdcExpandableItem, LoadBitmap(g.hInstance, (LPCWSTR)IDB_EXPANDABLEITEM), LoadBitmap(g.hInstance, (LPCWSTR)IDB_ITEMTILE));
					CreateItem(&hdcSelectedExpandableItem, LoadBitmap(g.hInstance, (LPCWSTR)IDB_SELECTEDEXPANDABLEITEM), LoadBitmap(g.hInstance, (LPCWSTR)IDB_SELECTEDITEMTILE));

					fntInputItem.CreateFont(hDlg, 12, L"MS Sans Serif", true, false, false);
					fntSelectedInputItem.CreateFont(hDlg, 12, L"MS Sans Serif", true, false, false);
					fntItem.CreateFont(hDlg, 10, L"MS Sans Serif", false, false, false);
					fntSelectedItem.CreateFont(hDlg, 10, L"MS Sans Serif", false, false, false);
					fntInputChar.CreateFont(hDlg, 8, L"MS Sans Serif", false, false, false);

					pb->SetGraphics(&hdcInputItem, &hdcSelectedInputItem, &hdcItem, &hdcSelectedItem, &hdcExpandableItem, &hdcSelectedExpandableItem, &fntInputItem, &fntSelectedInputItem, &fntItem, &fntSelectedItem, &fntInputChar);

					graphics_initialized = true;
				}

				if (lParam) {
					RECT rc;
					GetClientRect(hDlg, &rc);

					pb->SetDialogSize(rc.right - rc.left, rc.bottom - rc.top);

					// nastavit pozadí s logem
					hdcBackground.SetSize(rc.right - rc.left, rc.bottom - rc.top);
					pb->SetBackground(&hdcBackground, 0);
					DrawBackgroundLogo(&hdcBackground);

					pb->SetAllowInputItem(true);

					pb->SetHideEmptyInputItem(true);

					pb->SetInputItemText(L"");

					pb->SetInput(ab->sendTo == CDlgAddressBook::SENDTO::SMS || ab->sendTo == CDlgAddressBook::SENDTO::WEBSMS || ab->sendTo == CDlgAddressBook::SENDTO::MOBEMAIL || ab->sendTo == CDlgAddressBook::SENDTO::MAILBOX || ab->sendTo == CDlgAddressBook::SENDTO::PAIDSMS ? XPhonebook::INPUT::NUMBERS : XPhonebook::INPUT::LETTERS);


					usePhonebook = !g.setting.useInternalPhonebook && !ab->type == CDlgAddressBook::TYPE::SHOW;

					SendMessage(hDlg, USER_REFRESHLIST, 0, 0);

					char *receiver = NULL;
					if (ab->type == CDlgAddressBook::TYPE::SEND) {
						if (ab->sendTo == CDlgAddressBook::SENDTO::SMS || ab->sendTo == CDlgAddressBook::SENDTO::WEBSMS || ab->sendTo == CDlgAddressBook::SENDTO::MOBEMAIL || ab->sendTo == CDlgAddressBook::SENDTO::MAILBOX || ab->sendTo == CDlgAddressBook::SENDTO::PAIDSMS) receiver = g.setting.GetLastDestNum();
						else if (ab->sendTo == CDlgAddressBook::SENDTO::EMAIL) receiver = g.setting.GetLastDestEmail();
					}
					SendMessage(hDlg, USER_SELECTBYRECEIVER, 0, (LPARAM)receiver);


					DelayKeyboard();
				}

				InvalidateRect(hDlg, NULL, false);
				UpdateWindow(hDlg);
			}
		}break; // end case WM_INITDIALOG

		case WM_INITMENUPOPUP: {
			CheckMenuRadioItem((HMENU)wParam, IDM_MOZNOSTI_ADRESARTELEFONU, IDM_MOZNOSTI_ADRESARAPLIKACE, usePhonebook ? IDM_MOZNOSTI_ADRESARTELEFONU : IDM_MOZNOSTI_ADRESARAPLIKACE, MF_BYCOMMAND);

			EnableMenuItem((HMENU)wParam, IDM_MOZNOSTI_ADRESARTELEFONU, MF_BYCOMMAND | (ab->type == CDlgAddressBook::TYPE::SEND ? MF_ENABLED : MF_GRAYED));
			EnableMenuItem((HMENU)wParam, IDM_MOZNOSTI_ADRESARAPLIKACE, MF_BYCOMMAND | (ab->type == CDlgAddressBook::TYPE::SEND ? MF_ENABLED : MF_GRAYED));
			
			EnableMenuItem((HMENU)wParam, IDM_MOZNOSTI_PRIDAT, MF_BYCOMMAND | (!usePhonebook ? MF_ENABLED : MF_GRAYED));
			EnableMenuItem((HMENU)wParam, IDM_MOZNOSTI_UPRAVIT, MF_BYCOMMAND | (!usePhonebook ? MF_ENABLED : MF_GRAYED));
			EnableMenuItem((HMENU)wParam, IDM_MOZNOSTI_SMAZAT, MF_BYCOMMAND | (!usePhonebook ? MF_ENABLED : MF_GRAYED));
			EnableMenuItem((HMENU)wParam, IDM_MOZNOSTI_SMAZATVSE, MF_BYCOMMAND | (!usePhonebook ? MF_ENABLED : MF_GRAYED));
			EnableMenuItem((HMENU)wParam, IDM_MOZNOSTI_IMPORT, MF_BYCOMMAND | (!usePhonebook ? MF_ENABLED : MF_GRAYED));
			EnableMenuItem((HMENU)wParam, IDM_MOZNOSTI_EXPORT, MF_BYCOMMAND | (!usePhonebook ? MF_ENABLED : MF_GRAYED));
		}break; // end case WM_INITMENUPOPUP

		case WM_KEYDOWN: {
			if (g.skipUntilTime < GetTickCount()) {
				if (wParam == VK_RETURN) {
					if (ab->type == CDlgAddressBook::TYPE::SEND) SendMessage(hDlg, WM_COMMAND, IDM_MOZNOSTI_POSLAT, 0);
					else if (ab->type == CDlgAddressBook::TYPE::SHOW) SendMessage(hDlg, WM_COMMAND, IDM_MOZNOSTI_VLOZITCISLO, 0);
				}
			}
		}break; // end case USER_USERKEYDOWN

		case WM_HOTKEY: { // Override the back key to delete
			if (VK_TBACK == HIWORD(lParam) && !(lParam & MOD_KEYUP)) {
				if (!pb->GetInputItemText() || !*pb->GetInputItemText()) {
					SendMessage(hDlg, WM_COMMAND, IDM_ZPET, 0);
				}
			}
		}break;

		case USER_REFRESHLIST: {
			pb->RemoveAllItemsData();

			static CPhonebook bookInPhone;
			CPhonebook *book = usePhonebook ? &bookInPhone : &g.phonebook;
			if (usePhonebook) {
				bookInPhone.RemoveAll();
				bookInPhone.ImportFromPhone(ab->sendTo == CDlgAddressBook::SENDTO::SMS || ab->sendTo == CDlgAddressBook::SENDTO::WEBSMS || ab->sendTo == CDlgAddressBook::SENDTO::MOBEMAIL || ab->sendTo == CDlgAddressBook::SENDTO::MAILBOX || ab->sendTo == CDlgAddressBook::SENDTO::PAIDSMS, ab->sendTo == CDlgAddressBook::SENDTO::EMAIL);
			}

			for (DWORD i = 0; i < book->GetItemCount(); i++) {
				CPhonebookItem *pi = book->Get(i);
				if (pi) {
					WCHAR *name = ConvertToWideChar(pi->GetName(), CP_ACP);
					
					DWORD index;
					if ((index = pb->Exists(name, false)) == 0xFFFFFFFF) {
						pb->Add(name, ab->sendTo == CDlgAddressBook::SENDTO::SMS || ab->sendTo == CDlgAddressBook::SENDTO::WEBSMS || ab->sendTo == CDlgAddressBook::SENDTO::MOBEMAIL || ab->sendTo == CDlgAddressBook::SENDTO::MAILBOX || ab->sendTo == CDlgAddressBook::SENDTO::PAIDSMS ? pi->GetNumber() : pi->GetEmail());
					}else {
						XPhonebookItem *xpi = pb->Get(index, false);
						if (xpi) {
							if (!xpi->GetSubItemsCount()) {
								XPhonebookItem *subItem = new XPhonebookItem;
								WCHAR *name = ConvertToWideChar((char *)xpi->GetData(), CP_ACP);
								subItem->SetText(name);
								subItem->SetData(xpi->GetData());
								delete[] name;

								xpi->AddSubItem(subItem);
							}
							WCHAR *val = ConvertToWideChar(ab->sendTo == CDlgAddressBook::SENDTO::SMS || ab->sendTo == CDlgAddressBook::SENDTO::WEBSMS || ab->sendTo == CDlgAddressBook::SENDTO::MOBEMAIL || ab->sendTo == CDlgAddressBook::SENDTO::MAILBOX || ab->sendTo == CDlgAddressBook::SENDTO::PAIDSMS ? pi->GetNumber() : pi->GetEmail(), CP_ACP);

							XPhonebookItem *subItem = new XPhonebookItem;
							subItem->SetText(val);
							subItem->SetData(ab->sendTo == CDlgAddressBook::SENDTO::SMS || ab->sendTo == CDlgAddressBook::SENDTO::WEBSMS || ab->sendTo == CDlgAddressBook::SENDTO::MOBEMAIL || ab->sendTo == CDlgAddressBook::SENDTO::MAILBOX || ab->sendTo == CDlgAddressBook::SENDTO::PAIDSMS ? pi->GetNumber() : pi->GetEmail());
							xpi->AddSubItem(subItem);
						}
					}
					delete[] name;
				}
			}
			
			pb->SetSelectedItem(0, NOT_SELECTED);
			pb->CenterSelectedItem();
			pb->Redraw();
			InvalidateRect(hDlg, NULL, false);
			UpdateWindow(hDlg);
		}break; // end case USER_REFRESHLIST

		case USER_SELECTBYNAME: {
			WCHAR *name = ConvertToWideChar((char *)lParam, CP_ACP);
			pb->SetSelectedItem(0, NOT_SELECTED);

			DWORD index = pb->Exists(name, true);
			delete[] name;
			if (index != 0xFFFFFFFF) {
				pb->SetSelectedItem(index, NOT_SELECTED);
			}

			pb->CenterSelectedItem();
			pb->Redraw();
			InvalidateRect(hDlg, NULL, false);
			UpdateWindow(hDlg);
		}break; // end case USER_SELECTBYNAME

		case USER_SELECTBYRECEIVER: {
			pb->SetSelectedItem(0, NOT_SELECTED);
			char *receiver = (char *)lParam;

			if (receiver) {
				for (DWORD i = 0; i < pb->GetItemCount(); i++) {
					XPhonebookItem *xpi = pb->Get(i, true);
					if (xpi) {
						if (!_stricmp((char *)xpi->GetData(), receiver)) {
							pb->SetSelectedItem(i, NOT_SELECTED);
							break;
						}
						for (DWORD n = 0; n < xpi->GetSubItemsCount(); n++) {
							XPhonebookItem *xpiSubItem = xpi->GetSubItem(n);
							if (xpiSubItem) {
								if (!_stricmp((char *)xpiSubItem->GetData(), receiver)) {
									pb->SetSelectedItem(i, n);
									xpi->SetExpanded(true);
									break;
								}
							}
						}
					}
				}
			}

			pb->CenterSelectedItem();
			pb->Redraw();
			InvalidateRect(hDlg, NULL, false);
			UpdateWindow(hDlg);
		}break; // end case USER_SELECTBYNUMBER

		case WM_COMMAND: {
			switch (LOWORD(wParam)) {
				case IDM_ZPET: {
					if (ab->type == CDlgAddressBook::TYPE::SEND) DisplayDlg(SENDWAY, 0, false);
					else DisplayDlg(MAIN, 0, false);
				}break; // end case IDM_ZPET

				case IDM_MOZNOSTI_POSLAT: {
					char *receiver = NULL;
					WCHAR *receiverName = NULL;
					XPhonebookItem *pi = pb->GetSelectedItem();
					if (!pi) {
						receiver = ConvertToMultiByte(pb->GetInputItemText(), CP_ACP);
						receiverName = NULL;
					}else {
						XPhonebookItem *piParent = pb->Get(pb->GetSelectedItemIndex(), true);
						if (pb->GetSelectedSubItemIndex() == NOT_SELECTED) {
							// if selected item is expandable expand it
							if (piParent && piParent->GetSubItemsCount()) {
								piParent->SetExpanded(!piParent->GetExpanded());
								pb->Redraw();
								InvalidateRect(hDlg, NULL, false);
								UpdateWindow(hDlg);
								break;
							}
						}
						// otherwise use selected item
						receiver = _strdup((char *)pi->GetData());
						receiverName = piParent->GetText();
					}
					if (receiver && *receiver) {
						BOOL send = true;
						if (g.setting.confirmReceiver) {
							WCHAR b[100];
							if (receiverName) wsprintf(b, L"Poslat zprávu na?\n\n%s (%S)", receiverName, receiver);
							else wsprintf(b, L"Poslat zprávu na?\n\n%S", receiver);
							send = MessageBox(hDlg, b, g.szApp, MB_YESNO|MB_ICONQUESTION) == IDYES;
						}
						if (send) {
							// store last receiver
							if (ab->sendTo == CDlgAddressBook::SENDTO::SMS || ab->sendTo == CDlgAddressBook::SENDTO::WEBSMS || ab->sendTo == CDlgAddressBook::SENDTO::MOBEMAIL || ab->sendTo == CDlgAddressBook::SENDTO::MAILBOX || ab->sendTo == CDlgAddressBook::SENDTO::PAIDSMS) g.setting.SetLastDestNum(receiver);
							else if (ab->sendTo == CDlgAddressBook::SENDTO::EMAIL) g.setting.SetLastDestEmail(receiver);

							// store sending data
							if (g.sendMsgReceiver) free(g.sendMsgReceiver);
							g.sendMsgReceiver = _strdup(receiver);
							g.sendMsgSendTo = ab->sendTo;
							if (g.sendMsgEmailSubject) free(g.sendMsgEmailSubject);
							g.sendMsgEmailSubject = _wcsdup(ab->emailSubject);
							free(g.sendMsgMessageId); 
							g.sendMsgMessageId = NULL;
							free(g.sendMsgPictogram); 
							g.sendMsgPictogram = NULL;

							SendSMS(false);
						}
					}else MessageBox(hDlg, ab->sendTo == CDlgAddressBook::SENDTO::SMS || ab->sendTo == CDlgAddressBook::SENDTO::WEBSMS || ab->sendTo == CDlgAddressBook::SENDTO::MOBEMAIL || ab->sendTo == CDlgAddressBook::SENDTO::MAILBOX || ab->sendTo == CDlgAddressBook::SENDTO::PAIDSMS ? L"Èíslo pøíjemce není definováno" : ab->sendTo == CDlgAddressBook::SENDTO::EMAIL ? L"Email pøíjemce není definován" : L"Neznámý pøíjemce", g.szAppErr, MB_OK|MB_ICONSTOP);
					
					free(receiver);
					
					DelayKeyboard();
				}break; // end case IDM_MOZNOSTI_POSLAT

				case IDM_MOZNOSTI_VLOZITCISLO:
				case IDM_MOZNOSTI_VLOZITEMAIL: {
					DWORD selItem = pb->GetSelectedItemDataIndex();
					if (selItem != NOT_SELECTED) {
						CPhonebookItem *pi = g.phonebook.Get(selItem);
						if (pi) {
							if ((wParam == IDM_MOZNOSTI_VLOZITCISLO && pi->GetNumber() && *pi->GetNumber()) || (wParam == IDM_MOZNOSTI_VLOZITEMAIL && *pi->GetEmail())) {
								WCHAR *insertText = new WCHAR[512];
								if (wParam == IDM_MOZNOSTI_VLOZITCISLO) wsprintf(insertText, L"%S", pi->GetNumber());
								else if (wParam == IDM_MOZNOSTI_VLOZITEMAIL) wsprintf(insertText, L"%S", pi->GetEmail());
								g.display = MAIN;
								SendMessage(g.hMain, USER_INSERTTEXT, -1, (LPARAM)insertText);
								g.display = PHONEBOOK;
								delete[] insertText;
								
								DelayKeyboard();
								DisplayDlg(MAIN, 0, false);
							}else MessageBox(hDlg, wParam == IDM_MOZNOSTI_VLOZITCISLO ? L"Èíslo pøíjemce není definováno" : wParam == IDM_MOZNOSTI_VLOZITEMAIL ? L"Email pøíjemce není definován" : L"Neznámý pøíjemce", g.szAppErr, MB_OK|MB_ICONSTOP);
						}else MessageBox(hDlg, L"Vybraná položka je chybná", g.szAppErr, MB_OK|MB_ICONSTOP);
					}else MessageBox(hDlg, L"Nejprve musíte vybrat nìjakou položku", g.szAppErr, MB_OK|MB_ICONSTOP);
				}break; // end case IDM_MOZNOSTI_VLOZITCISLO

				case IDM_MOZNOSTI_ADRESARTELEFONU: {
					usePhonebook = true;
					g.setting.useInternalPhonebook = false;
					
					SendMessage(hDlg, USER_REFRESHLIST, 0, 0);
					pb->SetSelectedItem(0, NOT_SELECTED);
					pb->CenterSelectedItem();
					pb->Redraw();
					InvalidateRect(hDlg, NULL, false);
					UpdateWindow(hDlg);
				}break; // end case IDM_MOZNOSTI_ADRESARTELEFONU

				case IDM_MOZNOSTI_ADRESARAPLIKACE: {
					usePhonebook = false;
					g.setting.useInternalPhonebook = true;

					SendMessage(hDlg, USER_REFRESHLIST, 0, 0);
					pb->SetSelectedItem(0, NOT_SELECTED);
					pb->CenterSelectedItem();
					pb->Redraw();
					InvalidateRect(hDlg, NULL, false);
					UpdateWindow(hDlg);
				}break; // end case IDM_MOZNOSTI_ADRESARAPLIKACE

				case IDM_MOZNOSTI_PRIDAT: {
					CPhonebookItem pi;
					if (DialogBoxParam(g.hInstance, MAKEINTRESOURCE(IDD_CONTACT), 0, (DLGPROC)DlgContactProc, (LPARAM)&pi)) {
						g.phonebook.Add(pi.GetName(), pi.GetNumber(), pi.GetEmail());
						g.phonebook.Sort();
						// refresh list because user added a contact
						SendMessage(hDlg, USER_REFRESHLIST, 0, 0);

						SendMessage(hDlg, USER_SELECTBYNAME, 0, (LPARAM)pi.GetName());
					}
					InitDlgPhonebook(hDlg, ab->type);

					DelayKeyboard();
				}break; // end case IDM_MOZNOSTI_PRIDAT

				case IDM_MOZNOSTI_UPRAVIT: {
					DWORD selItem = pb->GetSelectedItemDataIndex();
					if (selItem != NOT_SELECTED) {
						CPhonebookItem *pi = g.phonebook.Get(selItem);
						if (pi) {
							if (DialogBoxParam(g.hInstance, MAKEINTRESOURCE(IDD_CONTACT), 0, (DLGPROC)DlgContactProc, (LPARAM)pi)) {
								g.phonebook.Sort();
								// refresh list because user has changed the contact
								SendMessage(hDlg, USER_REFRESHLIST, 0, 0);

								SendMessage(hDlg, USER_SELECTBYNAME, 0, (LPARAM)pi->GetName());
							}
							InitDlgPhonebook(hDlg, ab->type);
							pb->Redraw();
							InvalidateRect(hDlg, NULL, false);
							UpdateWindow(hDlg);
						}else MessageBox(hDlg, L"Vybraná položka je chybná", g.szAppErr, MB_OK|MB_ICONSTOP);
					}else MessageBox(hDlg, L"Nejprve musíte vybrat nìjakou položku", g.szAppErr, MB_OK|MB_ICONSTOP);

					DelayKeyboard();
				}break; // end case IDM_MOZNOSTI_UPRAVIT


				case IDM_MOZNOSTI_SMAZAT: {
					DWORD selItem = pb->GetSelectedItemDataIndex();
					if (selItem != NOT_SELECTED) {
						if ((DWORD)selItem < g.phonebook.GetItemCount()) {
							if (g.phonebook.Remove(selItem)) {
								// refresh list because user has deleted the contact
								SendMessage(hDlg, USER_REFRESHLIST, 0, 0);

								pb->SetSelectedItem(pb->GetSelectedItemIndex(), NOT_SELECTED);
								pb->CenterSelectedItem();
								pb->Redraw();
								InvalidateRect(hDlg, NULL, false);
								UpdateWindow(hDlg);
							}
						}else MessageBox(hDlg, L"Vybraná položka je chybná", g.szAppErr, MB_OK|MB_ICONSTOP);
					}else MessageBox(hDlg, L"Nejprve musíte vybrat nìjakou položku", g.szAppErr, MB_OK|MB_ICONSTOP);

					DelayKeyboard();
				}break; // end case IDM_MOZNOSTI_SMAZAT

				case IDM_MOZNOSTI_SMAZATVSE: {
					if (MessageBox(hDlg, L"Opravdu chcete smazat celý adresáø?", g.szApp, MB_YESNO|MB_ICONQUESTION) == IDYES) {
						g.phonebook.RemoveAll();
						// refresh list because user has deleted all contacts
						SendMessage(hDlg, USER_REFRESHLIST, 0, 0);
					}

					DelayKeyboard();
				}break; // end case IDM_MOZNOSTI_SMAZAT

				case IDM_MOZNOSTI_IMPORT: {
					char *encPwd = EncodeURL(g.setting.GetPwd());
					g.AllocateUrl(1024*1);
					
					sprintf(g.url, "%s/pb_import.php?num=%s&pwd=%s&action=list&client=%s&ver=%s", g.giboServer, g.setting.GetSrcNum(), encPwd, g.client, g.szVersionA);
					
					free(encPwd);

					DisplayDlg(WAIT, true, true);
					OpenPageByThread(g.url, L"Import adresáøe...", g.hMain, USER_FINISHDOWNLOADING, CDlgStatus::TYPE::IMPORTPHONEBOOK_LIST, ContinueWithSending);

					DelayKeyboard();
				}break; // end case IDM_MOZNOSTI_IMPORT

				case IDM_MOZNOSTI_EXPORT: {
					char *encPwd = EncodeURL(g.setting.GetPwd());
					// compute size of data
					DWORD dataSize = 0;
					DWORD i;
					for (i = 0; i < g.phonebook.GetItemCount(); i++) {
						CPhonebookItem *pi = g.phonebook.Get(i);
						if (pi) {
							dataSize += strlen(pi->GetName()) + 1 + 10 + 1 + strlen(pi->GetEmail()) + 50; // + 50 for safety
						}
					}
					// compound phonebook data
					char *data = new char[dataSize + 1];
					*data = '\0';
					for (i = 0; i < g.phonebook.GetItemCount(); i++) {
						CPhonebookItem *pi = g.phonebook.Get(i);
						if (pi) {
							if (*data) strcat(data, "\r\n");
							strcat(data, pi->GetName());
							strcat(data, "=");
							strcat(data, pi->GetNumber());
							strcat(data, "|");
							strcat(data, pi->GetEmail());
						}
					}
					
					char *encData = EncodeURL(data);
					g.AllocateUrl(1024*10 + strlen(encData));
					
					sprintf(g.url, "%s/pb_export.php?num=%s&pwd=%s&client=%s&ver=%s&data=%s", g.giboServer, g.setting.GetSrcNum(), encPwd, g.client, g.szVersionA, encData);
					
					delete[] data;
					free(encPwd); free(encData);

					DisplayDlg(WAIT, true, true);
					OpenPageByThread(g.url, L"Export adresáøe...", g.hMain, USER_FINISHDOWNLOADING, CDlgStatus::TYPE::EXPORTPHONEBOOK, ContinueWithSending);

					DelayKeyboard();
				}break; // end case IDM_MOZNOSTI_EXPORT
			}
		}break; // end case WM_COMMAND

		case WM_LBUTTONUP:
		case WM_LBUTTONDBLCLK: {
			if (ab->type == CDlgAddressBook::TYPE::SEND) SendMessage(hDlg, WM_COMMAND, IDM_MOZNOSTI_POSLAT, 0);
			else if (ab->type == CDlgAddressBook::TYPE::SHOW) SendMessage(hDlg, WM_COMMAND, IDM_MOZNOSTI_VLOZITCISLO, 0);
		}break; // end case WM_LBUTTONDBLCLK
	}

	// process the message
	pb->OnMsg(hDlg, msg, wParam, lParam);

	HandleWindowScroll(hDlg, msg, wParam, lParam);

	return DefWindowProc(hDlg, msg, wParam, lParam);
}


/***********************************************************************************
* Shows wait dialog                                                                *
***********************************************************************************/
BOOL InitDlgPhonebook(HWND hWnd, CDlgAddressBook::TYPE type) {
	// set the title bar
	SetWindowText(hWnd, L"Adresáø");

	// initialize menubar
	SHMENUBARINFO mbi;
	ZeroMemory(&mbi, sizeof(SHMENUBARINFO));
	mbi.cbSize      = sizeof(SHMENUBARINFO);
	mbi.hwndParent  = hWnd;
	mbi.nToolBarId  = type == CDlgAddressBook::TYPE::SHOW ? IDR_MENU_ADDRESSBOOK_SHOW : IDR_MENU_ADDRESSBOOK_SEND;
	mbi.hInstRes    = g.hInstance;

	if (!SHCreateMenuBar(&mbi)) return false;

	// Overriding Back button functionality
	SendMessage(mbi.hwndMB, SHCMBM_OVERRIDEKEY, VK_TBACK, 
				  MAKELPARAM(SHMBOF_NODEFAULT | SHMBOF_NOTIFY, 
							 SHMBOF_NODEFAULT | SHMBOF_NOTIFY));


	ShowWindow(g.hMenuBarCurrent, SW_HIDE);
	g.hMenuBarCurrent = mbi.hwndMB;
	ShowWindow(g.hMenuBarCurrent, SW_SHOW);

	return true;
}

/***********************************************************************************
* Handles window scrolling                                                         *
***********************************************************************************/
void HandleWindowScroll(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	static CArray<HWND> hwnds;
	static CArray<int> prevPos;

	switch (msg) {
		case WM_INITDIALOG: {
			DWORD n = 0xFFFFFFFF;
			for (DWORD i = 0; i < hwnds.num; i++) {
				if (hwnds[i] == hWnd) {
					n = i;
					break;
				}
			}

			if (n == 0xFFFFFFFF) {
				hwnds.Add(hWnd);
				prevPos.Add(0);
			}
		}break;

		case WM_KEYDOWN:
			if (wParam != 98 && wParam != 104) break;
		case WM_VSCROLL: {
			DWORD n = 0xFFFFFFFF;
			for (DWORD i = 0; i < hwnds.num; i++) {
				if (hwnds[i] == hWnd) {
					n = i;
					break;
				}
			}

			if (n != 0xFFFFFFFF) {
				SCROLLINFO si = {sizeof(SCROLLINFO), 
												 SIF_PAGE|SIF_POS|SIF_RANGE|SIF_TRACKPOS, 0, 0, 0, 0, 0};
				GetScrollInfo(hWnd, SB_VERT, &si);

				int nOldPos = si.nPos;
				int nNewPos = si.nPos;

				if (msg == WM_VSCROLL) {
					int nScrollCode = (int)LOWORD(wParam);
					int nPos = (short int)HIWORD(wParam);

					if (nScrollCode == SB_THUMBPOSITION) {
						nNewPos = nPos + si.nMin; // Adding si.nMin is the workaround.
					}else if (nScrollCode == SB_LINEDOWN) {
						nNewPos = nPos + 15;
					}else if (nScrollCode == SB_LINEUP) {
						nNewPos = nPos - 15;
					}else if (nScrollCode == SB_PAGEDOWN) {
						nNewPos = nPos + si.nPage;
					}else if (nScrollCode == SB_PAGEUP) {
						nNewPos = nPos - si.nPage;
					}
				}else if (msg == WM_KEYDOWN) {
					if (wParam == 98) nNewPos = si.nPos + 15;
					else if (wParam == 104) nNewPos = si.nPos - 15;
				}

				nNewPos = max(nNewPos, 0);
				nNewPos = min(nNewPos, si.nMax);

				if (nOldPos != nNewPos) {
					si.fMask = SIF_POS;
					si.nPos = nNewPos;
					SetScrollInfo(hWnd, SB_VERT, &si, TRUE);

					if (prevPos[n] != si.nPos) {
						if (prevPos[n]) ScrollWindowEx(hWnd, 0, prevPos[n], NULL, NULL, NULL, NULL, SW_SCROLLCHILDREN);
						prevPos[n] = si.nPos;
						if (si.nPos) ScrollWindowEx(hWnd, 0, -si.nPos, NULL, NULL, NULL, NULL, SW_SCROLLCHILDREN);
					}
				}
			}
		}break; // end case WM_SCROLL
	} // end switch (msg)
}


/***********************************************************************************
* Create item for XPhonebook                                                       *
***********************************************************************************/
BOOL CreateItem(CHdc *hdc, HBITMAP bmpIcon, HBITMAP bmpTile) {
	if (!bmpIcon) return false;
	if (!bmpTile) return false;

	CHdc hdcIcon(false);
	CHdc hdcTile(false);

	hdcIcon.LoadBitmap(bmpIcon);
	hdcTile.LoadBitmap(bmpTile);
	
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	hdc->SetSize(screenWidth, hdcIcon.GetHeight());

	BitBlt(*hdc, 0, 0, hdcIcon.GetWidth(), hdcIcon.GetHeight(), hdcIcon, 0, 0, SRCCOPY);

	int tileWidth = hdcTile.GetWidth(), tileHeight = hdcTile.GetHeight();
	for (int x = hdcIcon.GetWidth(); x < screenWidth; x += tileWidth) {
		BitBlt(*hdc, x, 0, tileWidth, tileHeight, hdcTile, 0, 0, SRCCOPY);
	}
	return true;
}

/***********************************************************************************
* Dlg message handler                                                              *
***********************************************************************************/
LRESULT DlgSendWayProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam) {
	static XPhonebook *pb = new XPhonebook();
	static BOOL initialized = false;
	static CHdc hdcItemMessage(false);
	static CHdc hdcSelectedItemMessage(false);
	static CHdc hdcItemEmail(false);
	static CHdc hdcSelectedItemEmail(false);
	static CHdc hdcItemMailbox(false);
	static CHdc hdcSelectedItemMailbox(false);
	static CHdc hdcBackground(false);
	static CFont fntItem;
	static CFont fntSelectedItem;
	static CHdc hdcItemPaidSMS(false);
	static CHdc hdcSelectedItemPaidSMS(false);

	switch (msg) {
		case WM_INITDIALOG: {
			if (!InitDlgSendWay(hDlg)) {
				DisplayDlg(MAIN, 0, false);
			}else {
				if (lParam) {
					if (!initialized) {
						CreateItem(&hdcItemMessage, LoadBitmap(g.hInstance, (LPCWSTR)IDB_ITEMMESSAGE), LoadBitmap(g.hInstance, (LPCWSTR)IDB_ITEMTILE));
						CreateItem(&hdcSelectedItemMessage, LoadBitmap(g.hInstance, (LPCWSTR)IDB_SELECTEDITEMMESSAGE), LoadBitmap(g.hInstance, (LPCWSTR)IDB_SELECTEDITEMTILE));
						CreateItem(&hdcItemEmail, LoadBitmap(g.hInstance, (LPCWSTR)IDB_ITEMEMAIL), LoadBitmap(g.hInstance, (LPCWSTR)IDB_ITEMTILE));
						CreateItem(&hdcSelectedItemEmail, LoadBitmap(g.hInstance, (LPCWSTR)IDB_SELECTEDITEMEMAIL), LoadBitmap(g.hInstance, (LPCWSTR)IDB_SELECTEDITEMTILE));
						CreateItem(&hdcItemMailbox, LoadBitmap(g.hInstance, (LPCWSTR)IDB_ITEMMAILBOX), LoadBitmap(g.hInstance, (LPCWSTR)IDB_ITEMTILE));
						CreateItem(&hdcSelectedItemMailbox, LoadBitmap(g.hInstance, (LPCWSTR)IDB_SELECTEDITEMMAILBOX), LoadBitmap(g.hInstance, (LPCWSTR)IDB_SELECTEDITEMTILE));
						CreateItem(&hdcItemPaidSMS, LoadBitmap(g.hInstance, (LPCWSTR)IDB_ITEMPAIDSMS), LoadBitmap(g.hInstance, (LPCWSTR)IDB_ITEMTILE));
						CreateItem(&hdcSelectedItemPaidSMS, LoadBitmap(g.hInstance, (LPCWSTR)IDB_SELECTEDITEMPAIDSMS), LoadBitmap(g.hInstance, (LPCWSTR)IDB_SELECTEDITEMTILE));

						fntItem.CreateFont(hDlg, 10, L"MS Sans Serif", false, false, false, 1, 0);
						fntSelectedItem.CreateFont(hDlg, 10, L"MS Sans Serif", false, false, false, 1, 0);

						pb->SetGraphics(NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, &fntItem, &fntSelectedItem, NULL);

						RECT rc;
						GetClientRect(hDlg, &rc);

						// nastavit pozadí s logem
						hdcBackground.SetSize(rc.right - rc.left, rc.bottom - rc.top);
						pb->SetBackground(&hdcBackground, 0);
						DrawBackgroundLogo(&hdcBackground);

						pb->SetDialogSize(rc.right - rc.left, rc.bottom - rc.top);

						pb->SetAllowInputItem(false);

						initialized = true;
					}

					pb->RemoveAll();

					XPhonebookItem *item = new XPhonebookItem;
					
					if (!g.setting.expertMode) {
						item->SetText(L"SMS zdarma pøes GPRS");
						item->SetGraphics(&hdcItemMessage, &hdcSelectedItemMessage);
						pb->Add(item);

						item = new XPhonebookItem;
						item->SetText(L"SMS za 1 Kè");
						item->SetGraphics(&hdcItemPaidSMS, &hdcSelectedItemPaidSMS);
						pb->Add(item);
					}else {
						item->SetText(L"Web SMS zdarma");
						item->SetGraphics(&hdcItemMessage, &hdcSelectedItemMessage);
						pb->Add(item);

						item = new XPhonebookItem;
						item->SetText(L"SMS za 1 Kè");
						item->SetGraphics(&hdcItemPaidSMS, &hdcSelectedItemPaidSMS);
						pb->Add(item);

						item = new XPhonebookItem;
						item->SetText(L"Mobilní email");
						item->SetGraphics(&hdcItemEmail, &hdcSelectedItemEmail);
						pb->Add(item);
					}

					item = new XPhonebookItem;
					item->SetText(L"Email");
					item->SetGraphics(&hdcItemEmail, &hdcSelectedItemEmail);
					pb->Add(item);

					item = new XPhonebookItem;
					item->SetText(L"Gibomeska");
					item->SetGraphics(&hdcItemMailbox, &hdcSelectedItemMailbox);
					pb->Add(item);

					if (!g.setting.expertMode) { 
						if (g.setting.lastSendWay == CDlgAddressBook::SENDTO::SMS) pb->SetSelectedItem(0, NOT_SELECTED);
						else if (g.setting.lastSendWay == CDlgAddressBook::SENDTO::PAIDSMS) pb->SetSelectedItem(1, NOT_SELECTED);
						else if (g.setting.lastSendWay == CDlgAddressBook::SENDTO::EMAIL) pb->SetSelectedItem(2, NOT_SELECTED);
						else if (g.setting.lastSendWay == CDlgAddressBook::SENDTO::MAILBOX) pb->SetSelectedItem(3, NOT_SELECTED);
						else pb->SetSelectedItem(0, NOT_SELECTED);
					}else {
						if (g.setting.lastSendWay == CDlgAddressBook::SENDTO::WEBSMS) pb->SetSelectedItem(0, NOT_SELECTED);
						else if (g.setting.lastSendWay == CDlgAddressBook::SENDTO::PAIDSMS) pb->SetSelectedItem(1, NOT_SELECTED);
						else if (g.setting.lastSendWay == CDlgAddressBook::SENDTO::MOBEMAIL) pb->SetSelectedItem(2, NOT_SELECTED);
						else if (g.setting.lastSendWay == CDlgAddressBook::SENDTO::EMAIL) pb->SetSelectedItem(3, NOT_SELECTED);
						else if (g.setting.lastSendWay == CDlgAddressBook::SENDTO::MAILBOX) pb->SetSelectedItem(4, NOT_SELECTED);
						else pb->SetSelectedItem(0, NOT_SELECTED);
					}

					DelayKeyboard();
				}

				pb->Redraw();
				InvalidateRect(hDlg, NULL, false);
				UpdateWindow(hDlg);
			}
		}break; // end case WM_INITDIALOG

		case WM_HOTKEY: { // Override the back key to delete
			if (HIWORD(lParam) == VK_TBACK && !(lParam & MOD_KEYUP)) {
				SendMessage(hDlg, WM_COMMAND, IDM_ZPET, 0);
			}
		}break;

		case WM_KEYDOWN: {
			if (g.skipUntilTime < GetTickCount()) {
				if (wParam == VK_RETURN) {
					SendMessage(hDlg, WM_COMMAND, IDM_VYBRAT, 0);
				}
			}
		}break; // end case USER_USERKEYDOWN

		case WM_COMMAND: {
			switch (LOWORD(wParam)) {
				case IDM_ZPET: {
					DisplayDlg(MAIN, 0, false);
				}break; // end case IDM_MOZNOSTI_KONEC

				case IDM_VYBRAT: {
					int selItem = pb->GetSelectedItemIndex();
					if (selItem != NOT_SELECTED) {
						static CDlgAddressBook ab;
						ab.type = CDlgAddressBook::TYPE::SEND;
						if (!g.setting.expertMode) {
							if (selItem == 0) ab.sendTo = CDlgAddressBook::SENDTO::SMS;
							else if (selItem == 1) ab.sendTo = CDlgAddressBook::SENDTO::PAIDSMS;
							else if (selItem == 2) ab.sendTo = CDlgAddressBook::SENDTO::EMAIL;
							else if (selItem == 3) ab.sendTo = CDlgAddressBook::SENDTO::MAILBOX;
						}else {
							if (selItem == 0) ab.sendTo = CDlgAddressBook::SENDTO::WEBSMS;
							else if (selItem == 1) ab.sendTo = CDlgAddressBook::SENDTO::PAIDSMS;
							else if (selItem == 2) ab.sendTo = CDlgAddressBook::SENDTO::MOBEMAIL;
							else if (selItem == 3) ab.sendTo = CDlgAddressBook::SENDTO::EMAIL;
							else if (selItem == 4) ab.sendTo = CDlgAddressBook::SENDTO::MAILBOX;
						}

						g.setting.lastSendWay = ab.sendTo;

						if (ab.emailSubject) free(ab.emailSubject);
						if (ab.sendTo == CDlgAddressBook::SENDTO::EMAIL) {
							ab.emailSubject = (WCHAR *)DialogBoxParam(g.hInstance, (LPCWSTR)IDD_EMAILSUBJECT, hDlg, (DLGPROC)DlgEmailSubjectProc, 0);
							if (!ab.emailSubject) {
								DisplayDlg(SENDWAY, 0, false);
								break;
							}
						}else ab.emailSubject = NULL;
						// show phonebook
						DisplayDlg(PHONEBOOK, (LPARAM)&ab, false);
					}
					DelayKeyboard();
				}break; // end case IDM_VYBRAT
			}
		}break; // end case WM_COMMAND

		case WM_LBUTTONUP:
		case WM_LBUTTONDBLCLK: {
			SendMessage(hDlg, WM_COMMAND, IDM_VYBRAT, 0);
		}break; // end case WM_LBUTTONDBLCLK
	}

	pb->OnMsg(hDlg, msg, wParam, lParam);

	HandleWindowScroll(hDlg, msg, wParam, lParam);

	return DefWindowProc(hDlg, msg, wParam, lParam);
}


/***********************************************************************************
* Shows wait dialog                                                                *
***********************************************************************************/
BOOL InitDlgSendWay(HWND hWnd) {
	// initialize dialog box, specifying that it should stretch full screen
	SHINITDLGINFO sid;
	ZeroMemory(&sid, sizeof(sid));
	sid.dwMask  = SHIDIM_FLAGS;
	sid.dwFlags = SHIDIF_SIZEDLGFULLSCREEN;
	sid.hDlg    = hWnd;

	// initialize the dialog
	if (!SHInitDialog(&sid)) return false;

	// set the title bar
	SetWindowText(hWnd, L"Vyberte zpùsob odeslání zprávy");

	// initialize menubar
	SHMENUBARINFO mbi;
	ZeroMemory(&mbi, sizeof(SHMENUBARINFO));
	mbi.cbSize      = sizeof(SHMENUBARINFO);
	mbi.hwndParent  = hWnd;
	mbi.nToolBarId  = IDR_MENU_SENDWAY;
	mbi.hInstRes    = g.hInstance;

	if (!SHCreateMenuBar(&mbi)) return false;

	// Overriding Back button functionality
	SendMessage(mbi.hwndMB, SHCMBM_OVERRIDEKEY, VK_TBACK, 
				  MAKELPARAM(SHMBOF_NODEFAULT | SHMBOF_NOTIFY, 
							 SHMBOF_NODEFAULT | SHMBOF_NOTIFY));


	ShowWindow(g.hMenuBarCurrent, SW_HIDE);
	g.hMenuBarCurrent = mbi.hwndMB;
	ShowWindow(g.hMenuBarCurrent, SW_SHOW);

	return true;
}


/***********************************************************************************
* Dlg message handler                                                              *
***********************************************************************************/
LRESULT DlgLstMsgsReadProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam) {
	static XPhonebook *pb = new XPhonebook();
	static BOOL initialized = false;
	static CHdc hdcItemMailbox(false);
	static CHdc hdcSelectedItemMailbox(false);
	static CHdc hdcItemSentMsgs(false);
	static CHdc hdcSelectedItemSentMsgs(false);
	static CHdc hdcItemReceivedMsgs(false);
	static CHdc hdcSelectedItemReceivedMsgs(false);
	static CHdc hdcBackground(false);
	static CFont fntItem;
	static CFont fntSelectedItem;

	switch (msg) {
		case WM_INITDIALOG: {
			if (!InitDlgLstMsgsRead(hDlg)) {
				DisplayDlg(MAIN, 0, false);
			}else {
				if (lParam) {
					if (!initialized) {
						CreateItem(&hdcItemMailbox, LoadBitmap(g.hInstance, (LPCWSTR)IDB_ITEMMAILBOX), LoadBitmap(g.hInstance, (LPCWSTR)IDB_ITEMTILE));
						CreateItem(&hdcSelectedItemMailbox, LoadBitmap(g.hInstance, (LPCWSTR)IDB_SELECTEDITEMMAILBOX), LoadBitmap(g.hInstance, (LPCWSTR)IDB_SELECTEDITEMTILE));
						CreateItem(&hdcItemSentMsgs, LoadBitmap(g.hInstance, (LPCWSTR)IDB_ITEMSENTMSGS), LoadBitmap(g.hInstance, (LPCWSTR)IDB_ITEMTILE));
						CreateItem(&hdcSelectedItemSentMsgs, LoadBitmap(g.hInstance, (LPCWSTR)IDB_SELECTEDITEMSENTMSGS), LoadBitmap(g.hInstance, (LPCWSTR)IDB_SELECTEDITEMTILE));
						CreateItem(&hdcItemReceivedMsgs, LoadBitmap(g.hInstance, (LPCWSTR)IDB_ITEMRECEIVEDMSGS), LoadBitmap(g.hInstance, (LPCWSTR)IDB_ITEMTILE));
						CreateItem(&hdcSelectedItemReceivedMsgs, LoadBitmap(g.hInstance, (LPCWSTR)IDB_SELECTEDITEMRECEIVEDMSGS), LoadBitmap(g.hInstance, (LPCWSTR)IDB_SELECTEDITEMTILE));

						fntItem.CreateFont(hDlg, 10, L"MS Sans Serif", false, false, false);
						fntSelectedItem.CreateFont(hDlg, 10, L"MS Sans Serif", false, false, false);

						pb->SetGraphics(NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, &fntItem, &fntSelectedItem, NULL);

						RECT rc;
						GetClientRect(hDlg, &rc);

						pb->SetDialogSize(rc.right - rc.left, rc.bottom - rc.top);

						pb->SetAllowInputItem(false);

						hdcBackground.SetSize(rc.right - rc.left, rc.bottom - rc.top);
						pb->SetBackground(&hdcBackground, 0);

						DrawBackgroundLogo(&hdcBackground);

						XPhonebookItem *item = new XPhonebookItem;
						
						item->SetText(L"Moje gibomesky");
						item->SetGraphics(&hdcItemMailbox, &hdcSelectedItemMailbox);
						pb->Add(item);

						item = new XPhonebookItem;
						item->SetText(L"Odeslané zprávy");
						item->SetGraphics(&hdcItemSentMsgs, &hdcSelectedItemSentMsgs);
						pb->Add(item);

						item = new XPhonebookItem;
						item->SetText(L"Pøijaté zprávy");
						item->SetGraphics(&hdcItemReceivedMsgs, &hdcSelectedItemReceivedMsgs);
						pb->Add(item);

						initialized = true;

					}

					pb->SetSelectedItem(0, NOT_SELECTED);

					DelayKeyboard();
				}

				pb->Redraw();
				InvalidateRect(hDlg, NULL, false);
				UpdateWindow(hDlg);
			}
		}break; // end case WM_INITDIALOG

		case WM_HOTKEY: { // Override the back key to delete
			if (HIWORD(lParam) == VK_TBACK && !(lParam & MOD_KEYUP)) {
				SendMessage(hDlg, WM_COMMAND, IDM_ZPET, 0);
			}
		}break;

		case WM_KEYDOWN: {
			if (g.skipUntilTime < GetTickCount()) {
				if (wParam == VK_RETURN) {
					SendMessage(hDlg, WM_COMMAND, IDM_VYBRAT, 0);
				}
			}
		}break; // end case USER_USERKEYDOWN

		case WM_COMMAND: {
			switch (LOWORD(wParam)) {
				case IDM_ZPET: {
					DisplayDlg(MAIN, 0, false);
				}break; // end case IDM_MOZNOSTI_KONEC

				case IDM_VYBRAT: {
					int selItem = pb->GetSelectedItemIndex();
					if (selItem != NOT_SELECTED) {
						SetCharPtr(&g.msgsReadType, selItem == 0 ? "mailbox" : selItem == 1 ? "sent" : "received");

						CDlgPassword ps;
						ps.SetPassword(g.setting.msgsReadRememberPassword ? g.setting.GetMsgsReadPassword() : "");
						ps.SetRememberPassword(g.setting.msgsReadRememberPassword);

						if (g.msgsReadCorrectPasswordSubmitted || DialogBoxParam(g.hInstance, (LPCWSTR)IDD_PASSWORD, hDlg, (DLGPROC)DlgPasswordProc, (LPARAM)&ps)) {
							// pokud byl zobrazen dialog se zadanim hesla tak ulozit udaje
							if (!g.msgsReadCorrectPasswordSubmitted) {
								SetCharPtr(&g.msgsReadPassword, ps.GetPassword());
								g.setting.SetMsgsReadPassword(ps.GetRememberPassword() ? ps.GetPassword() : "");
								g.setting.msgsReadRememberPassword = ps.GetRememberPassword();
							}
							
							// vynuloval cteci pozici
							g.msgsReadPos = 0;
							g.msgsReadPage = g.setting.msgsReadPage;
							SetCharPtr(&g.msgsReadCont, "");
							SetCharPtr(&g.msgsReadSearch_text, "");
							g.msgsReadStartTime = g.msgsReadEndTime = 0;

							ShowMsgsRead();
						}else {
							g.setting.SetMsgsReadPassword(ps.GetRememberPassword() ? ps.GetPassword() : "");
							g.setting.msgsReadRememberPassword = ps.GetRememberPassword();
							
							InitDlgLstMsgsRead(hDlg);
						}

					}
					DelayKeyboard();
				}break; // end case IDM_VYBRAT
			}
		}break; // end case WM_COMMAND

		case WM_LBUTTONUP:
		case WM_LBUTTONDBLCLK: {
			SendMessage(hDlg, WM_COMMAND, IDM_VYBRAT, 0);
		}break; // end case WM_LBUTTONDBLCLK
	}

	pb->OnMsg(hDlg, msg, wParam, lParam);

	HandleWindowScroll(hDlg, msg, wParam, lParam);

	return DefWindowProc(hDlg, msg, wParam, lParam);
}


/***********************************************************************************
* Shows wait dialog                                                                *
***********************************************************************************/
BOOL InitDlgLstMsgsRead(HWND hWnd) {
	// initialize dialog box, specifying that it should stretch full screen
	SHINITDLGINFO sid;
	ZeroMemory(&sid, sizeof(sid));
	sid.dwMask  = SHIDIM_FLAGS;
	sid.dwFlags = SHIDIF_SIZEDLGFULLSCREEN;
	sid.hDlg    = hWnd;

	// initialize the dialog
	if (!SHInitDialog(&sid)) return false;

	// set the title bar
	SetWindowText(hWnd, L"Vyberte jaké zprávy chcete zobrazit");

	// initialize menubar
	SHMENUBARINFO mbi;
	ZeroMemory(&mbi, sizeof(SHMENUBARINFO));
	mbi.cbSize      = sizeof(SHMENUBARINFO);
	mbi.hwndParent  = hWnd;
	mbi.nToolBarId  = IDR_MENU_LSTMSGSREAD;
	mbi.hInstRes    = g.hInstance;

	if (!SHCreateMenuBar(&mbi)) return false;

	// Overriding Back button functionality
	SendMessage(mbi.hwndMB, SHCMBM_OVERRIDEKEY, VK_TBACK, 
				  MAKELPARAM(SHMBOF_NODEFAULT | SHMBOF_NOTIFY, 
							 SHMBOF_NODEFAULT | SHMBOF_NOTIFY));


	ShowWindow(g.hMenuBarCurrent, SW_HIDE);
	g.hMenuBarCurrent = mbi.hwndMB;
	ShowWindow(g.hMenuBarCurrent, SW_SHOW);

	return true;
}


/***********************************************************************************
* Dlg message handler                                                              *
***********************************************************************************/
BOOL CALLBACK DlgPasswordProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam) {
	static CDlgPassword *ps = NULL;

	switch (msg) {
		case WM_INITDIALOG: {
			if (!InitDlgPassword(hDlg)) {
				EndDialog(hDlg, 0);
			}else {
				ps = (CDlgPassword *)lParam;
				if (ps) {
					SetFocus(GetDlgItem(hDlg, IDC_EDIT_PASSWORD));

					WCHAR *tW = ConvertToWideChar(ps->GetPassword(), CP_ACP);
					SetDlgItemText(hDlg, IDC_EDIT_PASSWORD, tW);
					SendDlgItemMessage(hDlg, IDC_EDIT_PASSWORD, EM_SETSEL, GetWindowTextLength(GetDlgItem(hDlg, IDC_EDIT_PASSWORD)), GetWindowTextLength(GetDlgItem(hDlg, IDC_EDIT_PASSWORD)));
					SendDlgItemMessage(hDlg, IDC_EDIT_PASSWORD, EM_SCROLLCARET, 0, 0);
					if (wcslen(tW)) SendDlgItemMessage(hDlg, IDC_EDIT_PASSWORD, EM_SETPASSWORDCHAR, '*', 0);
					delete[] tW;

					if (ps->GetRememberPassword()) SendDlgItemMessage(hDlg, IDC_CHECK_REMEMBER, BM_SETCHECK, BST_CHECKED, 0);
				}else {
					EndDialog(hDlg, 0);
					return 1;
				}
			}
		}break; // end case WM_INITDIALOG

		case WM_HOTKEY: { // Override the back key to delete
			if (VK_TBACK == HIWORD(lParam)) {
				#ifndef PPC
					SHSendBackToFocusWindow(msg, wParam, lParam);
				#endif
			}
		}break;

		case WM_CLOSE: {
			EndDialog(hDlg, 0);
		}break; // case WM_CLOSE

		case WM_COMMAND: {
			switch (LOWORD(wParam)) {
				case IDOK: {
					WCHAR b[100]; char *bb;

					GetDlgItemText(hDlg, IDC_EDIT_PASSWORD, b, 100);
					ps->SetPassword(bb = ConvertToMultiByte(b, CP_ACP));
					free(bb);

					ps->SetRememberPassword(SendDlgItemMessage(hDlg, IDC_CHECK_REMEMBER, BM_GETCHECK, 0, 0));

					EndDialog(hDlg, true);
				}break; // end case IDOK

				case IDCANCEL: {
					ps->SetRememberPassword(SendDlgItemMessage(hDlg, IDC_CHECK_REMEMBER, BM_GETCHECK, 0, 0));
					ps->SetPassword(NULL);
					EndDialog(hDlg, false);
				}break; // end case IDCANCEL:
			}
		}break; // end case WM_COMMAND
	}

	HandleWindowScroll(hDlg, msg, wParam, lParam);

	return false;
}


/***********************************************************************************
* Initialize the dialog                                                            *
***********************************************************************************/
BOOL InitDlgPassword(HWND hWnd) {
	// initialize dialog box, specifying that it should stretch full screen
	SHINITDLGINFO sid;
	ZeroMemory(&sid, sizeof(sid));
	sid.dwMask  = SHIDIM_FLAGS;
	sid.dwFlags = SHIDIF_SIZEDLGFULLSCREEN;
	sid.hDlg    = hWnd;

	// initialize the dialog
	if (!SHInitDialog(&sid)) return false;

	// set the title bar
	SetWindowText(hWnd, L"Zadejte heslo");


	// initialize menubar
	SHMENUBARINFO mbi;
	ZeroMemory(&mbi, sizeof(SHMENUBARINFO));
	mbi.cbSize      = sizeof(SHMENUBARINFO);
	mbi.hwndParent  = hWnd;
	mbi.nToolBarId  = IDR_MENU_PASSWORD;
	mbi.hInstRes    = g.hInstance;

	if (!SHCreateMenuBar(&mbi)) return false;

	// Overriding Back button functionality
	SendMessage(mbi.hwndMB, SHCMBM_OVERRIDEKEY, VK_TBACK, 
				  MAKELPARAM(SHMBOF_NODEFAULT | SHMBOF_NOTIFY, 
							 SHMBOF_NODEFAULT | SHMBOF_NOTIFY));

	ShowWindow(g.hMenuBarCurrent, SW_HIDE);
	g.hMenuBarCurrent = mbi.hwndMB;
	ShowWindow(g.hMenuBarCurrent, SW_SHOW);


	return true;
}


/***********************************************************************************
* Dlg message handler                                                              *
***********************************************************************************/
BOOL CALLBACK DlgJumpOverProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam) {
	static CDlgJumpOver *jo = NULL;

	switch (msg) {
		case WM_INITDIALOG: {
			if (!InitDlgJumpOver(hDlg)) {
				EndDialog(hDlg, 0);
			}else {
				jo = (CDlgJumpOver *)lParam;
				if (jo) {
					SetFocus(GetDlgItem(hDlg, IDC_EDIT_STEP));

					SetDlgItemInt(hDlg, IDC_EDIT_STEP, jo->step > 0 ? jo->step : -jo->step, false);

					SetDlgItemInt(hDlg, IDC_EDIT_PAGESIZE, jo->pageSize, false);
				}else {
					EndDialog(hDlg, 0);
					return 1;
				}
			}
		}break; // end case WM_INITDIALOG

		case WM_HOTKEY: { // Override the back key to delete
			if (VK_TBACK == HIWORD(lParam)) {
				#ifndef PPC
					SHSendBackToFocusWindow(msg, wParam, lParam);
				#endif
			}
		}break;

		case WM_CLOSE: {
			EndDialog(hDlg, 0);
		}break; // case WM_CLOSE

		case WM_COMMAND: {
			switch (LOWORD(wParam)) {
				case IDM_MOZNOSTI_DOPREDU:
				case IDM_MOZNOSTI_DOZADU: {
					jo->step = GetDlgItemInt(hDlg, IDC_EDIT_STEP, NULL, false);
					if (LOWORD(wParam) == IDM_MOZNOSTI_DOZADU) jo->step = -jo->step;
					jo->pageSize = GetDlgItemInt(hDlg, IDC_EDIT_PAGESIZE, NULL, false);

					EndDialog(hDlg, true);
				}break; // end case IDM_MOZNOSTI_DOPREDU || IDM_MOZNOSTI_DOZADU

				case IDM_ZPET: {
					EndDialog(hDlg, false);
				}break; // end case IDCANCEL:
			}
		}break; // end case WM_COMMAND
	}

	HandleWindowScroll(hDlg, msg, wParam, lParam);

	return false;
}


/***********************************************************************************
* Initialize the dialog                                                            *
***********************************************************************************/
BOOL InitDlgJumpOver(HWND hWnd) {
	// initialize dialog box, specifying that it should stretch full screen
	SHINITDLGINFO sid;
	ZeroMemory(&sid, sizeof(sid));
	sid.dwMask  = SHIDIM_FLAGS;
	sid.dwFlags = SHIDIF_SIZEDLGFULLSCREEN;
	sid.hDlg    = hWnd;

	// initialize the dialog
	if (!SHInitDialog(&sid)) return false;

	// set the title bar
	SetWindowText(hWnd, L"Pøesunout se o...");


	// initialize menubar
	SHMENUBARINFO mbi;
	ZeroMemory(&mbi, sizeof(SHMENUBARINFO));
	mbi.cbSize      = sizeof(SHMENUBARINFO);
	mbi.hwndParent  = hWnd;
	mbi.nToolBarId  = IDR_MENU_JUMPOVER;
	mbi.hInstRes    = g.hInstance;

	if (!SHCreateMenuBar(&mbi)) return false;

	// Overriding Back button functionality
	SendMessage(mbi.hwndMB, SHCMBM_OVERRIDEKEY, VK_TBACK, 
				  MAKELPARAM(SHMBOF_NODEFAULT | SHMBOF_NOTIFY, 
							 SHMBOF_NODEFAULT | SHMBOF_NOTIFY));

	ShowWindow(g.hMenuBarCurrent, SW_HIDE);
	g.hMenuBarCurrent = mbi.hwndMB;
	ShowWindow(g.hMenuBarCurrent, SW_SHOW);


	return true;
}


/***********************************************************************************
* Dlg message handler                                                              *
***********************************************************************************/
BOOL CALLBACK DlgSearchProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam) {
	static CDlgSearch *srch = NULL;

	switch (msg) {
		case WM_INITDIALOG: {
			if (!InitDlgSearch(hDlg)) {
				EndDialog(hDlg, 0);
			}else {
				srch = (CDlgSearch *)lParam;
				if (srch) {
					SetFocus(GetDlgItem(hDlg, IDC_EDIT_CONTACT));

					WCHAR *tW = ConvertToWideChar(srch->GetCont(), CP_ACP);
					SetDlgItemText(hDlg, IDC_EDIT_CONTACT, tW);
					delete[] tW;

					tW = ConvertToWideChar(srch->GetSearch_text(), CP_ACP);
					SetDlgItemText(hDlg, IDC_EDIT_TEXT, tW);
					delete[] tW;

					SetScroller(hDlg);
				}else {
					EndDialog(hDlg, 0);
					return 1;
				}
			}
		}break; // end case WM_INITDIALOG

		case WM_HOTKEY: { // Override the back key to delete
			if (VK_TBACK == HIWORD(lParam)) {
				#ifndef PPC
					SHSendBackToFocusWindow(msg, wParam, lParam);
				#endif
			}
		}break;

		case WM_CLOSE: {
			EndDialog(hDlg, 0);
		}break; // case WM_CLOSE

		case WM_COMMAND: {
			switch (LOWORD(wParam)) {
				case IDOK: {
					WCHAR b[100]; char *bb;

					GetDlgItemText(hDlg, IDC_EDIT_CONTACT, b, 100);
					srch->SetCont(bb = ConvertToMultiByte(b, CP_ACP));
					free(bb);

					GetDlgItemText(hDlg, IDC_EDIT_TEXT, b, 100);
					srch->SetSearch_text(bb = ConvertToMultiByte(b, CP_ACP));
					free(bb);

					SYSTEMTIME sysTime;

					DateTime_GetSystemtime(GetDlgItem(hDlg, IDC_DATETIME_OD), &sysTime);
					sysTime.wHour = sysTime.wMinute = sysTime.wSecond = 0;
					srch->startTime = SystemTimeToUnixEpoch(&sysTime);

					DateTime_GetSystemtime(GetDlgItem(hDlg, IDC_DATETIME_DO), &sysTime);
					sysTime.wHour = sysTime.wMinute = sysTime.wSecond = 0;
					srch->endTime = SystemTimeToUnixEpoch(&sysTime);

					EndDialog(hDlg, true);
				}break; // end case IDOK

				case IDCANCEL: {
					EndDialog(hDlg, false);
				}break; // end case IDCANCEL:
			}
		}break; // end case WM_COMMAND
	}

	HandleWindowScroll(hDlg, msg, wParam, lParam);

	return false;
}


/***********************************************************************************
* Initialize the dialog                                                            *
***********************************************************************************/
BOOL InitDlgSearch(HWND hWnd) {
	// initialize dialog box, specifying that it should stretch full screen
	SHINITDLGINFO sid;
	ZeroMemory(&sid, sizeof(sid));
	sid.dwMask  = SHIDIM_FLAGS;
	sid.dwFlags = SHIDIF_SIZEDLGFULLSCREEN;
	sid.hDlg    = hWnd;

	// initialize the dialog
	if (!SHInitDialog(&sid)) return false;

	// set the title bar
	SetWindowText(hWnd, L"Vyhledat zprávy...");


	// initialize menubar
	SHMENUBARINFO mbi;
	ZeroMemory(&mbi, sizeof(SHMENUBARINFO));
	mbi.cbSize      = sizeof(SHMENUBARINFO);
	mbi.hwndParent  = hWnd;
	mbi.nToolBarId  = IDR_MENU_SEARCH;
	mbi.hInstRes    = g.hInstance;

	if (!SHCreateMenuBar(&mbi)) return false;

	// Overriding Back button functionality
	SendMessage(mbi.hwndMB, SHCMBM_OVERRIDEKEY, VK_TBACK, 
				  MAKELPARAM(SHMBOF_NODEFAULT | SHMBOF_NOTIFY, 
							 SHMBOF_NODEFAULT | SHMBOF_NOTIFY));

	ShowWindow(g.hMenuBarCurrent, SW_HIDE);
	g.hMenuBarCurrent = mbi.hwndMB;
	ShowWindow(g.hMenuBarCurrent, SW_SHOW);


	return true;
}


/***********************************************************************************
* Shows dialog with messages reading                                               *
***********************************************************************************/
void ShowMsgsRead() {
	g.AllocateUrl(1024*10);
	
	sprintf(g.url, "%s/messages.php?num=%s&pwd=%s&type=%s&cont=%s&search_text=%s&pos=%i&page=%i&startTime=%u&endTime=%u&varsformat=csv&client=%s&ver=%s", g.giboServer, g.setting.GetSrcNum(), g.msgsReadPassword, g.msgsReadType, g.msgsReadCont, g.msgsReadSearch_text, g.msgsReadPos, g.msgsReadPage, g.msgsReadStartTime, g.msgsReadEndTime, g.client, g.szVersionA);

	DisplayDlg(WAIT, true, true);
	g.displayPrev = LSTMSGSREAD;
	OpenPageByThread(g.url, !strcmp(g.msgsReadType, "mailbox") ? L"Moje gibomesky" : !strcmp(g.msgsReadType, "sent") ? L"Odeslané zprávy" : L"Pøijaté zprávy", g.hMain, USER_FINISHDOWNLOADING, CDlgStatus::TYPE::MSGSREAD, ContinueWithSending);
}


/***********************************************************************************
* Sets the right scroller size                                                     *
***********************************************************************************/
void SetScroller(HWND hDlg) {
	RECT rc;
	GetWindowRect(hDlg, &rc);
	int dialogHeight = 0;

	HWND h = NULL;
	while (h = GetWindow(!h ? hDlg : h, !h ? GW_CHILD : GW_HWNDNEXT)) {
		RECT rcChild;
		GetWindowRect(h, &rcChild);
		POINT pt = {rcChild.left, rcChild.bottom};
		ScreenToClient(hDlg, &pt);
		if (pt.y > dialogHeight) dialogHeight = pt.y;
	}

	// set the scrollbar
	SCROLLINFO si;
	si.cbSize = sizeof(si);
	si.fMask = SIF_RANGE|SIF_PAGE;
	GetScrollInfo(hDlg, SB_VERT, &si);
	si.nMin = 0;
	si.nMax = dialogHeight;
	si.nPage = rc.bottom - rc.top;
	SetScrollInfo(hDlg, SB_VERT, &si, true);
}


/***********************************************************************************
* Dlg message handler                                                              *
***********************************************************************************/
LRESULT DlgEnterPictogramProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam) {
	static XPhonebook *pb = new XPhonebook();
	static BOOL graphics_initialized = false;
	static CHdc hdcInputItem(false);
	static CHdc hdcSelectedInputItem(false);
	static CHdc hdcBackground(false);
	static CFont fntInputItem;
	static CFont fntSelectedInputItem;
	static CFont fntInputChar;

	switch (msg) {
		case WM_INITDIALOG: {
			if (!InitDlgEnterPictogram(hDlg)) {
				DisplayDlg(MAIN, 0, false);
			}else {
				if (lParam) {
					if (!graphics_initialized) {
						CreateItem(&hdcInputItem, LoadBitmap(g.hInstance, (LPCWSTR)IDB_INPUTITEM), LoadBitmap(g.hInstance, (LPCWSTR)IDB_INPUTITEMTILE));
						CreateItem(&hdcSelectedInputItem, LoadBitmap(g.hInstance, (LPCWSTR)IDB_SELECTEDINPUTITEM), LoadBitmap(g.hInstance, (LPCWSTR)IDB_SELECTEDINPUTITEMTILE));

						fntInputItem.CreateFont(hDlg, 12, L"MS Sans Serif", true, false, false);
						fntSelectedInputItem.CreateFont(hDlg, 12, L"MS Sans Serif", true, false, false);
						fntInputChar.CreateFont(hDlg, 8, L"MS Sans Serif", false, false, false);

						pb->SetGraphics(&hdcInputItem, &hdcSelectedInputItem, NULL, NULL, NULL, NULL, &fntInputItem, &fntSelectedInputItem, NULL, NULL, &fntInputChar);

						graphics_initialized = true;

						RECT rc;
						GetClientRect(hDlg, &rc);

						pb->SetDialogSize(rc.right - rc.left, rc.bottom - rc.top);

						hdcBackground.SetSize(rc.right - rc.left, rc.bottom - rc.top);
						pb->SetBackground(&hdcBackground, 0);

						pb->SetAllowInputItem(true);

						pb->SetHideEmptyInputItem(false);

						pb->SetInputItemText(L"");

						pb->SetInput(XPhonebook::INPUT::NUMBERS/* : XPhonebook::INPUT::LETTERS*/);
					}

					pb->SetInputItemText(L"");

					// nastavit pozadí (s piktogramem)
					DrawBackgroundLogo(&hdcBackground);

					CHdc *hdcPictogram = (CHdc *)lParam;
					BitBlt(hdcBackground, hdcBackground.GetWidth() / 2 - hdcPictogram->GetWidth() / 2, hdcBackground.GetHeight() / 2 - hdcPictogram->GetHeight() / 2, hdcPictogram->GetWidth(), hdcPictogram->GetHeight(), *hdcPictogram, 0, 0, SRCCOPY);

					DelayKeyboard();
				}

				pb->Redraw();
				InvalidateRect(hDlg, NULL, false);
				UpdateWindow(hDlg);
			}
		}break; // end case WM_INITDIALOG

		case WM_HOTKEY: { // Override the back key to delete
			if (HIWORD(lParam) == VK_TBACK && !(lParam & MOD_KEYUP)) {
				if (!pb->GetInputItemText() || !*pb->GetInputItemText()) {
					SendMessage(hDlg, WM_COMMAND, IDM_ZPET, 0);
				}
			}
		}break;

		case WM_KEYDOWN: {
			if (g.skipUntilTime < GetTickCount()) {
				if (wParam == VK_RETURN) {
					SendMessage(hDlg, WM_COMMAND, IDOK, 0);
				}
			}
		}break; // end case USER_USERKEYDOWN

		case WM_COMMAND: {
			switch (LOWORD(wParam)) {
				case IDM_ZPET: {
					DisplayDlg(MAIN, 0, false);
				}break; // end case IDM_MOZNOSTI_KONEC

				case IDOK: {
					char *pictogram = ConvertToMultiByte(pb->GetInputItemText(), CP_ACP);
					if (pictogram) {
						if (g.sendMsgPictogram) free(g.sendMsgPictogram);
						g.sendMsgPictogram = pictogram;

						SendSMS(true);
					}

					DelayKeyboard();
				}break; // end case IDOK
			}
		}break; // end case WM_COMMAND

		case WM_LBUTTONUP:
		case WM_LBUTTONDBLCLK: {
			SendMessage(hDlg, WM_COMMAND, IDM_VYBRAT, 0);
		}break; // end case WM_LBUTTONDBLCLK
	}

	pb->OnMsg(hDlg, msg, wParam, lParam);

	HandleWindowScroll(hDlg, msg, wParam, lParam);

	return DefWindowProc(hDlg, msg, wParam, lParam);
}


/***********************************************************************************
* Shows wait dialog                                                                *
***********************************************************************************/
BOOL InitDlgEnterPictogram(HWND hWnd) {
	// set the title bar
	SetWindowText(hWnd, L"Opište kód:");

	// initialize menubar
	SHMENUBARINFO mbi;
	ZeroMemory(&mbi, sizeof(SHMENUBARINFO));
	mbi.cbSize      = sizeof(SHMENUBARINFO);
	//mbi.dwFlags = SHCMBF_EMPTYBAR;
	mbi.hwndParent  = hWnd;
	mbi.nToolBarId  = IDR_MENU_ENTERPICTOGRAM;
	mbi.hInstRes    = g.hInstance;

	if (!SHCreateMenuBar(&mbi)) return false;

	// Overriding Back button functionality
	SendMessage(mbi.hwndMB, SHCMBM_OVERRIDEKEY, VK_TBACK, 
				  MAKELPARAM(SHMBOF_NODEFAULT | SHMBOF_NOTIFY, 
							 SHMBOF_NODEFAULT | SHMBOF_NOTIFY));


	ShowWindow(g.hMenuBarCurrent, SW_HIDE);
	g.hMenuBarCurrent = mbi.hwndMB;
	ShowWindow(g.hMenuBarCurrent, SW_SHOW);

	return true;
}


/***********************************************************************************
* Draws logo                                                                       *
***********************************************************************************/
void DrawBackgroundLogo(CHdc *hdcBackground) {
	HBRUSH hBr = CreateSolidBrush(RGB(239, 239, 239));
	RECT rc = {0, 0, hdcBackground->GetWidth(), hdcBackground->GetHeight()};
	FillRect(*hdcBackground, &rc, hBr);
	DeleteObject(hBr);

	CHdc hdcLogo(false);
	hdcLogo.LoadBitmap(LoadBitmap(g.hInstance, (LPCWSTR)IDB_LOGO));

	BitBlt(*hdcBackground, hdcBackground->GetWidth() / 2 - hdcLogo.GetWidth() / 2, hdcBackground->GetHeight() / 2 - hdcLogo.GetHeight() / 2, hdcLogo.GetWidth(), hdcLogo.GetHeight(), hdcLogo, 0, 0, SRCCOPY);
}


/***********************************************************************************
* Send SMS                                                                         *
***********************************************************************************/
void SendSMS(BOOL send_retry) {
	g.AllocateUrl(1024*10);
	WCHAR *msgW, *sbjW = NULL;
	DISPLAY display = g.display;
	g.display = MAIN;
	if (!send_retry || !g.sendMsgMessageId || !*g.sendMsgMessageId) SendMessage(g.hMain, USER_GETMSG, 0, (LPARAM)&msgW);
	else msgW = _wcsdup(L"");
	g.display = display;
	char *msg = ConvertToMultiByte(msgW, CP_ACP);
	char *sbj = ConvertToMultiByte(g.sendMsgEmailSubject, CP_ACP);
	
	char *encPwd = EncodeURL(g.setting.GetPwd());
	char *encMsg = EncodeURL(msg);
	char *encSbj = EncodeURL(sbj);
	char *encSrcEmail = EncodeURL(g.setting.GetEmail());
	char *encDestEmail = EncodeURL(g.sendMsgReceiver);
	
	if (g.sendMsgSendTo == CDlgAddressBook::SENDTO::SMS) sprintf(g.url, "%s/send.php?s_n=%s&pwd=%s&d_n=%s&client=%s&ver=%s&msg=%s&sendway=%i&varsformat=csv&send_retry=%i&message_id=%s&pictogram=%s", g.giboServer, g.setting.GetSrcNum(), encPwd, g.sendMsgReceiver, g.client, g.szVersionA, encMsg, g.sendMsgSendTo, send_retry ? 1 : 0, g.sendMsgMessageId ? g.sendMsgMessageId : "", g.sendMsgPictogram ? g.sendMsgPictogram : "");
	else if (g.sendMsgSendTo == CDlgAddressBook::SENDTO::WEBSMS) sprintf(g.url, "%s/send.php?s_n=%s&pwd=%s&d_n=%s&client=%s&ver=%s&msg=%s&sendway=%i&varsformat=csv&send_retry=%i&message_id=%s&pictogram=%s", g.giboServer, g.setting.GetSrcNum(), encPwd, g.sendMsgReceiver, g.client, g.szVersionA, encMsg, g.sendMsgSendTo, send_retry ? 1 : 0, g.sendMsgMessageId ? g.sendMsgMessageId : "", g.sendMsgPictogram ? g.sendMsgPictogram : "");
	else if (g.sendMsgSendTo == CDlgAddressBook::SENDTO::MOBEMAIL) sprintf(g.url, "%s/send.php?s_n=%s&pwd=%s&d_n=%s&client=%s&ver=%s&msg=%s&sendway=%i&varsformat=csv&send_retry=%i&message_id=%s&pictogram=%s", g.giboServer, g.setting.GetSrcNum(), encPwd, g.sendMsgReceiver, g.client, g.szVersionA, encMsg, g.sendMsgSendTo, send_retry ? 1 : 0, g.sendMsgMessageId ? g.sendMsgMessageId : "", g.sendMsgPictogram ? g.sendMsgPictogram : "");
	else if (g.sendMsgSendTo == CDlgAddressBook::SENDTO::EMAIL) sprintf(g.url, "%s/send.php?s_n=%s&pwd=%s&s_e=%s&d_e=%s&sbj=%s&client=%s&ver=%s&msg=%s&sendway=%i&varsformat=csv&send_retry=%i&message_id=%s&pictogram=%s", g.giboServer, g.setting.GetSrcNum(), encPwd, encSrcEmail, encDestEmail, encSbj, g.client, g.szVersionA, encMsg, g.sendMsgSendTo, send_retry ? 1 : 0, g.sendMsgMessageId ? g.sendMsgMessageId : "", g.sendMsgPictogram ? g.sendMsgPictogram : "");
	else if (g.sendMsgSendTo == CDlgAddressBook::SENDTO::MAILBOX) sprintf(g.url, "%s/send.php?s_n=%s&pwd=%s&d_n=%s&client=%s&ver=%s&msg=%s&sendway=%i&varsformat=csv&send_retry=%i&message_id=%s&pictogram=%s", g.giboServer, g.setting.GetSrcNum(), encPwd, g.sendMsgReceiver, g.client, g.szVersionA, encMsg, g.sendMsgSendTo, send_retry ? 1 : 0, g.sendMsgMessageId ? g.sendMsgMessageId : "", g.sendMsgPictogram ? g.sendMsgPictogram : "");
	else if (g.sendMsgSendTo == CDlgAddressBook::SENDTO::PAIDSMS) sprintf(g.url, "%s/send.php?s_n=%s&pwd=%s&d_n=%s&client=%s&ver=%s&msg=%s&sendway=%i&varsformat=csv&send_retry=%i&message_id=%s&pictogram=%s", g.giboServer, g.setting.GetSrcNum(), encPwd, g.sendMsgReceiver, g.client, g.szVersionA, encMsg, g.sendMsgSendTo, send_retry ? 1 : 0, g.sendMsgMessageId ? g.sendMsgMessageId : "", g.sendMsgPictogram ? g.sendMsgPictogram : "");
	
	delete[] msgW; delete[] msg;
	delete[] sbjW; delete[] sbj;
	free(encPwd); free(encMsg); free(encSbj); free(encSrcEmail); free(encDestEmail);

	DisplayDlg(WAIT, true, true);
	g.displayPrev = MAIN;
	OpenPageByThread(g.url, L"Status", g.hMain, USER_FINISHDOWNLOADING, CDlgStatus::TYPE::SENDMSG, ContinueWithSending);
}

/***********************************************************************************
* Vytvoøí brush z hdc                                                              *
***********************************************************************************/
HBRUSH CreateBrushFromHDC(HDC hdc, int wid, int hei) {
	BITMAPINFO *bi;

	bi = (LPBITMAPINFO)malloc(sizeof(BITMAPINFO) + (wid * hei * sizeof(RGBQUAD)));

	bi->bmiHeader.biSize = sizeof(bi->bmiHeader);
	bi->bmiHeader.biWidth = wid;
	bi->bmiHeader.biHeight = hei;
	bi->bmiHeader.biPlanes = 1;
	bi->bmiHeader.biBitCount = 32;
	bi->bmiHeader.biCompression = BI_RGB;
	bi->bmiHeader.biSizeImage = 0;
	bi->bmiHeader.biXPelsPerMeter = 0;
	bi->bmiHeader.biYPelsPerMeter = 0;
	bi->bmiHeader.biClrUsed = 0;
	bi->bmiHeader.biClrImportant = 0;

	RGBQUAD *clr = (RGBQUAD *)bi->bmiColors;
	clr += wid * hei;

	for (int y = 0; y < hei; y++) {
		for (int x = wid; x > 0; x--) {
			*(DWORD *)clr = GetPixel(hdc, x, y);
			clr--;
		}
	}

	HBRUSH hbr = CreateDIBPatternBrushPt(bi, DIB_RGB_COLORS);
	if (!hbr) {
		int err = GetLastError();
		WCHAR b[10];
		wsprintf(b, L"%i", err);
		MessageBox(0, b, 0, 0);
	}
	
	return hbr;
}