#ifndef __CHDC_H__
#define __CHDC_H__

#include <windows.h>

#ifndef LR_LOADFROMFILE
#define LR_LOADFROMFILE     0x0010
#endif


void BitBltTransparent(HDC hdcDest, int nXDest, int nYDest, int nWidth, int nHeight, HDC hdcSrc, int nXSrc, int nYSrc, COLORREF clrTrans);

class CHdc {
private:
	HDC hdc;        // HDC
	HGDIOBJ bmp;    // bitmap
	HGDIOBJ bmpOld; // old bitmap
	BOOL mono;
	DWORD wid;
	DWORD hei;

private:
	BOOL Create(BOOL bMono);
	void Destroy();
public:
	BOOL SetSize(DWORD nWidth, DWORD nHeight);
	BOOL LoadBitmap(WCHAR *szFileName);
	BOOL LoadBitmap(HBITMAP hBmp);
	DWORD GetWidth();
	DWORD GetHeight();
	operator HDC() {return hdc;}

	CHdc(BOOL bMono) {
		Create(bMono);
	}
	CHdc(BOOL bMono, DWORD nWidth, DWORD nHeight) {
		Create(bMono);
		SetSize(nWidth, nHeight);
	}
	~CHdc() {
		Destroy();
	}
};

#endif //__CHDC_H__