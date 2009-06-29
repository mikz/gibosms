#include "stdafx.h"
#include "XPhonebook.h"


/***********************************************************************************
*                              CLASS XPhonebook                                    *
***********************************************************************************/
//----------------------------------------------------------------------------------
BOOL XPhonebook::Add(WCHAR *text, void *data) {
	XPhonebookItem *pi = new XPhonebookItem();
	pi->SetText(text);
	pi->SetData(data);
	
	BOOL ok = this->Add(pi);
	if (!ok) delete pi;
	return ok;
}
//----------------------------------------------------------------------------------
BOOL XPhonebook::Add(XPhonebookItem *item) {
	BOOL retVal = this->m_data.Add(item);
	this->ApplyItemFilter();
	return retVal;
}
//----------------------------------------------------------------------------------
BOOL XPhonebook::Remove(DWORD n) {
	if (n < this->m_items.num) {
		for (DWORD i = 0; i < this->m_data.num; i++) {
			if (this->m_data[i] == this->m_items[n]) {
				delete this->m_data[i];
				this->m_data.Remove(i);
				break;
			}
		}
		this->m_items.Remove(n);
		return true;
	}
	return false;
}
//----------------------------------------------------------------------------------
void XPhonebook::RemoveAll() {
	for (DWORD i = this->m_items.num - 1; i < this->m_items.num; i--)
		this->Remove(i);
}
//----------------------------------------------------------------------------------
void XPhonebook::RemoveAllItemsData() {
	DWORD i;
	for (i = this->m_items.num - 1; i < this->m_items.num; i--) {
		this->m_items.Remove(i);
	}
	for (i = this->m_data.num - 1; i < this->m_data.num; i--) {
		delete this->m_data[i];
		this->m_data.Remove(i);
	}
}
//----------------------------------------------------------------------------------
XPhonebookItem *XPhonebook::Get(DWORD n, BOOL fromFiltered) {
	if (fromFiltered && n < this->m_items.num) return this->m_items[n];
	if (n < this->m_data.num) return this->m_data[n];
	return NULL;
}
//----------------------------------------------------------------------------------
LPARAM XPhonebook::OnMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	BOOL refresh = false;
	
	switch (msg) {
		case WM_KEYDOWN: {
			if (wParam == VK_DOWN) {
				refresh = this->SelectNextItem();
				this->CenterSelectedItem();
			}else if (wParam == VK_UP) {
				refresh = this->SelectPreviousItem();
				this->CenterSelectedItem();
			}else if (wParam == VK_RIGHT) {
				refresh = this->ExpandItem(this->m_selectedItem);
				this->CenterSelectedItem();
			}else if (wParam == VK_LEFT) {
				refresh = this->CollapseItem(this->m_selectedItem);
				this->CenterSelectedItem();
			}else if (wParam == VK_BACK) {
				if (*this->GetInputItemText()) {
					this->InputRemoveLastChar();

					this->m_inputItemShown = this->m_allowInputItem && (!this->m_hideEmptyInputItem || (this->GetInputItemText() && *this->GetInputItemText()));

					this->ApplyItemFilter();

					if (this->m_inputItemShown) {
						this->m_selectedItem = NOT_SELECTED;
						this->m_selectedSubItem = NOT_SELECTED;
					}else if (this->GetDisplayedItemsCount()) {
						this->m_selectedItem = 0;
						this->m_selectedSubItem = NOT_SELECTED;
					}
					
					this->CenterSelectedItem();

					refresh = true;
				}
			}
		}break; // end case WM_KEYDOWN

		case WM_LBUTTONDOWN: {
			POINT pt;
			DWORD nItem, nSubItem;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);

			if (GetItemAtPos(pt, &nItem, &nSubItem)) {
				SetSelectedItem(nItem, nSubItem);

				refresh = true;
			}
		}break; // end case WM_LBUTTONDOWN

		case WM_TIMER: {
			if (wParam == this->m_idTimerInput) {
				this->m_actInputChar = '\0';
				refresh = true;

				KillTimer(hWnd, this->m_idTimerInput);
			}
		}break; //

		case WM_CHAR: {
			if (wParam == VK_RETURN || wParam == _T('\n') || wParam == VK_BACK)
				return 0;
			
			if (((wParam >= '0' && wParam <= '9') || (wParam >= 'a' && wParam <= 'z') || (wParam >= 'A' && wParam <= 'Z')) || this->m_input == XPhonebook::INPUT::RAW) {
 				BOOL replaceActual;
				WCHAR c = this->InputGetChar((WCHAR)wParam, &replaceActual);

				if (c) {
					if (!replaceActual) {
						this->InputAddChar(c);
					}else {
						this->InputChangeLastChar(c);
					}
					this->m_inputItemShown = this->m_allowInputItem && (!this->m_hideEmptyInputItem || (this->GetInputItemText() && *this->GetInputItemText()));

					this->ApplyItemFilter();

					if (this->m_inputItemShown) {
						this->m_selectedItem = NOT_SELECTED;
						this->m_selectedSubItem = NOT_SELECTED;
						
						this->CenterSelectedItem();
					}

					SetTimer(hWnd, this->m_idTimerInput, 1000, NULL);

					refresh = true;
				}
			}
		}break; // end case WM_CHAR

		case WM_PAINT: {
			PAINTSTRUCT ps;
			BeginPaint(hWnd, &ps);
			BitBlt(ps.hdc, 0, 0, this->m_hStage.GetWidth(), this->m_hStage.GetHeight(), this->m_hStage, 0, 0, SRCCOPY);
			EndPaint(hWnd, &ps);
		}break; // end case WM_PAINT
	} // end switch (msg)

	// should refresh window because of changes?
	if (refresh) {
		this->Redraw();

		InvalidateRect(hWnd, NULL, false);
		UpdateWindow(hWnd);
	}

	return 0;
}
//----------------------------------------------------------------------------------
DWORD XPhonebook::GetItemCount() {
	return this->m_items.num;
}
//----------------------------------------------------------------------------------
DWORD XPhonebook::GetDisplayedItemsCount() {
	DWORD nCount = 0;
	
	for (DWORD i = 0; i < this->GetItemCount(); i++) {
		XPhonebookItem *pi = this->Get(i, true);
		if (pi) {
			nCount++;
			if (pi->GetExpanded())
				nCount += pi->GetSubItemsCount();
		}
	}
	return nCount;
}
//----------------------------------------------------------------------------------
BOOL XPhonebook::Redraw(BOOL dontDraw) {
	// clear background
	HBRUSH hBr = CreateSolidBrush(m_clrBg);
	RECT rc = {0, 0, m_hStage.GetWidth(), m_hStage.GetHeight()};
	FillRect(m_hStage, &rc, hBr);
	DeleteObject(hBr);

	if (m_hdcBackground) {
		BitBlt(m_hStage, 0, 0, m_hdcBackground->GetWidth(), m_hdcBackground->GetHeight(), *m_hdcBackground, 0, 0, SRCCOPY);
	}

	
	m_nItemsShown = 0;
	UINT y = 0;

	// draw input item
	if (m_inputItemShown) {
		if (!m_hdcInputItem || !m_hdcSelectedInputItem) return false;

		SetBkMode(m_hStage, TRANSPARENT);
		SetTextColor(m_hStage, RGB(0, 0, 0));

		// draw background
		CHdc *sprite = m_selectedItem != NOT_SELECTED ? m_hdcInputItem : m_hdcSelectedInputItem;
		CFont *font = m_selectedItem != NOT_SELECTED ? m_fntInputItem : m_fntSelectedInputItem;
		if (!dontDraw) {
			BitBlt(m_hStage, 0, y, sprite->GetWidth(), sprite->GetHeight(), *sprite, 0, 0, SRCCOPY);

			// draw item text
			if (m_inputItemText) {
				RECT rc = {0 + m_inputItemTextMargin, y, sprite->GetWidth() - m_inputItemTextMargin, y + sprite->GetHeight()};
				font->SelectInto(m_hStage);
				DrawText(m_hStage, m_inputItemText, -1, &rc, DT_RIGHT | DT_SINGLELINE | DT_VCENTER | DT_NOPREFIX);
				font->SelectOutOf();
			}
		}

		y += sprite->GetHeight();

		this->InputDrawChar();
	}

	// draw all other items
	DWORD nTopItem = m_topItem, nItem, nSubItem;
	
	m_nItemsShown = m_hdcItem ? (m_hStage.GetHeight() - y) / m_hdcItem->GetHeight() : 0;
	
	while (y < m_hStage.GetHeight()) {
		if (this->GetItemAtPos(nTopItem, &nItem, &nSubItem)) {
			XPhonebookItem *pi = this->Get(nItem, true);
			BOOL isSubItem = nSubItem != NOT_SELECTED;
			if (isSubItem) {
				if (nSubItem < pi->GetSubItemsCount()) {
					pi = pi->GetSubItem(nSubItem);
				}else break;
			}
			// draw background
			BOOL isSelected = m_selectedItem == nItem && m_selectedSubItem == nSubItem;
			CHdc *sprite;
			if (!isSelected && pi->GetHdcItem()) sprite = pi->GetHdcItem();
			else if (isSelected && pi->GetHdcSelectedItem()) sprite = pi->GetHdcSelectedItem();
			else sprite = isSelected ? (pi->GetSubItemsCount() ? m_hdcSelectedExpandableItem : m_hdcSelectedItem) : (pi->GetSubItemsCount() ? m_hdcExpandableItem : m_hdcItem);

			if (!sprite) break;
			CFont *font = isSelected ? m_fntSelectedItem : m_fntItem;
			if (!dontDraw) {
				BitBlt(m_hStage, isSubItem ? m_subItemOffset : 0, y, sprite->GetWidth() - (isSubItem ? m_subItemOffset : 0), sprite->GetHeight(), *sprite, 0, 0, SRCCOPY);
				
				// draw item text
				if (pi->GetText()) {
					RECT rc = {0 + (isSubItem ? m_subItemOffset : 0) + m_itemTextOffset, y, sprite->GetWidth(), y + sprite->GetHeight()};
					font->SelectInto(m_hStage);
					SetBkMode(m_hStage, TRANSPARENT);
					SetTextColor(m_hStage, m_clrItemFg);
					DrawText(m_hStage, pi->GetText(), -1, &rc, DT_LEFT | DT_SINGLELINE | DT_VCENTER | DT_NOPREFIX);

					// draw selection
					if (HIWORD(pi->GetSelection())) {
						SetBkMode(m_hStage, OPAQUE);
						SetBkColor(m_hStage, isSelected ? m_clrSelectionSelectedItemBg : m_clrSelectionItemBg);
						SIZE sz;
						GetTextExtentPoint32(m_hStage, pi->GetText(), LOWORD(pi->GetSelection()), &sz);
						rc.left += sz.cx;
						SetTextColor(m_hStage, isSelected ? m_clrSelectionSelectedItemFg : m_clrSelectionItemFg);
						DrawText(m_hStage, pi->GetText() + LOWORD(pi->GetSelection()), HIWORD(pi->GetSelection()), &rc, DT_LEFT | DT_SINGLELINE | DT_VCENTER | DT_NOPREFIX);
					}

					font->SelectOutOf();
				}
			}

			y += sprite->GetHeight();

			if (!m_nItemsShown) m_nItemsShown =  (m_hStage.GetHeight() - y) / sprite->GetHeight();

			//m_nItemsShown++;

			nTopItem++;
		}else break;
	}

	return true;
}
//----------------------------------------------------------------------------------
void XPhonebook::RecomputeItemsCount() {
	this->Redraw(true);
}
//----------------------------------------------------------------------------------
BOOL XPhonebook::SelectNextItem() {
	if (this->m_selectedItem != NOT_SELECTED) {
		XPhonebookItem *pi = this->Get(this->m_selectedItem, true);
		if (!pi->GetExpanded()) {
			if (this->m_selectedItem + 1 < this->GetItemCount()) {
				m_selectedItem++;
				return true;
			}
		}else {
			if (this->m_selectedSubItem == NOT_SELECTED) {
				this->m_selectedSubItem = 0;
				return true;
			}else {
				if (this->m_selectedSubItem + 1 < pi->GetSubItemsCount()) {
					this->m_selectedSubItem++;
					return true;
				}else {
					if (this->m_selectedItem + 1 < this->GetItemCount()) {
						this->m_selectedSubItem = NOT_SELECTED;
						this->m_selectedItem++;
						return true;
					}
				}
			}
		}
	}else {
		if (this->GetItemCount()) {
			// select the first item
			this->m_selectedItem = 0;
			this->m_selectedSubItem = NOT_SELECTED;
			return true;
		}
	}
	return false;
}
//----------------------------------------------------------------------------------
BOOL XPhonebook::SelectPreviousItem() {
	if (this->m_selectedItem == NOT_SELECTED) return false;

	if (this->m_selectedItem == 0 && this->m_selectedSubItem == NOT_SELECTED) {
		if (this->m_inputItemShown) {
			this->m_selectedItem = NOT_SELECTED;
			return true;
		}
	}else {
		if (this->m_selectedSubItem != NOT_SELECTED) {
			if (this->m_selectedSubItem == 0) this->m_selectedSubItem = NOT_SELECTED;
			else this->m_selectedSubItem--;
			return true;
		}else {
			XPhonebookItem *piPrev = this->Get(this->m_selectedItem - 1, true);
			if (piPrev) {
				if (piPrev->GetExpanded() && piPrev->GetSubItemsCount()) {
					this->m_selectedItem--;
					this->m_selectedSubItem = piPrev->GetSubItemsCount() - 1;
					return true;
				}else {
					this->m_selectedItem--;
					this->m_selectedSubItem = NOT_SELECTED;
					return true;
				}
			}
		}
	}
	return false;
}
//----------------------------------------------------------------------------------
BOOL XPhonebook::SetSelectedItem(DWORD nItem, DWORD nSubItem) {
	if (nItem == NOT_SELECTED) {
		this->m_selectedItem = NOT_SELECTED;
		this->m_selectedSubItem = NOT_SELECTED;
	}else if (nItem < this->m_items.num) {
		if (nSubItem == NOT_SELECTED) {
			this->m_selectedItem = nItem;
			this->m_selectedSubItem = nSubItem;
		}else {
			if (this->m_items[nItem] && nSubItem < this->m_items[nItem]->GetSubItemsCount()) {
				this->m_selectedItem = nItem;
				this->m_selectedSubItem = nSubItem;
			}
		}
	}
	return false;
}
//----------------------------------------------------------------------------------
XPhonebookItem *XPhonebook::GetSelectedItem() {
	if (this->m_selectedItem == NOT_SELECTED) return NULL;

	XPhonebookItem *pi = this->Get(this->m_selectedItem, true);
	if (pi && this->m_selectedSubItem != NOT_SELECTED && this->m_selectedSubItem < pi->GetSubItemsCount()) {
		return pi->GetSubItem(this->m_selectedSubItem);
	}
	return pi;
}
//----------------------------------------------------------------------------------
DWORD XPhonebook::GetSelectedItemIndex() {
	return this->m_selectedItem;
}
//----------------------------------------------------------------------------------
DWORD XPhonebook::GetSelectedSubItemIndex() {
	return this->m_selectedSubItem;
}
//----------------------------------------------------------------------------------
DWORD XPhonebook::GetSelectedItemDataIndex() {
	if (this->m_selectedItem == NOT_SELECTED) return this->m_selectedItem;
	XPhonebookItem *pi = this->Get(this->m_selectedItem, true);
	if (!pi) return NOT_SELECTED;
	for (DWORD i = 0; i < this->m_data.num; i++) {
		if (this->m_data[i] == pi) {
			return i;
		}
	}
	return NOT_SELECTED;
}
//----------------------------------------------------------------------------------
void XPhonebook::CenterSelectedItem() {
	if (this->m_selectedItem == NOT_SELECTED) {
		this->m_topItem = 0;
		if (this->m_allowInputItem && (!this->m_hideEmptyInputItem || (this->GetInputItemText() && *this->GetInputItemText()))) this->m_inputItemShown = true;
		else this->m_inputItemShown = false;
	}else {
		// get real position of selected item
		DWORD nPos;
		if (!this->GetItemPos(this->m_selectedItem, this->m_selectedSubItem, &nPos)) return;

		for (int i = 0; i < 2; i++) {
			this->m_inputItemShown = i == 0;

			this->RecomputeItemsCount();
			// get center
			DWORD midItem = this->m_nItemsShown / 2;
			
			if (this->GetDisplayedItemsCount() <= this->m_nItemsShown || nPos <= midItem) {
				this->m_topItem = 0;
				if (i != 1 && this->m_allowInputItem && (!this->m_hideEmptyInputItem || (this->GetInputItemText() && *this->GetInputItemText()))) this->m_inputItemShown = true;
				else this->m_inputItemShown = false;
				return;
			}else if (midItem + nPos >= this->GetDisplayedItemsCount()) {
				this->m_topItem = this->GetDisplayedItemsCount() - this->m_nItemsShown;
			}else {
				this->m_topItem = nPos - midItem;
			}
		}
	}
}
//----------------------------------------------------------------------------------
BOOL XPhonebook::ExpandItem(DWORD n) {
	XPhonebookItem *pi = this->Get(n, true);
	if (pi) {
		if (pi->GetSubItemsCount()) {
			pi->SetExpanded(true);
			return true;
		}
	}
	return false;
}
//----------------------------------------------------------------------------------
BOOL XPhonebook::CollapseItem(DWORD n) {
	XPhonebookItem *pi = this->Get(n, true);
	if (pi) {
		pi->SetExpanded(false);
		if (this->m_selectedItem == n)
			this->m_selectedSubItem = NOT_SELECTED;
		return true;
	}
	return false;
}
//----------------------------------------------------------------------------------
BOOL XPhonebook::GetItemAtPos(DWORD nTopItem, DWORD *pnItem, DWORD *pnSubItem) {
	if (!pnItem || !pnSubItem) return false;

	DWORD nCount = 0;

	for (DWORD i = 0; i < this->GetItemCount(); i++) {
		XPhonebookItem *pi = this->Get(i, true);
		if (pi) {
			nCount++;
			if (nCount - 1 == nTopItem) {
				*pnItem = i;
				*pnSubItem = NOT_SELECTED;
				return true;
			}
			DWORD nSubItemsCount = 0;
			if (pi->GetExpanded() && pi->GetSubItemsCount()) {
				nCount += (nSubItemsCount = pi->GetSubItemsCount());
			}
			if (nCount - 1 >= nTopItem) {
				*pnItem = i;
				*pnSubItem = nSubItemsCount - (nCount - nTopItem);
				return true;
			}
		}
	}

	return false;
}
//----------------------------------------------------------------------------------
BOOL XPhonebook::GetItemAtPos(POINT pt, DWORD *pnItem, DWORD *pnSubItem) {
	if (!pnItem || !pnSubItem) return false;

	UINT y = 0;

	// draw input item
	if (m_inputItemShown) {
		if (!m_hdcInputItem || !m_hdcSelectedInputItem) return false;

		// draw background
		CHdc *sprite = m_selectedItem != NOT_SELECTED ? m_hdcInputItem : m_hdcSelectedInputItem;

		if ((int)(y + sprite->GetHeight()) > pt.y) {
			*pnItem = NOT_SELECTED;
			*pnSubItem = NOT_SELECTED;
			return true;
		}
		y += sprite->GetHeight();
	}


	// draw all other items
	DWORD nTopItem = m_topItem, nItem, nSubItem;
	
	while (y < m_hStage.GetHeight()) {
		if (this->GetItemAtPos(nTopItem, &nItem, &nSubItem)) {
			XPhonebookItem *pi = this->Get(nItem, true);
			BOOL isSubItem = nSubItem != NOT_SELECTED;
			if (isSubItem) {
				if (nSubItem < pi->GetSubItemsCount()) {
					pi = pi->GetSubItem(nSubItem);
				}else break;
			}
			// draw background
			BOOL isSelected = m_selectedItem == nItem && m_selectedSubItem == nSubItem;
			CHdc *sprite;
			if (!isSelected && pi->GetHdcItem()) sprite = pi->GetHdcItem();
			else if (isSelected && pi->GetHdcSelectedItem()) sprite = pi->GetHdcSelectedItem();
			else sprite = isSelected ? (pi->GetSubItemsCount() ? m_hdcSelectedExpandableItem : m_hdcSelectedItem) : (pi->GetSubItemsCount() ? m_hdcExpandableItem : m_hdcItem);

			if (!sprite) break;

			if ((int)(y + sprite->GetHeight()) > pt.y) {
				*pnItem = nItem;
				*pnSubItem = nSubItem;
				return true;
			}
			y += sprite->GetHeight();

			nTopItem++;
		}else break;
	}

	return false;
}
//----------------------------------------------------------------------------------
BOOL XPhonebook::GetItemPos(DWORD nItem, DWORD nSubItem, DWORD *nTopItem) {
	if (!nTopItem) return false;

	DWORD nCount = 0;

	for (DWORD i = 0; i < this->GetItemCount(); i++) {
		XPhonebookItem *pi = this->Get(i, true);
		if (pi) {
			nCount++;
			if (i == nItem) {
				if (nSubItem == NOT_SELECTED || nSubItem < pi->GetSubItemsCount()) {
					*nTopItem = nCount + (nSubItem == NOT_SELECTED ? 0 : nSubItem + 1) - 1;
					return true;
				}
				return false;
			}
			if (pi->GetExpanded()) {
				nCount += pi->GetSubItemsCount();
			}
		}
	}
	return false;
}
//----------------------------------------------------------------------------------
void XPhonebook::InputAddChar(WCHAR c) {
	size_t len = this->m_inputItemText ? wcslen(this->m_inputItemText) : 0;
	WCHAR *newText = new WCHAR[len + 2];
	if (this->m_inputItemText) wcscpy(newText, this->m_inputItemText);
	if (this->m_inputItemText) free(this->m_inputItemText);
	newText[len] = c;
	newText[len + 1] = '\0';
	this->m_inputItemText = newText;
	
	this->m_actInputChar = c;
}
//----------------------------------------------------------------------------------
void XPhonebook::InputChangeLastChar(WCHAR c) {
	if (!this->m_inputItemText || !*this->m_inputItemText) return;
	size_t len = wcslen(this->m_inputItemText);
	this->m_inputItemText[len - 1] = c;

	this->m_actInputChar = c;
}
//----------------------------------------------------------------------------------
void XPhonebook::InputRemoveLastChar() {
	if (!this->m_inputItemText || !*this->m_inputItemText) return;
	size_t len = wcslen(this->m_inputItemText);
	this->m_inputItemText[len - 1] = '\0';
}
//----------------------------------------------------------------------------------
void XPhonebook::InputDrawChar() {
	if (!m_fntInputChar) return;

	if (this->m_actInputChar) {
		int iStr, iLetter;

		if (this->InputGetCharData(m_actInputChar, &iStr, &iLetter)) {
			int ox = 5, oy = 3;
			COLORREF textBgColor = RGB(0, 0, 0);
			COLORREF textFgColor = RGB(255, 255, 255);

			HBRUSH hBrOutline = CreateSolidBrush(RGB(132, 132, 132));
			HBRUSH hBrBg = CreateSolidBrush(RGB(255, 255, 255));
			
			// select font
			m_fntInputChar->SelectInto(m_hStage);

			// compute rectangle size
			SIZE sz;
			GetTextExtentPoint32(m_hStage, m_keyboardMap[iStr], (int)wcslen(m_keyboardMap[iStr]), &sz);

			// draw outline rect
			RECT rc = {ox, oy, ox + sz.cx + 5, oy + sz.cy + 5};
			FillRect(m_hStage, &rc, hBrOutline);
			// draw background rect
			SetRect(&rc, rc.left + 1, rc.top + 1, rc.right - 1, rc.bottom - 1);
			FillRect(m_hStage, &rc, hBrBg);

			rc.left += 2;
			// draw background text
			SetBkMode(m_hStage, TRANSPARENT);
			DrawText(m_hStage, m_keyboardMap[iStr], -1, &rc, DT_LEFT | DT_SINGLELINE | DT_VCENTER | DT_NOPREFIX);
			// draw selected char
			SetBkMode(m_hStage, OPAQUE);
			SetBkColor(m_hStage, textBgColor);
			GetTextExtentPoint32(m_hStage, m_keyboardMap[iStr], iLetter, &sz);
			WCHAR c[2] = {m_actInputChar, '\0'};
			rc.left += sz.cx;
			SetTextColor(m_hStage, textFgColor);
			DrawText(m_hStage, c, -1, &rc, DT_LEFT | DT_SINGLELINE | DT_VCENTER | DT_NOPREFIX);

			// restore font
			m_fntInputChar->SelectOutOf();

			// clean out memory
			DeleteObject(hBrOutline);
			DeleteObject(hBrBg);
		}
	}
}
//----------------------------------------------------------------------------------
BOOL XPhonebook::InputGetCharData(WCHAR c, int *iStr, int *iLetter) {
	if (!iStr || !iLetter) return false;

	for (int i = 0; i < 10; i++) {
		WCHAR *p;
		if (p = wcschr(m_keyboardMap[i], c)) {
			*iStr = i;
			*iLetter = (int)(p - m_keyboardMap[i]);
			return true;
		}
	}
	return false;
}
//----------------------------------------------------------------------------------
WCHAR XPhonebook::InputGetChar(WCHAR pressedChar, BOOL *replaceActual) {
	if (this->m_input == XPhonebook::INPUT::NUMBERS) {
		*replaceActual = false;
		if (pressedChar >= '0' && pressedChar <= '9') return pressedChar;
		else {
			return pressedChar;
		}
	}else if (this->m_input == XPhonebook::INPUT::LETTERS) {
		int iStr, iLetter;
		if (this->InputGetCharData(pressedChar, &iStr, &iLetter)) {
			if (!this->m_actInputChar) {
				*replaceActual = false;
				return m_keyboardMap[iStr][0];
			}else {
				int iStrAct, iLetterAct;
				if (this->InputGetCharData(this->m_actInputChar, &iStrAct, &iLetterAct)) {
					if (iStr != iStrAct) {
						*replaceActual = false;
						return m_keyboardMap[iStr][0];
					}else {
						*replaceActual = true;
						return m_keyboardMap[iStr][iLetterAct + 1] == '\0' ? m_keyboardMap[iStr][0] : m_keyboardMap[iStr][iLetterAct + 1];
					}
				}
			}
		}
	}else if (this->m_input == XPhonebook::INPUT::RAW) {
		*replaceActual = false;
		return pressedChar;
	}
	return '\0';
}
//----------------------------------------------------------------------------------
void XPhonebook::ApplyItemFilter() {
	WCHAR *text = this->GetInputItemText();
	size_t textLen = wcslen(text);
	BOOL t9 = this->m_input == XPhonebook::INPUT::NUMBERS;

	this->m_items.RemoveAll();
	for (DWORD i = 0; i < this->m_data.num; i++) {
		if (*this->GetInputItemText()) {
			WCHAR *seps = L" ,-_()<>{}[]:/";
			WCHAR *start = this->m_data[i]->GetText();
			WCHAR *p = start;
			do {
				WCHAR *pp;
				if ((pp = wcschr(seps, *p)) || p == start) {
					pp = pp ? p + 1 : p;
					BOOL matches = true;
					if (t9) {
						for (size_t n = 0; n < textLen; n++) {
							if (!this->T9CharMatches(text[n], *(pp + n))) {
								matches = false;
								break;
							}
						}
					}else {
						matches = !_wcsnicmp(pp, text, textLen);
					}
					
					if (matches) {
						this->m_items.Add(this->m_data[i]);
						this->m_data[i]->SetSelection(MAKELONG(pp - start, textLen));
						break;
					}
				}
			}while (*(++p));
		}else {
			this->m_items.Add(this->m_data[i]);
			this->m_data[i]->SetSelection(MAKELONG(0, 0));
		}
	}
}
//----------------------------------------------------------------------------------
BOOL XPhonebook::T9CharMatches(WCHAR a, WCHAR b) {
	int iStrA, iLetterA, iStrB, iLetterB;
	CharLower((WCHAR *)&a);
	if (InputGetCharData(a, &iStrA, &iLetterA)) {
		CharLower((WCHAR *)&b);
		if (InputGetCharData(b, &iStrB, &iLetterB)) {
			return iStrA == iStrB;
		}
	}
	return false;
}
//----------------------------------------------------------------------------------
DWORD XPhonebook::Exists(WCHAR *itemText, BOOL inFiltered) {
	CArray<XPhonebookItem *> *items = inFiltered ? &this->m_items : &this->m_data;
	
	for (DWORD i = 0; i < items->num; i++) {
		if ((*items)[i] && !_wcsicmp((*items)[i]->GetText(), itemText)) {
			return i;
		}
	}
	return 0xFFFFFFFF;
}
/***********************************************************************************
*                            END CLASS XPhonebook                                  *
***********************************************************************************/
