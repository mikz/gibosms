// Base64Coder.h: interface for the CBase64Coder class.
//
//////////////////////////////////////////////////////////////////////

#ifndef AFX_BASE64CODER_H__12531EDB_6A7C_40B6_A918_BFCC3B9425D0__INCLUDED_
#define AFX_BASE64CODER_H__12531EDB_6A7C_40B6_A918_BFCC3B9425D0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define ATL_BASE64_FLAG_NONE 0
#define ATL_BASE64_FLAG_NOPAD 1
#define ATL_BASE64_FLAG_NOCRLF 2

class CBase64Coder
{
public:
   CBase64Coder();
   virtual ~CBase64Coder();
   int Base64EncodeGetRequiredLength(int nSrcLen, DWORD
                                     dwFlags=ATL_BASE64_FLAG_NONE);
   int Base64DecodeGetRequiredLength(int nSrcLen);
   BOOL Base64Encode(const BYTE *pbSrcData,
                          int nSrcLen,
                          LPSTR szDest,
                          int *pnDestLen,
                          DWORD dwFlags=ATL_BASE64_FLAG_NONE);

   BOOL Base64Decode(LPCSTR szSrc,
                     int nSrcLen,
                     BYTE *pbDest,
                     int *pnDestLen);
   int DecodeBase64Char(unsigned int ch);

};

#define ATLASSERT(x) {};

#endif //!defined(AFX_BASE64CODER_H__12531EDB_6A7C_40B6_A918_BFCC3B9425D0__INCLUDED_)