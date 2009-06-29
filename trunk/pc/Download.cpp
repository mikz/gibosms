#include "stdafx.h"
#include "Download.h"
#include <stdlib.h>
#include <Winsock2.h>
#include "Base64Coder.h"

//#include <atlenc.h>

/***********************************************************************************
* Downloads required file from internet and returns error number                   *
***********************************************************************************/
int DownloadToMemory(char *lpFrom, char *proxyAddress, char **lplpData, DWORD *lpSize, BOOL postMethod, BOOL (* callback)(WPARAM wParam, LPARAM lParam)) {
  if (!lpFrom || !lplpData) return DTME_INVALIDPARAMETER;
  *lplpData = NULL;
	// call callback procedure and check whether it should continue
	if (callback) if (!callback(DTMS_BEGINNING, 0)) {return DTME_ACTIONCANCELED;}
	static BOOL wsaInitialized = false;
	WSAData wsadata;
	// initialize WSA
	if (wsaInitialized || (WSAStartup(MAKEWORD(1, 1), &wsadata) == 0)) {
		wsaInitialized = true;
	}else return DTME_CANTINITIALIZEWSA;
	// call callback procedure and check whether it should continue
	if (callback) if (!callback(DTMS_WSAINITIALIZED, 0)) {return DTME_ACTIONCANCELED;}

	// create socket
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (!sock) return DTME_CANTCREATESOCKET;
	// call callback procedure and check whether it should continue
	if (callback) if (!callback(DTMS_SOCKETCREATED, 0)) {return DTME_ACTIONCANCELED;}

	// size of required file
	DWORD fileSize = 0;
	if (!lpSize) lpSize = &fileSize;
	DWORD errNum = DTME_NO;
	char *data = NULL;
	SOCKADDR_IN SockAddrIn;
	memset(&SockAddrIn, 0, sizeof(SockAddrIn));
	SockAddrIn.sin_family      = AF_INET;
	SockAddrIn.sin_port        = htons(80); // default HTTP port is 80
	SockAddrIn.sin_addr.s_addr = 0;//inet_addr("192.168.000.001");

	if (!_strnicmp(lpFrom, "http://", 7)) lpFrom += 7;
	lpFrom = _strdup(lpFrom);
	char *pSlash = strchr(lpFrom, '/');
	if (pSlash) *pSlash = '\0';
	char *hdrs = NULL;
	// use proxy if should
	hostent *host = NULL;
	char *authentication = NULL;
	if (proxyAddress && *proxyAddress) {
		char *proxy = _strdup(proxyAddress);
		char *p = strchr(proxy, '@');
		if (p && p - proxy > 0) {
			*p = '\0';
			CBase64Coder coder;
			int lenEnc = coder.Base64EncodeGetRequiredLength((int)strlen(proxy), ATL_BASE64_FLAG_NOCRLF);
			authentication = new char[lenEnc + 1];
			coder.Base64Encode((BYTE *)proxy, (int)strlen(proxy), authentication, &lenEnc, ATL_BASE64_FLAG_NOCRLF);
			authentication[lenEnc] = '\0';
		}
		char *server = p ? p + 1 : proxy;
		if (p = strchr(server, ':')) *p = '\0';
		// get ip adrress of server
		host = gethostbyname(server);
		// set proxy port
		SockAddrIn.sin_port = htons(p ? atoi(p + 1) : 80);
		free(proxy);
	}else {
		// get ip adrress of server
		host = gethostbyname(lpFrom);
	}
	if (!host) {errNum = DTME_SERVERNOTFOUND; goto dm_cleanup;}
	// call callback procedure and check whether it should continue
	if (callback) if (!callback(DTMS_SERVERFOUND, 0)) {errNum = DTME_ACTIONCANCELED; goto dm_cleanup;}
	// connect to server
	memcpy(&SockAddrIn.sin_addr.s_addr, *(host->h_addr_list), 4);
	if (connect(sock, (sockaddr *)&SockAddrIn, sizeof(SockAddrIn)) == SOCKET_ERROR ) {
		errNum = DTME_CANTCONNECT;
		goto dm_cleanup;
	}
	// call callback procedure and check whether it should continue
	if (callback) if (!callback(DTMS_CONNECTED, 0)) {errNum = DTME_ACTIONCANCELED; goto dm_cleanup;}
	
	hdrs = new char[1000 + strlen(lpFrom)];
	size_t requestSize = 0;
	char *pQuestMark = NULL;
	if (postMethod && pSlash) {
		pQuestMark = strchr(pSlash + 1, '?');
		if (pQuestMark) {
			requestSize = strlen(pQuestMark) - 1;
			*pQuestMark = '\0';
		}
	}
	if (proxyAddress && *proxyAddress) {
		if (postMethod) wsprintfA(hdrs, "POST http://%s/%s HTTP/1.0%s%s\r\nContent-Type: application/x-www-form-urlencoded\r\nContent-Length: %i\r\nHost: %s\r\n\r\n", lpFrom, pSlash ? pSlash + 1 : "", authentication ? "\r\nProxy-Authorization: Basic " : "", authentication ? authentication : "", requestSize, lpFrom);
		else wsprintfA(hdrs, "GET http://%s/%s HTTP/1.0%s%s\r\nAccept: */*\r\nUser-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1)\r\nHost: %s\r\n\r\n", lpFrom, pSlash ? pSlash + 1 : "", authentication ? "\r\nProxy-Authorization: Basic " : "", authentication ? authentication : "", lpFrom);
	}else {
		if (postMethod) wsprintfA(hdrs, "POST /%s HTTP/1.0\r\nContent-Type: application/x-www-form-urlencoded\r\nContent-Length: %i\r\nHost: %s\r\n\r\n", pSlash ? pSlash + 1 : "", requestSize, lpFrom);
		else wsprintfA(hdrs, "GET /%s HTTP/1.0\r\nAccept: */*\r\nUser-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1)\r\nHost: %s\r\n\r\n", pSlash ? pSlash + 1 : "",  lpFrom);
	}
	// send headers of request
	if (send(sock, hdrs, (int)strlen(hdrs), 0) <= 0) {errNum = DTME_CANTSENDHEADERS; goto dm_cleanup;}
	// call callback procedure and check whether it should continue
	if (callback) if (!callback(DTMS_HEADERSSENT, 0)) {errNum = DTME_ACTIONCANCELED; goto dm_cleanup;}
	size_t bytesSent = 0;
	char *p = pQuestMark + 1;
	size_t chunkSize = 1024;
	while (postMethod && bytesSent < requestSize) {
		int bs = send(sock, p, (int)min(chunkSize, requestSize - bytesSent), 0);
		if (bs == SOCKET_ERROR) {
			errNum = DTME_CANTSENDPARAMETERS;
			goto dm_cleanup;
		}
		p += bs;
		bytesSent += bs;
		// call callback procedure and check whether it should continue
		if (callback) if (!callback(DTMS_PARTOFPARAMETERSSENT, 0)) {errNum = DTME_ACTIONCANCELED; goto dm_cleanup;}
	}
	// call callback procedure and check whether it should continue
	if (callback) if (!callback(DTMS_PARAMETERSSENT, 0)) {errNum = DTME_ACTIONCANCELED; goto dm_cleanup;}
	
	// set timeout and receive data from server
	int to = 60*1000; // 60 seconds
	setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char *)&to, sizeof(int));

	DWORD readBytes;
	DWORD allocated = 0;
	*lpSize = 0;
	// read file step by step (step = n bytes)
	int step = 4*1024; // 4 kilobytes
	do {
		if (*lpSize + step >= allocated) data = (char *)realloc(data, allocated = (*lpSize + step + 1));
		readBytes = recv(sock, data + *lpSize, step * (sizeof(char)), 0);
		if(readBytes != SOCKET_ERROR) {
			*lpSize += readBytes;
		}else {
			errNum = DTME_RECEIVEDATA;
			free(data);
			goto dm_cleanup;
		}
		// call callback procedure and check whether it should continue
		if (callback) if (!callback(DTMS_PARTOFDATARECEIVED, 0)) {errNum = DTME_ACTIONCANCELED; free(data); goto dm_cleanup;}
	}while(readBytes);
	// call callback procedure and check whether it should continue
	if (callback) if (!callback(DTMS_DATARECEIVED, 0)) {errNum = DTME_ACTIONCANCELED; free(data); goto dm_cleanup;}

	if (data) {
		data[*lpSize] = '\0';
		// move data to the beginning (cut the headers)
		DWORD i;
		for (i = 0; i < *lpSize; i++) {
			if (data[i] != '\r') continue;
			if (i <= *lpSize - 4 && data[i + 1] == '\n' && data[i + 2] == '\r' && data[i + 3] == '\n') {
				i += 4;
				break;
			}
			if (i <= *lpSize - 2 && data[i + 1] == '\r') {	
				i += 2;
				break;
			}
		}
		if (i) memmove(data, data + i, *lpSize - i + 1);
	}
	*lplpData = data;

dm_cleanup:
	if (sock) closesocket(sock);
	free(lpFrom);
	delete[] hdrs;
	delete[] authentication;

	// call callback procedure and check whether it should continue
	if (callback) if (!callback(DTMS_DOWNLOADINGCOMPLETE, 0)) {return DTME_ACTIONCANCELED;}
	return errNum;
}
