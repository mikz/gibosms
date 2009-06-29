#include "stdafx.h"
#include "CHdc.h"

/***********************************************************************************
* The TransparentImage function transfers a bitmap, omitting the portions drawn    *
* in a specified color                                                             *
***********************************************************************************/
void BitBltTransparent(HDC hdcDest, int nXDest, int nYDest, int nWidth, int nHeight, HDC hdcSrc, int nXSrc, int nYSrc, COLORREF clrTrans)
{
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
* Creates region from bitmap according transparent color                           *
***********************************************************************************/
HRGN CreateRegionFromBitmap(HBITMAP hBitmap, COLORREF transparentColor)
{
	HRGN hRgn = NULL;
	HRGN hRgn1 = NULL;

	// Check for valid bitmap handle
	if ( hBitmap != NULL ) {
		// Get bitmap object information
		BITMAP bitmap;
		GetObject(hBitmap, sizeof(BITMAP), &bitmap);
		DWORD dwSize = bitmap.bmHeight * bitmap.bmWidthBytes;
		int bitsPixel = bitmap.bmBitsPixel / 8;

		// Check bitmap color depth (only 24 or 32 bits per pixel allowed)
		if (( bitsPixel == 3 ) || ( bitsPixel == 4 ) ) {
			// Get bitmap bits
			unsigned char* pBits = new unsigned char[dwSize];
			GetBitmapBits(hBitmap, dwSize, pBits);

			// Create region from bitmap
			unsigned char red, green, blue;
			for (int y = 0; y < bitmap.bmHeight; y++ ) {
				for (int x = 0; x < bitmap.bmWidth; x++ ) {
					// Get pixel color
					blue = pBits[y*bitmap.bmWidthBytes + bitsPixel*x];
					green = pBits[y*bitmap.bmWidthBytes + bitsPixel*x+1];
					red = pBits[y*bitmap.bmWidthBytes + bitsPixel*x+2];
					//COLORREF = pBits[y*bitmap.bmWidthBytes + bitsPixel*x];
				   
					// Check transparent color
					if (RGB(red, green, blue) != transparentColor ) {
						// Combine regions
						if ( hRgn == NULL ) {
							hRgn = CreateRectRgn(x, y, x+1, y+1);
						}else {
							// Delete temporary region
							if (hRgn1 != NULL)
								DeleteObject(hRgn1);

							// Create temporary region
							hRgn1 = CreateRectRgn(x, y, x+1, y+1);

							// Combine regions
							CombineRgn(hRgn, hRgn, hRgn1, RGN_OR);
						}
					}
				}
			}

			// Free bitmap bits
			delete pBits;
		}
	}

	// Delete temporary region
	if ( hRgn1 != NULL )
		DeleteObject(hRgn1);

	return hRgn;
}

/***********************************************************************************
*                                    CLASS CHdc                                    *
***********************************************************************************/
//----------------------------------------------------------------------------------
BOOL CHdc::Create(BOOL bMono)
{
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
void CHdc::Destroy()
{
	if (this->hdc) {
		if (this->bmpOld) SelectObject(this->hdc, this->bmpOld);
		DeleteDC(this->hdc);
	}
	if (this->bmp) DeleteObject(this->bmp);
}
//----------------------------------------------------------------------------------
BOOL CHdc::SetSize(DWORD nWidth, DWORD nHeight)
{
	if (this->hdc) {
		//if (nWidth <= this->wid && nHeight <= this->hei) return true;
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
BOOL CHdc::LoadBitmap(LPTSTR szFileName, BOOL deletePrevious)
{
	HBITMAP bmp = (HBITMAP)LoadImage(NULL, szFileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE|LR_CREATEDIBSECTION);
	return this->LoadBitmap(bmp);
}
//----------------------------------------------------------------------------------
BOOL CHdc::LoadBitmap(HBITMAP hBmp, BOOL deletePrevious)
{
	if (this->hdc) {
		if (this->bmpOld) SelectObject(this->hdc, this->bmpOld);
		if (this->bmp && deletePrevious) DeleteObject(this->bmp);
		this->bmp = hBmp;
		if (this->bmp) {
			this->bmpOld = (HBITMAP)SelectObject(this->hdc, this->bmp);
			if (deletePrevious) {
				this->bmpOld = NULL;
			}
			BITMAP bm;
			GetObject(this->bmp, sizeof(BITMAP), &bm);
			this->wid = bm.bmWidth;
			this->hei = bm.bmHeight;
		}else {
			this->bmpOld = NULL;
		}
		return this->bmp != NULL;
	}
	return false;
}
//----------------------------------------------------------------------------------
HBITMAP CHdc::UnloadBitmap()
{
	if (this->hdc) {
		if (this->bmpOld) SelectObject(this->hdc, this->bmpOld);
		HBITMAP bmp = (HBITMAP)this->bmp;
		this->bmp = NULL;
		this->bmpOld = NULL;
		return bmp;
	}
	return NULL;
}
//----------------------------------------------------------------------------------
DWORD CHdc::GetWidth()
{
	return this->wid;
}
//----------------------------------------------------------------------------------
DWORD CHdc::GetHeight()
{
	return this->hei;
}
/***********************************************************************************
*                                  END CLASS CHdc                                  *
***********************************************************************************/
