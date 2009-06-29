#ifndef __CSVWALKER_H__
#define __CSVWALKER_H__

#include "CArray.h"
#include "SetCharPtr.h"


class CCSVWalker {
private:
	char *m_delimiter;
	char m_quote;
	CArray<char *> m_columns;
	CArray<CArray<char *> *> m_data;
	DWORD m_actRow;
	
	static LPSTR* explodeqEx(char *str, char *d, unsigned long* count, char quote);
	static LPSTR* ParseLines(char *str, char *d, unsigned long* count, char quote);
	static char *CorrectEnters(char *text);

public:
	void SetDelimiter(char *delimiter);
	char *GetDelimiter();
	void SetQuoteChar(char quote);
	void Free();
	BOOL Load(char *szFileName);
	BOOL Write(char *szFileName);
	BOOL ParseText(char *text);
	
	char *GetField(char *colName, char *defaultValue = NULL);
	char *GetField(char *colName, DWORD row, char *defaultValue = NULL);
	char *GetField(DWORD col, char *defaultValue = NULL);
	char *GetField(DWORD col, DWORD row, char *defaultValue = NULL);
	
	BOOL SetField(char *colName, char *value);
	BOOL SetField(char *colName, DWORD row, char *value);
	BOOL SetField(DWORD col, char *value);
	BOOL SetField(DWORD col, DWORD row, char *value);
	
	long GetFieldInt(char *colName, long defaultValue = 0);
	long GetFieldInt(char *colName, DWORD row, long defaultValue = 0);
	long GetFieldInt(DWORD col, long defaultValue = 0);
	long GetFieldInt(DWORD col, DWORD row, long defaultValue = 0);
	
	BOOL SetFieldInt(char *colName, long value);
	BOOL SetFieldInt(char *colName, DWORD row, long value);
	BOOL SetFieldInt(DWORD col, long value);
	BOOL SetFieldInt(DWORD col, DWORD row, long value);
	
	void AddColumn(char *colName);
	BOOL RemoveColumn(char *colName);
	BOOL RemoveColumn(DWORD col);
	void AddRow();
	BOOL RemoveRow(DWORD row);
	DWORD GetColByName(char *colName);
	BOOL NextRow();
	void Reset();
	DWORD GetNumRows();

	CCSVWalker() {
		m_delimiter = NULL;
		m_quote = '"';
		m_actRow = 0;
	}
	~CCSVWalker() {
		SetDelimiter(NULL);
		Free();
	}
};

#endif //__CSVWALKER_H__