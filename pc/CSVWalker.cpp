#include "stdafx.h"
#include "CSVWalker.h"
#include <stdio.h>

/***********************************************************************************
*                                 CLASS CCSVWalker                                 *
***********************************************************************************/
//-Vrátí pole pointerù na stringy rozdìlených z jednoho stringu podle stringu, s----
//-tim že pøeskoèí uvozovky, ale se zachováním prázdných slov-----------------------
LPSTR* CCSVWalker::explodeqEx(char *str, char *d, unsigned long* count, char quote) {
	if (str && d && count) {
		char *orgStr = str;
		#define POKOLIKA 10 // alokuje to pole po 10
		// alokuje pole pointeru
		unsigned long alokovano=0;
		LPSTR* a=(LPSTR*)malloc( (alokovano+=POKOLIKA) * sizeof(LPSTR) );
		DWORD dLen = (DWORD)strlen(d);
		*count = 0;
		BOOL isInsideWord = false;
		BOOL isInsideQuotes = false;
		BOOL lastCharDivider = false;

		while (*str) {
			if (!memcmp(str, d, dLen)) {
				if (!isInsideQuotes) {
					if (lastCharDivider) {
						a[(*count)] = str;
						(*count)++;
						if(*count == alokovano) a=(LPSTR*)realloc(a, (alokovano += POKOLIKA) * sizeof(LPSTR) );
					}
					isInsideWord = false;
					lastCharDivider = true;
					*str = '\0';
				}
			}else if (/**str == '"' || */*str == quote && (*(str + 1) != quote && !(str != orgStr && *(str - 1) == quote))) {
				if (!isInsideWord && isInsideQuotes) {
					a[(*count)] = str;
					(*count)++;
					if(*count == alokovano) a=(LPSTR*)realloc(a, (alokovano += POKOLIKA) * sizeof(LPSTR) );
				}
				isInsideWord = false;
				isInsideQuotes = !isInsideQuotes;
				lastCharDivider = false;
				*str = '\0';
			}else {
				if (!isInsideWord) {
					a[(*count)] = str;
					(*count)++;
					if(*count == alokovano) a=(LPSTR*)realloc(a, (alokovano += POKOLIKA) * sizeof(LPSTR) );
					isInsideWord = true;
				}
				lastCharDivider = false;
			}
			str++;
		}
		
		char exc[3] = {quote, quote, '\0'};
		for (unsigned long i = 0; i < *count; i++) {
			if (!strcmp(a[i], exc)) a[i][0] = '\0';
		}

		return a;
	}
	return NULL;
}
/***********************************************************************************
* Vrátí pole pointerù na stringy rozdìlených z jednoho stringu podle stringu, s    *
* tim že pøeskoèí uvozovky                                                         *
***********************************************************************************/
LPSTR* CCSVWalker::ParseLines(char *str, char *d, unsigned long* count, char quote) {
	if (str && d && count) {
		char *orgStr = str;
		#define POKOLIKA 10 // alokuje to pole po 10
		// alokuje pole pointeru
		unsigned long alokovano=0;
		LPSTR* a=(LPSTR*)malloc( (alokovano+=POKOLIKA) * sizeof(LPSTR) );
		DWORD dLen = (DWORD)strlen(d);
		*count = 1;
		BOOL isInsideQuotes = false;

		a[0]=str;
		while (*str) {
			if (!memcmp(str, d, dLen)) {
				if (!isInsideQuotes) {
					*str = '\0';
					str += dLen;
					a[(*count)] = str;
					(*count)++;
					if(*count == alokovano) a=(LPSTR*)realloc(a, (alokovano += POKOLIKA) * sizeof(LPSTR) );
				}
			}
			if (*str == quote && *(str + 1) != quote && !(str != orgStr && *(str - 1) == quote)) {
				isInsideQuotes = !isInsideQuotes;
			}
			str++;
		}

		return a;
	}
	return NULL;
}
//-Converts linux ends of lines (\n) to windows ends of lines (\r\n)----------------
char *CCSVWalker::CorrectEnters(char *text) {
	if (text) {
		char *p = text;
		int last = 0;
		char *retVal = NULL;
		
		DWORD allocated = 0;
		DWORD necessarySize = 1; // for '\0'
		int ALLOC_STEP = 100;
		
		while (*p) {
			if (*p == '\n' && (p == text || *(p - 1) != '\r')) {
				necessarySize++;
				if (necessarySize >= allocated) retVal = (char *)realloc(retVal, allocated += ALLOC_STEP);
				retVal[last++] = '\r';
			}else if (*p == '\r' && *(p + 1) != '\n') {
				necessarySize += 2;
				if (necessarySize >= allocated) retVal = (char *)realloc(retVal, allocated += ALLOC_STEP);
				retVal[last++] = *(p++);
				retVal[last++] = '\n';
				continue;
			}
			necessarySize++;
			if (necessarySize >= allocated) retVal = (char *)realloc(retVal, allocated += ALLOC_STEP);
			retVal[last++] = *(p++);
		}
		if (!retVal) retVal = _strdup(text);
		else retVal[last] = '\0';
		return retVal;
	}
	return NULL;
}
//----------------------------------------------------------------------------------
void CCSVWalker::SetDelimiter(char *delimiter) {
	SetCharPtr(&this->m_delimiter, delimiter);
}
//----------------------------------------------------------------------------------
char *CCSVWalker::GetDelimiter() {
	return this->m_delimiter ? this->m_delimiter : ";";
}
//----------------------------------------------------------------------------------
void CCSVWalker::SetQuoteChar(char quote) {
	this->m_quote = quote;
}
//----------------------------------------------------------------------------------
void CCSVWalker::Free() {
	for (DWORD i = 0; i < this->m_columns.num; i++)
		free(this->m_columns[i]);
	this->m_columns.RemoveAll();
	
	for (DWORD m = 0; m < this->m_data.num; m++) {
		CArray<char *> *row = this->m_data[m];
		if (row) {
			for (DWORD n = 0; n < row->num; n++)
				free((*row)[n]);
			row->RemoveAll();
			delete row;
		}
	}
	this->m_data.RemoveAll();
}
//----------------------------------------------------------------------------------
BOOL CCSVWalker::Load(char *szFileName) {
	FILE *fp;;
	if (fp = fopen(szFileName, "rb")) {
		fseek(fp, 0, SEEK_END);
		long fsize = ftell(fp);
		fseek(fp, 0, 0);
		char *text = new char[fsize + 1];
		size_t nread = fread(text, sizeof(char), fsize, fp);
		text[nread] = '\0';
		fclose(fp);
		
		this->ParseText(text);
		
		delete[] text;
	}

	// set actual line
	this->Reset();
	return true;
}
//----------------------------------------------------------------------------------
BOOL CCSVWalker::ParseText(char *text) {
	if (!text) return false;
	
	char *data = this->CorrectEnters(text);
	
	ULONG numLines;
	LPSTR *lines = this->ParseLines(data, "\r\n", &numLines, this->m_quote);
	char *delim = this->GetDelimiter();
	for (ULONG i = 0; i < numLines; i++) {
		if (*lines[i]/* || true*/) {
			ULONG numCols;
			LPSTR *cols = this->explodeqEx(lines[i], delim, &numCols, this->m_quote);
			if (i == 0) {
				// read header
				for (ULONG n = 0; n < numCols; n++)
					this->AddColumn(cols[n]);
			}else {
				this->AddRow();
				for (DWORD n = 0; n < this->m_columns.num; n++) {
					char *p = cols[n]; char c[3] = {this->m_quote, this->m_quote, '\0'};
					while (p && (p = strstr(p, c))) {
						memmove(p + 1, p + 2, strlen(p) - 2 + 1);
						p += 1;
					}
					this->SetField(n, n < numCols ? cols[n] : NULL);
				}
			}
			free(cols);
		}
	}
	free(lines);
	free(data);
	
	return true;
}
//----------------------------------------------------------------------------------
BOOL CCSVWalker::Write(char *szFileName) {
	FILE *fp;
	
	if (fp = fopen(szFileName, "wb")) {
		// write header
		DWORD i;
		for (i = 0; i < this->m_columns.num; i++) {
			if (i != 0) fwrite(this->GetDelimiter(), sizeof(char), strlen(this->GetDelimiter()), fp);
			fputc(this->m_quote, fp);
			if (this->m_columns[i]) {
				for (char *p = this->m_columns[i]; *p; p++) {
					fputc(*p, fp);
					if (*p == this->m_quote) fputc(this->m_quote, fp);
				}
			}
			fputc(this->m_quote, fp);
		}
		if (i) fwrite("\r\n", sizeof(char), 2, fp);
		
		// write data
		for (DWORD m = 0; m < this->m_data.num; m++) {
			if (m != 0) fwrite("\r\n", sizeof(char), 2, fp);

			CArray<char *> *row = this->m_data[m];
			if (row) {
				for (DWORD n = 0; n < this->m_columns.num; n++) {
					if (n != 0) fwrite(this->GetDelimiter(), sizeof(char), strlen(this->GetDelimiter()), fp);
					fputc(this->m_quote, fp);
					if (n < row->num && (*row)[n]) {
						for (char *p = (*row)[n]; *p; p++) {
							fputc(*p, fp);
							if (*p == this->m_quote) fputc(this->m_quote, fp);
						}
					}
					fputc(this->m_quote, fp);
				}
			}
		}
	
		fclose(fp);
		return true;
	}
	return false;
}
//----------------------------------------------------------------------------------
char *CCSVWalker::GetField(char *colName, char *defaultValue) {
	return this->GetField(colName, this->m_actRow, defaultValue);
}
//----------------------------------------------------------------------------------
char *CCSVWalker::GetField(char *colName, DWORD row, char *defaultValue) {
	return this->GetField(this->GetColByName(colName), row, defaultValue);
}
//----------------------------------------------------------------------------------
char *CCSVWalker::GetField(DWORD col, char *defaultValue) {
	return this->GetField(col, this->m_actRow, defaultValue);
}
//----------------------------------------------------------------------------------
char *CCSVWalker::GetField(DWORD col, DWORD row, char *defaultValue) {
	if (row < this->m_data.num && col < this->m_data[row]->num) {
		return (*this->m_data[row])[col];
	}
	return defaultValue;
}
//----------------------------------------------------------------------------------
BOOL CCSVWalker::SetField(char *colName, char *value) {
	return this->SetField(colName, this->m_actRow, value);
}
//----------------------------------------------------------------------------------
BOOL CCSVWalker::SetField(char *colName, DWORD row, char *value) {
	return this->SetField(this->GetColByName(colName), row, value);
}
//----------------------------------------------------------------------------------
BOOL CCSVWalker::SetField(DWORD col, char *value) {
	return this->SetField(col, this->m_actRow, value);
}
//----------------------------------------------------------------------------------
BOOL CCSVWalker::SetField(DWORD col, DWORD row, char *value) {
	if (row < this->m_data.num && col < this->m_data[row]->num) {
		free((*this->m_data[row])[col]);
		(*this->m_data[row])[col] = value ? _strdup(value) : NULL;
		return true;
	}
	return false;
}
//----------------------------------------------------------------------------------
long CCSVWalker::GetFieldInt(char *colName, long defaultValue) {
	return this->GetFieldInt(colName, this->m_actRow, defaultValue);
}
//----------------------------------------------------------------------------------
long CCSVWalker::GetFieldInt(char *colName, DWORD row, long defaultValue) {
	return this->GetFieldInt(this->GetColByName(colName), row, defaultValue);
}
//----------------------------------------------------------------------------------
long CCSVWalker::GetFieldInt(DWORD col, long defaultValue) {
	return this->GetFieldInt(col, this->m_actRow, defaultValue);
}
//----------------------------------------------------------------------------------
long CCSVWalker::GetFieldInt(DWORD col, DWORD row, long defaultValue) {
	char *data = this->GetField(col, row);

	if (!data) return defaultValue;
	return atol(data);
}
//----------------------------------------------------------------------------------
BOOL CCSVWalker::SetFieldInt(char *colName, long value) {
	return this->SetFieldInt(colName, this->m_actRow, value);
}
//----------------------------------------------------------------------------------
BOOL CCSVWalker::SetFieldInt(char *colName, DWORD row, long value) {
	return this->SetFieldInt(this->GetColByName(colName), row, value);
}
//----------------------------------------------------------------------------------
BOOL CCSVWalker::SetFieldInt(DWORD col, long value) {
	return this->SetFieldInt(col, this->m_actRow, value);
}
//----------------------------------------------------------------------------------
BOOL CCSVWalker::SetFieldInt(DWORD col, DWORD row, long value) {
	char b[20];
	_ltoa(value, b, 10);
	return this->SetField(col, row, b);
}
//----------------------------------------------------------------------------------
void CCSVWalker::AddColumn(char *colName) {
	if (!colName || !*colName) return;
	
	for (DWORD i = 0; i < this->m_columns.num; i++) {
		if (!_stricmp(colName, this->m_columns[i]))
			return;
	}
	
	if (this->m_columns.Add(_strdup(colName))) {
		for (DWORD m = 0; m < this->m_data.num; m++) {
			CArray<char *> *row = this->m_data[m];
			if (!row) {
				this->m_data[m] = row = new CArray<char *>;
			}
			DWORD addCols = this->m_columns.num - row->num;
			if (addCols > 0) {
				for (DWORD i = 0; i < addCols; i++)
					row->Add(NULL);
			}
		}
	}
}
//----------------------------------------------------------------------------------
BOOL CCSVWalker::RemoveColumn(char *colName) {
	return this->RemoveColumn(this->GetColByName(colName));
}
//----------------------------------------------------------------------------------
BOOL CCSVWalker::RemoveColumn(DWORD col) {
	if (col < this->m_columns.num) {
		free(this->m_columns[col]);
		this->m_columns.Remove(col);
		
		for (DWORD m = 0; m < this->m_data.num; m++) {
			CArray<char *> *row = this->m_data[m];
			if (row && col < row->num) {
				free((*row)[col]);
				row->Remove(col);
			}
		}
		return true;
	}
	return false;
}
//----------------------------------------------------------------------------------
void CCSVWalker::AddRow() {
	CArray<char *> *row = new CArray<char *>;

	DWORD addCols = this->m_columns.num;
	if (addCols > 0) {
		for (DWORD i = 0; i < addCols; i++)
			row->Add(NULL);
	}

	this->m_data.Add(row);
	
	if (this->m_data.num) this->m_actRow = this->m_data.num - 1;
}
//----------------------------------------------------------------------------------
BOOL CCSVWalker::RemoveRow(DWORD row) {
	if (row < this->m_data.num) {
		CArray<char *> *m = this->m_data[row];
		if (m) {
			for (DWORD n = 0; n < m->num; n++)
				free((*m)[n]);
			m->RemoveAll();
			delete m;
		}
		this->m_data.Remove(row);
	}
	return false;
}
//----------------------------------------------------------------------------------
DWORD CCSVWalker::GetColByName(char *colName) {
	if (!colName || !*colName) return 0xFFFFFFFF;

	for (DWORD i = 0; i < this->m_columns.num; i++) {
		if (!_stricmp(colName, this->m_columns[i]))
			return i;
	}
	return 0xFFFFFFFF;
}
//----------------------------------------------------------------------------------
BOOL CCSVWalker::NextRow() {
	if (this->m_actRow < this->m_data.num) {
		this->m_actRow++;
		return true;
	}
	return false;
}
//----------------------------------------------------------------------------------
void CCSVWalker::Reset() {
	this->m_actRow = 0;
}
//----------------------------------------------------------------------------------
DWORD CCSVWalker::GetNumRows() {
	return this->m_data.num;
}
/***********************************************************************************
*                               END CLASS CCSVWalker                               *
***********************************************************************************/
