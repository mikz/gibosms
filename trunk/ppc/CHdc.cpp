#include "CHdc.h"

/***********************************************************************************
* The TransparentImage function transfers a bitmap, omitting the portions drawn    *
* in a specified color                                                             *
***********************************************************************************/
void BitBltTransparent(HDC hdcDest, int nXDest, int nYDest, int nWidth, int nHeight, HDC hdcSrc, int nXSrc, int nYSrc, COLORREF clrTrans) {
	HDC hdcMask = CreateCompatibleDC(0);
	HBITMAP bmpMask = CreateCompatibleBitmap(hdcMask, nWidth, nHeight);
	HBITMAP bmpMaskOld = (HBITMAP)SelectObject(hdcMask, bmpMask);
	
	HDC hdcSprite = CreateCompatibleDC(hdcSrc);
	HBITMAP bmpSprite = CreateCompatibleBitmap(hdcSrc, nWidth, nHeight);
	HBITMAP bmpSpriteOld = (HBITMAP)SelectObject(hdcSprite, bmpSprite);
	BitBlt(hdcSprite, 0, 0, nWidth, nHeight, hdcSrc, nXSrc, nYSrc, SRCCOPY);

	COLORREF clrOld = SetBkColor(hdcSprite, clrTrans);
	BitBlt(hdcMask, 0, 0, nWidth, nHeight, hdcSprite, nXSrc, nYSrc, SRCCOPY);
	SetBkColor(hdcSprite, clrOld);
	BitBlt(hdcSprite, 0, 0, nWidth, nHeight, hdcMask, 0, 0, SRCPAINT);
	BitBlt(hdcMask, 0, 0, nWidth, nHeight, NULL, 0, 0, DSTINVERT);

	BitBlt(hdcDest, nXDest, nYDest, nWidth, nHeight, hdcMask, 0, 0, SRCPAINT);
	BitBlt(hdcDest, nXDest, nYDest, nWidth, nHeight, hdcSprite, 0, 0, SRCAND);

	SelectObject(hdcMask, bmpMaskOld);
	DeleteObject(bmpMask);
	DeleteDC(hdcMask);

	SelectObject(hdcSprite, bmpSpriteOld);
	DeleteObject(bmpSprite);
	DeleteDC(hdcSprite);
}

/***********************************************************************************
*                                    CLASS CHdc                                    *
***********************************************************************************/
//----------------------------------------------------------------------------------
BOOL CHdc::Create(BOOL bMono) {
	HDC hdcScreen = GetDC(0);
	if (hdcScreen) {
		this->hdc = CreateCompatibleDC(hdcScreen);
		this->bmp = NULL;
		this->bmpOld = NULL;
		ReleaseDC(0, hdcScreen);
		this->wid = (this->hdc) ? 1 : 0;
		this->hei = (this->hdc) ? 1 : 0;
		this->mono = bMono;
		return this->hdc != NULL;
	}
	return false;
}
//----------------------------------------------------------------------------------
void CHdc::Destroy() {
	if (this->hdc) {
		if (this->bmpOld) SelectObject(this->hdc, this->bmpOld);
		DeleteDC(this->hdc);
	}
	if (this->bmp) DeleteObject(this->bmp);
}
//----------------------------------------------------------------------------------
BOOL CHdc::SetSize(DWORD nWidth, DWORD nHeight) {
	if (this->hdc) {
		if (nWidth <= this->wid && nHeight <= this->hei) {
			this->wid = nWidth;
			this->hei = nHeight;
			return true;
		}
		if (nWidth == 0 || nHeight == 0) return false;
		if (this->bmpOld) SelectObject(this->hdc, this->bmpOld);
		if (this->bmp) DeleteObject(this->bmp);
		HDC hdcScreen = GetDC(0);
		if (hdcScreen) {
			this->bmp = CreateCompatibleBitmap(this->mono ? this->hdc : hdcScreen, nWidth, nHeight);
			if (this->bmp) {
				this->bmpOld = (HBITMAP)SelectObject(this->hdc, this->bmp);
				this->wid = nWidth;
				this->hei = nHeight;
			}
			ReleaseDC(0, hdcScreen);
			return this->bmp != NULL;
		}
	}
	return false;
}
//----------------------------------------------------------------------------------
BOOL CHdc::LoadBitmap(WCHAR *szFileName) {
	if (this->hdc) {
		if (this->bmpOld) SelectObject(this->hdc, this->bmpOld);
		if (this->bmp) DeleteObject(this->bmp);
		this->bmp = LoadImage(NULL, szFileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		if (this->bmp) {
			this->bmpOld = (HBITMAP)SelectObject(this->hdc, this->bmp);
			BITMAP bm;
			GetObject(this->bmp, sizeof(BITMAP), &bm);
			this->wid = bm.bmWidth;
			this->hei = bm.bmHeight;
		}
		return this->bmp != NULL;
	}
	return false;
}
//----------------------------------------------------------------------------------
BOOL CHdc::LoadBitmap(HBITMAP hBmp) {
	if (this->hdc) {
		if (this->bmpOld) SelectObject(this->hdc, this->bmpOld);
		if (this->bmp) DeleteObject(this->bmp);
		this->bmp = hBmp;
		if (this->bmp) {
			this->bmpOld = (HBITMAP)SelectObject(this->hdc, this->bmp);
			BITMAP bm;
			GetObject(this->bmp, sizeof(BITMAP), &bm);
			this->wid = bm.bmWidth;
			this->hei = bm.bmHeight;
		}
		return this->bmp != NULL;
	}
	return false;
}
//----------------------------------------------------------------------------------
DWORD CHdc::GetWidth() {
	return this->wid;
}
//----------------------------------------------------------------------------------
DWORD CHdc::GetHeight() {
	return this->hei;
}
/***********************************************************************************
*                                  END CLASS CHdc                                  *
***********************************************************************************/
