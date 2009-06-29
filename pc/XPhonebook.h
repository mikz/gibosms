#ifndef __XPHONEBOOK_H__
#define __XPHONEBOOK_H__

#include "CArray.h"
#include "SetCharPtr.h"
#include "CHdc.h"
#include "CFont.h"


class XPhonebookItem {
private:
	WCHAR *m_text;
	CArray<XPhonebookItem *> *m_subItems;
	void *m_data;
	BOOL m_expanded;
	DWORD m_selection; // LOWORD = selection start, HIWORD = selection length
	CHdc *m_hdcItem;
	CHdc *m_hdcSelectedItem;

public:
	WCHAR *GetText() {return this->m_text ? this->m_text : L"";}
	//CArray<XPhonebookItem *> *GetSubItems() {return this->m_subItems;}
	void *GetData()  {return this->m_data;}
	BOOL GetExpanded()  {return this->m_expanded;}
	BOOL GetSelection()  {return this->m_selection;}
	CHdc *GetHdcItem() {return m_hdcItem;}
	CHdc *GetHdcSelectedItem() {return m_hdcSelectedItem;}

	void SetText(WCHAR *val) {SetCharPtr(&this->m_text, val);}
	//void SetSubItems(CArray<XPhonebookItem *> *subItems) {this->m_subItems = subItems;}
	DWORD GetSubItemsCount() {if (this->m_subItems) return this->m_subItems->num; return 0;}
	void SetData(void *data) {this->m_data = data;}
	void SetExpanded(BOOL expanded) {this->m_expanded = expanded;}
	void SetSelection(DWORD selection) {this->m_selection = selection;}
	void SetGraphics(CHdc *hdcItem, CHdc *hdcSelectedItem) {m_hdcItem = hdcItem; m_hdcSelectedItem = hdcSelectedItem;}

	void AddSubItem(XPhonebookItem *pi) {
		if (!m_subItems) m_subItems = new CArray<XPhonebookItem *>;
		m_subItems->Add(pi);
	}
	XPhonebookItem *GetSubItem(DWORD n) {if (n < GetSubItemsCount()) return (*m_subItems)[n]; return NULL;}
	BOOL RemoveSubItem(DWORD n) {
		if (n < GetSubItemsCount()) {
			m_subItems->Remove(n);
			return true;
		}
		return false;
	}
	void RemoveAllSubItems() {
		for (DWORD i = GetSubItemsCount() - 1; i < GetSubItemsCount(); i--)
			this->RemoveSubItem(i);
	}

	XPhonebookItem() {
		m_text = NULL;
		m_subItems = NULL;
		m_data = NULL;
		m_expanded = false;
		m_selection = 0;
		m_hdcItem = NULL;
		m_hdcSelectedItem = NULL;
	}
	~XPhonebookItem() {
		SetText(NULL);
		RemoveAllSubItems();
		if (m_subItems) delete m_subItems;
	}
};

//------------

#define NOT_SELECTED  0xFFFFFFFF

class XPhonebook {
public:
	class INPUT {
	public:
		enum _ENUM {NUMBERS, LETTERS, NOINPUT, RAW};
	};

private:
	CArray<XPhonebookItem *> m_data;
	CArray<XPhonebookItem *> m_items;
	DWORD m_topItem;
	DWORD m_selectedItem;
	DWORD m_selectedSubItem;
	CHdc m_hStage;
	HWND m_notifyWnd;
	UINT m_notifyMsg;
	BOOL m_inputItemShown;
	BOOL m_allowInputItem;
	BOOL m_hideEmptyInputItem;
	CHdc *m_hdcInputItem;
	CHdc *m_hdcSelectedInputItem;
	CHdc *m_hdcItem;
	CHdc *m_hdcSelectedItem;
	CHdc *m_hdcExpandableItem;
	CHdc *m_hdcSelectedExpandableItem;
	CFont *m_fntInputItem;
	CFont *m_fntSelectedInputItem;
	CFont *m_fntItem;
	CFont *m_fntSelectedItem;
	CFont *m_fntInputChar;
	WCHAR *m_inputItemText;
	int m_inputItemTextMargin;
	int m_subItemOffset;
	int m_itemTextOffset;
	DWORD m_nItemsShown;
	COLORREF m_clrBg;
	COLORREF m_clrItemFg;
	COLORREF m_clrSelectionItemFg;
	COLORREF m_clrSelectionItemBg;
	COLORREF m_clrSelectionSelectedItemFg;
	COLORREF m_clrSelectionSelectedItemBg;
	CHdc *m_hdcBackground;
	WCHAR **m_keyboardMap;
	WCHAR m_actInputChar;
	INPUT::_ENUM m_input;
	UINT m_idTimerInput;

private:
	void InputAddChar(WCHAR c);
	void InputChangeLastChar(WCHAR c);
	void InputRemoveLastChar();
	void InputDrawChar();
	BOOL InputGetCharData(WCHAR c, int *iStr, int *iLetter);
	WCHAR InputGetChar(WCHAR pressedChar, BOOL *replaceActual);
	BOOL T9CharMatches(WCHAR a, WCHAR b);

public:
	void SetNotification(HWND notifyWnd, UINT notifyMsg) {this->m_notifyWnd = notifyWnd; this->m_notifyMsg = notifyMsg;}
	void SetInputItemShown(BOOL inputItemShown) {m_inputItemShown = inputItemShown;}
	void SetAllowInputItem(BOOL allowInputItem) {m_allowInputItem = allowInputItem;}
	void SetHideEmptyInputItem(BOOL hideEmptyInputItem) {m_hideEmptyInputItem = hideEmptyInputItem; if (!m_hideEmptyInputItem) m_inputItemShown = true;}
	void SetInputItemText(WCHAR *inputItemText) {SetCharPtr(&this->m_inputItemText, inputItemText);};
	WCHAR *GetInputItemText() {return this->m_inputItemText ? this->m_inputItemText : L"";};
	void SetInput(XPhonebook::INPUT::_ENUM input) {m_input = input;};
	
	void SetDialogSize(int wid, int hei) {this->m_hStage.SetSize(wid, hei);}

	BOOL Add(WCHAR *text, void *data);
	BOOL Add(XPhonebookItem *item);
	BOOL Remove(DWORD n);
	void RemoveAll();
	void RemoveAllItemsData();
	XPhonebookItem *Get(DWORD n, BOOL fromFiltered);
	LPARAM OnMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	DWORD GetItemCount();
	DWORD GetDisplayedItemsCount();

	BOOL Redraw(BOOL dontDraw = false);
	void SetGraphics(CHdc *hdcInputItem, CHdc *hdcSelectedInputItem, CHdc *hdcItem, CHdc *hdcSelectedItem, CHdc *hdcExpandableItem, CHdc *hdcSelectedExpandableItem, CFont *fntInputItem, CFont *fntSelectedInputItem, CFont *fntItem, CFont *fntSelectedItem, CFont *fntInputChar) {m_hdcInputItem = hdcInputItem; m_hdcSelectedInputItem = hdcSelectedInputItem; m_hdcItem = hdcItem; m_hdcSelectedItem = hdcSelectedItem; m_hdcExpandableItem = hdcExpandableItem; m_hdcSelectedExpandableItem = hdcSelectedExpandableItem; m_fntInputItem = fntInputItem; m_fntSelectedInputItem = fntSelectedInputItem; m_fntItem = fntItem; m_fntSelectedItem = fntSelectedItem; m_fntInputChar = fntInputChar;}
	void SetBackground(CHdc *hdcBackground, COLORREF clrBg) {m_hdcBackground = hdcBackground; m_clrBg = clrBg;}
	void RecomputeItemsCount();

	BOOL SelectNextItem();
	BOOL SelectPreviousItem();
	BOOL SetSelectedItem(DWORD nItem, DWORD nSubItem);
	XPhonebookItem *GetSelectedItem();
	DWORD GetSelectedItemIndex();
	DWORD GetSelectedSubItemIndex();
	void CenterSelectedItem();
	DWORD GetSelectedItemDataIndex();

	BOOL ExpandItem(DWORD n);
	BOOL CollapseItem(DWORD n);

	BOOL GetItemAtPos(DWORD nTopItem, DWORD *nItem, DWORD *nSubItem);
	BOOL GetItemAtPos(POINT pt, DWORD *nItem, DWORD *nSubItem);
	BOOL GetItemPos(DWORD nItem, DWORD nSubItem, DWORD *nTopItem);

	void ApplyItemFilter();

	DWORD Exists(WCHAR *itemText, BOOL inFiltered);


	XPhonebook() : m_hStage(false, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN)) {
		SetBkMode(m_hStage, TRANSPARENT);

		m_topItem = 0;
		m_selectedItem = NOT_SELECTED;
		m_selectedSubItem = NOT_SELECTED;
		m_inputItemShown = false;
		m_allowInputItem = false;
		m_hideEmptyInputItem = true;
		m_hdcInputItem = NULL;
		m_hdcSelectedInputItem = NULL;
		m_hdcItem = NULL;
		m_hdcSelectedItem = NULL;
		m_hdcExpandableItem = NULL;
		m_hdcSelectedExpandableItem = NULL;
		m_fntInputItem = NULL;
		m_fntSelectedInputItem = NULL;
		m_fntItem = NULL;
		m_fntSelectedItem = NULL;
		m_fntInputChar = NULL;
		m_inputItemText = NULL;
		m_inputItemTextMargin = 5;
		m_subItemOffset = 10;
		m_itemTextOffset = 27;
		m_nItemsShown = 0;
		m_clrBg = RGB(239, 239, 239);
		m_clrItemFg = RGB(0, 0, 0);
		m_clrSelectionItemFg = RGB(0, 0, 0);
		m_clrSelectionItemBg = RGB(192, 189, 210);
		m_clrSelectionSelectedItemFg = RGB(0, 0, 0);
		m_clrSelectionSelectedItemBg = RGB(218, 220, 228);
		m_hdcBackground = NULL;
		m_keyboardMap = new WCHAR *[10];
		m_keyboardMap[0] = L"0+";
		m_keyboardMap[1] = L".@-_;1";
		m_keyboardMap[2] = L"abc2αθ";
		m_keyboardMap[3] = L"def3ιμο";
		m_keyboardMap[4] = L"ghi4ν";
		m_keyboardMap[5] = L"jkl5";
		m_keyboardMap[6] = L"mno6σς";
		m_keyboardMap[7] = L"pqrs7ψ";
		m_keyboardMap[8] = L"tuv8ϊω";
		m_keyboardMap[9] = L"wxyz9ύ";
		m_actInputChar = '\0';
		m_idTimerInput = 0x100 + 1;
		m_input = XPhonebook::INPUT::NOINPUT;
	}
	~XPhonebook() {
		RemoveAll();
	}
};


#endif //__XPHONEBOOK_H__