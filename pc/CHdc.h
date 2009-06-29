#ifndef __CHDC_H__
#define __CHDC_H__

void BitBltTransparent(HDC hdcDest, int nXDest, int nYDest, int nWidth, int nHeight, HDC hdcSrc, int nXSrc, int nYSrc, COLORREF clrTrans);
HRGN CreateRegionFromBitmap(HBITMAP hBitmap, COLORREF transparentColor);

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
	BOOL LoadBitmap(LPTSTR szFileName, BOOL deletePrevious = true);
	BOOL LoadBitmap(HBITMAP hBmp, BOOL deletePrevious = true);
	HBITMAP UnloadBitmap();
	DWORD GetWidth();
	DWORD GetHeight();
	operator HDC() {return hdc;}

	CHdc(BOOL bMono)
	{
		Create(bMono);
	}
	CHdc(BOOL bMono, DWORD nWidth, DWORD nHeight)
	{
		Create(bMono);
		SetSize(nWidth, nHeight);
	}
	~CHdc()
	{
		Destroy();
	}
};

#endif //__CHDC_H__