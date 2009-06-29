#include "stdafx.h"
#include "CFont.h"

/***********************************************************************************
*                                  CLASS CFont                                     *
***********************************************************************************/
//----------------------------------------------------------------------------------
HFONT CFont::CreateFont(int height, LPCWSTR faceName, BOOL bold, BOOL italic, BOOL underline, BYTE charset, int angle) {
	HDC hScreen = CreateCompatibleDC(0);
	height = -height * GetDeviceCaps(hScreen, LOGPIXELSY) / 72;
	DeleteDC(hScreen);

	LOGFONTW fn;
	memset(&fn, 0, sizeof(fn));
	fn.lfHeight = height;
	fn.lfWidth = 0;
	fn.lfEscapement = angle;
	fn.lfOrientation = 0;
	fn.lfWeight = bold ? FW_BOLD : FW_NORMAL;
	fn.lfItalic = italic;
	fn.lfUnderline = underline;
	fn.lfStrikeOut = false;
	fn.lfCharSet = charset;
	fn.lfOutPrecision = OUT_DEFAULT_PRECIS;
	fn.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	fn.lfQuality = DEFAULT_QUALITY;
	fn.lfPitchAndFamily = DEFAULT_PITCH;
	wcscpy(fn.lfFaceName, faceName);
	HFONT fnt = CreateFontIndirectW(&fn);

	if (fnt) {
		HDC selInto = this->m_selectedInHdc;
		this->DeleteFont();
		this->m_fnt = fnt;
		this->SelectInto(selInto);
	}
	return this->m_fnt;
}
//----------------------------------------------------------------------------------
HFONT CFont::SelectInto(HDC hdc) {
	if (hdc && this->m_fnt) {
		this->m_oldFnt = (HFONT)SelectObject(hdc, this->m_fnt);
		this->m_selectedInHdc = hdc;
	}
	return this->m_oldFnt;
}
//----------------------------------------------------------------------------------
void CFont::SelectOutOf() {
	if (this->m_selectedInHdc) {
		SelectObject(this->m_selectedInHdc, this->m_oldFnt);
		this->m_oldFnt = NULL;
		this->m_selectedInHdc = NULL;
	}
}
//----------------------------------------------------------------------------------
void CFont::DeleteFont() {
	if (this->m_fnt) {
		this->SelectOutOf();
		DeleteObject(this->m_fnt);
		this->m_fnt = NULL;
	}
}
//----------------------------------------------------------------------------------
HFONT CFont::GetFont() {
	return this->m_fnt;
}
/***********************************************************************************
*                                END CLASS CFont                                   *
***********************************************************************************/
