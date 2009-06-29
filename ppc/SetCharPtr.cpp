#include "SetCharPtr.h"
#include <stdlib.h>
#include <string.h>

/***********************************************************************************
* Sets the string                                                                  *
***********************************************************************************/
void SetCharPtr(char **ptr, char *val) {
	if (*ptr) free(*ptr);
	if (val) *ptr = _strdup(val);
	else *ptr = NULL;
}

/***********************************************************************************
* Sets the string                                                                  *
***********************************************************************************/
void SetCharPtr(WCHAR **ptr, WCHAR *val) {
	if (*ptr) free(*ptr);
	if (val) *ptr = _wcsdup(val);
	else *ptr = NULL;
}
