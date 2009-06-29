#pragma once

#include "windows.h"

template <class TDataType> class CArray {
private:
	DWORD allocated;

public:
	TDataType **data;
	DWORD num;

	BOOL Add(TDataType newData) {
		if (allocated == num) {
			TDataType **oldData = data;
			if (!allocated) allocated = 1;
			data = new TDataType *[allocated *= 2];
			if (data && oldData) memcpy(data, oldData, num * sizeof(TDataType *));
			delete[] oldData;
		}
		if (data) {
			data[num] = new TDataType;
			if (data[num]) {
				*(data[this->num]) = newData;
				this->num++;
				return true;
			}
		}
		return false;
	}
	BOOL Insert(DWORD index, TDataType newData) {
		if (index < num) {
			if (allocated == num) {
				TDataType **oldData = data;
				if (!allocated) allocated = 1;
				data = new TDataType *[allocated *= 2];
				if (data && oldData) memcpy(data, oldData, num * sizeof(TDataType *));
				free(oldData);
			}
			if (data) {
				memmove(data + index + 1, data + index, (num - index) * sizeof(TDataType *));
				data[index] = new TDataType;
				if (data[index]) {
					*(data[index]) = newData;
					this->num++;
					return true;
				}
			}
			return false;
		}else return Add(newData);
	}
	BOOL Remove(DWORD n) {
		if (n < num) {
			delete (data[n]);
			if (n != num - 1) memmove(data + n, data + n + 1, (num - n - 1) * sizeof(TDataType *));
			num--;
			return true;
		}
		
		return false;
	}
	void RemoveAll() {
		for (DWORD i = num - 1; i < num; i--)
			Remove(i);
	}
	TDataType &operator[](DWORD i) {
		return *data[i];
	}
	BOOL IsEmpty() {
		return this->num == 0;
	}

	CArray() {allocated = 0; data = NULL; num = 0;}
	CArray(int size) {allocated = size; data = new TDataType*[allocated]; num = 0;}
	~CArray() {RemoveAll(); if (data) delete [] data;}
};