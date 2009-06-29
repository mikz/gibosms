#ifndef __CFONT_H__
#define __CFONT_H__

#include <windows.h>

class CFont {
private:
	HFONT m_fnt;
	HFONT m_oldFnt;
	HDC m_selectedInHdc;
	
public:
	HFONT CreateFont(HDC hdc, int height, LPCWSTR faceName, BOOL bold = false, BOOL italic = false, BOOL underline = false, BYTE charset = DEFAULT_CHARSET, int angle = 0);
	HFONT CreateFont(HWND hWnd, int height, LPCWSTR faceName, BOOL bold = false, BOOL italic = false, BOOL underline = false, BYTE charset = DEFAULT_CHARSET, int angle = 0);
	HFONT SelectInto(HDC hdc);
	void SelectOutOf();
	void DeleteFont();
	HFONT GetFont();

	CFont () {
		m_fnt = NULL;
		m_oldFnt = NULL;
		m_selectedInHdc = NULL;
	}
	~CFont () {
		DeleteFont();
	}
};

#endif //__CFONT_H__